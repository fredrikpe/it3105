#include <math.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <omp.h>

#include "selforganizingmap.h"


void SelfOrganizingMap::one_step(const point& city)
{
    int bmu_index = bestMatchingUnit(city);
    std::vector<int> bmu_neighborhood_indexes = neighborhood(bmu_index);
    updateWeights(bmu_neighborhood_indexes, city, bmu_index);
}

void SelfOrganizingMap::one_epoch()
{
    epoch_radius = radius();
    epoch_learning_rate = learningRate();

    for (auto&& city : cities)
    {
        one_step(city);
    }
    epoch++;
}

int SelfOrganizingMap::bestMatchingUnit(const point &city)
{
    double minimum_distance = euclidean_distance_squared(nodes[0], city);
    int minimum_index = 0;

#pragma omp parallel
    {
        double min_local = minimum_distance;
        int index_local = minimum_index;

#pragma omp for nowait
        for (int i=1; i<num_of_nodes; ++i)
        {
            double distance = euclidean_distance_squared(nodes[i], city);
            if (distance < min_local)
            {
                min_local = distance;
                index_local = i;
            }
        }

#pragma omp critical
        {
            if (min_local < minimum_distance)
            {
                minimum_distance = min_local;
                minimum_index = index_local;
            }
        }
    }
    return minimum_index;
}

int SelfOrganizingMap::bestMatchingUnitSerialized(const point &city)
{
    double current_minimum = euclidean_distance_squared(nodes[0], city);
    int current_index = 0;

    for(int i=1; i<num_of_nodes; ++i)
    {
        double dist = euclidean_distance_squared(nodes[i], city);
        if (dist < current_minimum)
        {
            current_minimum = dist;
            current_index = i;
        }
    }
    return current_index;
}

int SelfOrganizingMap::radius()
{
    switch (decay_type)
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
    return 1; //RADIUS / 3;
}

int SelfOrganizingMap::radiusLinear()
{
    int r = RADIUS - epoch * RADIUS / num_of_iterations;
    return r < 0 ? 0 : r;
}

int SelfOrganizingMap::radiusExponential()
{
    return RADIUS * exp(-(double)epoch*4/num_of_iterations);
}

double SelfOrganizingMap::learningRate()
{
    switch (decay_type)
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
    double rate = LEARNING_RATE - (double)epoch * 0.5 * LEARNING_RATE / num_of_iterations;
    return rate < 0 ? 0 : rate;
}

double SelfOrganizingMap::learningRateExponential()
{
    return LEARNING_RATE * exp(-(double)epoch/(num_of_iterations * 4));
}


std::vector<int> SelfOrganizingMap::neighborhood(int bmu_index)
{
    std::vector<int> neighborhood_indexes;

    for (int i=bmu_index - epoch_radius; i<=bmu_index + epoch_radius; ++i)
    {
        int index = i % num_of_nodes;
        index = index >= 0 ? index : index + num_of_nodes;
        neighborhood_indexes.push_back(index);
    }
    return neighborhood_indexes;
}

double SelfOrganizingMap::influence(const point &node, const point &bmu)
{
    switch (influence_type)
    {
    case STATIC:
        return influence_static();
    case EXPONENTIAL:
        return influence_gaussian(node, bmu);
    }
}

double SelfOrganizingMap::influence_static()
{
    return 1;
}

double SelfOrganizingMap::influence_gaussian(const point &node, const point &bmu)
{
    if (epoch_radius == 0)
    {
        return euclidean_distance_squared(node, bmu) == 0 ? 1 : 0;
    }
    return exp(-euclidean_distance(node, bmu)/(2 * pow(epoch_radius, 2)));
}

void SelfOrganizingMap::updateWeights(std::vector<int>& neighborhood_indexes, const point& city, int bmu_index)
{
    for (auto i : neighborhood_indexes)
    {
        nodes[i].first += influence(nodes[i], nodes[bmu_index]) * epoch_learning_rate * (city.first - nodes[i].first);
        nodes[i].second += influence(nodes[i], nodes[bmu_index]) * epoch_learning_rate * (city.second - nodes[i].second);
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
    tour_distance = euclidean_distance_scaled(prev, cities[tour_indexes.back()]);

    for (int i=1; i<tour_indexes.size(); i++)
    {
        current = cities[tour_indexes[i]];
        tour_distance += euclidean_distance_scaled(prev, current);
        prev = current;
    }
    tour_distance += euclidean_distance_scaled(prev, current);
}


double SelfOrganizingMap::euclidean_distance(const point &a, const point &b)
{
    return sqrt(euclidean_distance_squared(a, b));
}

double SelfOrganizingMap::euclidean_distance_squared(const point &a, const point &b)
{
    return pow(a.first - b.first, 2) + pow(a.second - b.second, 2);
}

double SelfOrganizingMap::euclidean_distance_scaled(const point &a, const point &b)
{
    return sqrt(pow((a.first - b.first) * x_scaling, 2) + pow((a.second - b.second) * y_scaling, 2));
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

    if (num_of_cities > 10000)
    {
        RADIUS = num_of_cities / 64;
    }
    if (num_of_cities > 1000)
    {
        RADIUS = num_of_cities / 8;
    }
    else
    {
        RADIUS = num_of_cities / 4;
    }

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
