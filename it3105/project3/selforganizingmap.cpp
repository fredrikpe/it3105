#include <math.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "selforganizingmap.h"


void SelfOrganizingMap::one_step(const point& city)
{
    int bmu_index = bestMatchingUnit(city);
    std::vector<int> bmu_neighborhood_indexes = neighborhood(bmu_index);
    updateWeights(bmu_neighborhood_indexes, city, bmu_index);
}

void SelfOrganizingMap::one_epoch()
{
    for (auto&& city : cities)
    {
        one_step(city);
    }
    epoch++;
}


int SelfOrganizingMap::bestMatchingUnit(const point &city)
{
    double currentMinimum = euclidean_distance(nodes[0], city);
    int currentIndex = 0;

    for(int i=1; i<num_of_nodes; ++i)
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
    return RADIUS;
}

int SelfOrganizingMap::radiusLinear()
{
    int r = RADIUS - epoch;
    return r < 0 ? 0 : r;
}

int SelfOrganizingMap::radiusExponential()
{
    return RADIUS * exp(-(double)epoch/(num_of_cities / log(RADIUS)));
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
    return LEARNING_RATE;
}

double SelfOrganizingMap::learningRateLinear()
{
    double rate = LEARNING_RATE -  epoch / num_of_cities;
    return rate < 0 ? 0 : rate;
}

double SelfOrganizingMap::learningRateExponential()
{
    return LEARNING_RATE * exp(-(double)epoch/(num_of_cities / log(LEARNING_RATE)));
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

double SelfOrganizingMap::influence(const point &node, const point &bmu)
{
    int r = radius();
    if (r == 0)
    {
        return euclidean_distance_squared(node, bmu) == 0 ? 1 : 0;
    }
    return exp(-euclidean_distance_squared(node, bmu)/(2 * pow(r, 2)));
}

void SelfOrganizingMap::updateWeights(std::vector<int>& neighborhood_indexes, const point& city, int bmu_index)
{
    for (auto i : neighborhood_indexes)
    {
        nodes[i].first += influence(nodes[i], nodes[bmu_index]) * learningRate() * (city.first - nodes[i].first);
        nodes[i].second += influence(nodes[i], nodes[bmu_index]) * learningRate() * (city.second - nodes[i].second);
    }
}

void SelfOrganizingMap::makeTour()
{
    vector<int> tour;
    for (auto&& city : cities)
    {
        tour.push_back(bestMatchingUnit(city));
    }

    tour_indexes.clear();
    for (int i=0; i<num_of_cities; i++)
    {
        tour_indexes.push_back(i);
    }

    mt19937 gen(random_engine());
    uniform_real_distribution<> dis(0, 1);

    // Sort indexes based on comparing values in tour
    // Choose at random if equal
    sort(tour_indexes.begin(), tour_indexes.end(),
         [&tour, &dis, &gen](int i1, int i2)
    {
        if (tour[i1] == tour[i2])
        {
            return dis(gen) < 0.5;
        }
        return tour[i1] < tour[i2];
    });
    calculateTourDistance();
}

void SelfOrganizingMap::calculateTourDistance()
{
    point current, prev = cities[tour_indexes.front()];
    tour_distance = euclidean_distance(prev, cities[tour_indexes.back()]);

    for (int i=1; i<tour_indexes.size(); i++)
    {
        current = cities[tour_indexes[i]];
        tour_distance += euclidean_distance(prev, current);
        prev = current;
    }
    tour_distance += euclidean_distance(prev, current);
}


double SelfOrganizingMap::euclidean_distance(const point &a, const point &b)
{
    return sqrt(euclidean_distance_squared(a, b));
}

double SelfOrganizingMap::euclidean_distance_squared(const point &a, const point &b)
{
    return pow(a.first - b.first, 2) + pow(a.second - b.second, 2);
}


SelfOrganizingMap::SelfOrganizingMap() {}


void SelfOrganizingMap::newCityInstance(const cityMap &data)
{
    epoch = 1;
    cities = data;

    // Shuffle cities
    shuffle(cities.begin(), cities.end(), random_engine);

    num_of_cities = cities.size();
    num_of_nodes = num_of_cities + num_of_cities / 4;
    RADIUS = num_of_cities / 10;

    nodes.clear();
    //generateRandomCityMap(nodes, num_of_nodes);
    generateCircularCityMap(nodes, 0.25, num_of_nodes);
}

void SelfOrganizingMap::generateCircularCityMap(cityMap &cm, double radius, int len)
{
    //A point at angle theta on the circle whose centre is (x0,y0) and whose radius is r is (x0 + r cos theta, y0 + r sin theta).
    //Now choose theta values evenly spaced between 0 and 2pi.
    double slice = 2*4*atan(1) / len;
    double x, y, theta, x0 = 0.5, y0 = 0.5;

    for (int i=1; i<=len; i++)
    {
        theta = slice * i;
        x = x0 + radius * cos(theta);
        y = y0 + radius * sin(theta);
        cm.push_back(make_pair(x, y));
    }
}

void SelfOrganizingMap::generateRandomCityMap(cityMap &cm, int len)
{
    mt19937 gen(random_engine());
    uniform_real_distribution<> dis(0, 1);

    for (int i=0; i<len; i++)
    {
        cm.push_back(make_pair(dis(gen), dis(gen)));
    }
}
