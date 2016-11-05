#ifndef SELFORGANIZINGMAP_H
#define SELFORGANIZINGMAP_H

#include <vector>
#include <utility>
#include <random>
#include <memory>
#include <algorithm>
#include <math.h>

using namespace std;

typedef pair<double, double> point;
typedef vector<point> cityMap;

enum DecayType
{
    STATIC,
    LINEAR,
    EXPONENTIAL,
};

class SelfOrganizingMap
{
private:

public:
    //SelfOrganizingMap(cityMap &data);
    SelfOrganizingMap();
    void generateRandomCityMap(cityMap &cm, int len);
    void generateCircularCityMap(cityMap &cm, double radius, int len);
    void newCityInstance(const cityMap &data);

    void makeTour();
    void calculateTourDistance();

    int epoch = 1;
    double LEARNING_RATE = 0.2;
    int RADIUS;


    // Algorithm methods
    void one_step(const point& city);
    void one_epoch();
    int bestMatchingUnit(const point &city);
    std::vector<int> neighborhood(int bmu_index);
    void updateWeights(std::vector<int>& neighborhood_indexes, const point &city, int bmu_index);
    double influence(const point &node, const point &bmu);

    double euclidean_distance_squared(const point &a, const point &b);
    double euclidean_distance(const point &a, const point &b);

    double learningRate();
    double learningRateStatic();
    double learningRateLinear();
    double learningRateExponential();

    int radius();
    int radiusStatic();
    int radiusLinear();
    int radiusExponential();

    int num_of_cities;
    int num_of_nodes;
    cityMap cities;
    cityMap nodes;

    vector<int> tour_indexes;
    double tour_distance = 0.0;

    DecayType decayType = LINEAR;

    random_device random_engine;
};





#endif // SELFORGANIZINGMAP_H
