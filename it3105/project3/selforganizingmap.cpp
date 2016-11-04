#include <math.h>
#include <iostream>

#include "selforganizingmap.h"


void SelfOrganizingMap::one_iteration(point& city)
{
    //cout << city.first << ", " << city.second << endl;
    int bmu_index = bestMatchingUnit(city);
    std::cout << "bmu_index: " << bmu_index << std::endl;
    std::vector<int> bmu_neighborhood_indexes = neighborhood(bmu_index);
    //std::cout << "2" << std::endl;
    updateWeights(bmu_neighborhood_indexes, city, bmu_index);
    //std::cout << "3" << std::endl;
    step++;
}


int SelfOrganizingMap::bestMatchingUnit(point &city)
{
    double currentMinimum = INFINITY;
    int currentIndex;
    for(int i=0; i<num_of_cities; ++i)
    {
        double dist = euclidean_distance(nodes[i], city);
        if (dist < currentMinimum)
        {
            currentMinimum = dist;
            currentIndex = i;
        }
    }
    return currentIndex;
}

int SelfOrganizingMap::radius()
{
    switch (radiusType)
    {
    case STATIC:
        return radiusStatic();
    case LINEAR:
        return radiusLinear();
    case EXPONENTIAL:
        return radiusExponential();
    }
}


int SelfOrganizingMap::radiusStatic()
{
    return 1;
}

int SelfOrganizingMap::radiusLinear()
{
    int radius = num_of_cities - step / 2;
    if (radius < 0) {
        return 0;
    }
    return radius;
}

int SelfOrganizingMap::radiusExponential()
{
    return 1;
}


std::vector<int> SelfOrganizingMap::neighborhood(int bmu_index)
{
    int r = radius();
    std::vector<int> neighborhood_indexes;

    for (int i=bmu_index - r; i<=bmu_index + r; ++i)
    {
        int index = i % num_of_cities;
        index = index >= 0 ? index : index + num_of_cities;
        neighborhood_indexes.push_back(index);
    }
    return neighborhood_indexes;
}


void SelfOrganizingMap::updateWeights(std::vector<int>& neighborhood_indexes, point& city, int bmu_index)
{
    for (auto i : neighborhood_indexes)
    {
        //double dist = 1; //euclidean_distance(nodes[i], nodes[bmu_index]);
        nodes[i].first += learning_rate() * (city.first - nodes[i].first);
        nodes[i].second += learning_rate() * (city.second - nodes[i].second);
        //cout << dist << ", " << nodes[i].first << ", " << nodes[i].second << endl;
    }
}


double SelfOrganizingMap::learning_rate()
{
    return exp(-step/25.0);
}


double SelfOrganizingMap::euclidean_distance(point a, point b)
{
    //cout << a.first << " a, b " << b.first << endl;
    return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}


SelfOrganizingMap::SelfOrganizingMap() {}


SelfOrganizingMap::SelfOrganizingMap(cityMap &data)
    : cities(data)
{
    num_of_cities = cities.size();
    for (int i=0; i<num_of_cities; i++)
    {
        nodes.push_back(SelfOrganizingMap::randomWeightVector());
    }
}


point SelfOrganizingMap::randomWeightVector()
{
    point r;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    for (int i=0; i<num_of_cities; i++)
    {
        r = make_pair(dis(gen), dis(gen));
    }
    return r;
}
