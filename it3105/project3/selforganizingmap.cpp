#include <math.h>
#include <iostream>
#include <chrono>
#include <thread>

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
    for(int i=0; i<num_of_nodes; ++i)
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
    switch (decayType)
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
    int radius = num_of_cities / 2 - step / 2;
    return radius < 0 ? 0 : radius;
}

int SelfOrganizingMap::radiusExponential()
{
    return 1;
}

double SelfOrganizingMap::learningRate()
{
    switch (decayType)
    {
    case STATIC:
        return learningRateStatic();
    case LINEAR:
        return learningRateLinear();
    case EXPONENTIAL:
        return learningRateExponential();
    }
}

double SelfOrganizingMap::learningRateStatic()
{
    return 0.3;
}

double SelfOrganizingMap::learningRateLinear()
{
    double rate = 1 - step*0.01;
    return rate < 0 ? 0 : rate;
}

double SelfOrganizingMap::learningRateExponential()
{
    return exp(-step/25.0);
}


std::vector<int> SelfOrganizingMap::neighborhood(int bmu_index)
{
    int r = radius();
    std::vector<int> neighborhood_indexes;

    for (int i=bmu_index - r; i<=bmu_index + r; ++i)
    {
        int index = i % num_of_nodes;
        index = index >= 0 ? index : index + num_of_nodes;
        neighborhood_indexes.push_back(index);
    }
    return neighborhood_indexes;
}


void SelfOrganizingMap::updateWeights(std::vector<int>& neighborhood_indexes, point& city, int bmu_index)
{
    for (auto i : neighborhood_indexes)
    {
        //double dist = 1; //euclidean_distance(nodes[i], nodes[bmu_index]);
        nodes[i].first += learningRate() * (city.first - nodes[i].first);
        nodes[i].second += learningRate() * (city.second - nodes[i].second);
        //cout << dist << ", " << nodes[i].first << ", " << nodes[i].second << endl;
    }
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
    num_of_nodes = num_of_cities + 5;
    cities.clear();
    randomCityMap(cities, num_of_cities);
    this_thread::sleep_for(chrono::seconds(1));
    randomCityMap(nodes, num_of_nodes);
}


void SelfOrganizingMap::randomCityMap(cityMap &cm, int len)
{
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    for (int i=0; i<len; i++)
    {
        cm.push_back(make_pair(dis(gen), dis(gen)));
    }
}
