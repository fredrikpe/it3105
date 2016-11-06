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

    double LEARNING_RATE = 0.2;
    int RADIUS;


    // Algorithm methods
    void one_step(const point& city);
    void one_epoch();
    int bestMatchingUnit(const point &city);
    int bestMatchingUnitSerialized(const point &city);
    std::vector<int> neighborhood(int bmu_index);
    void updateWeights(std::vector<int>& neighborhood_indexes, const point &city, int bmu_index);

    double euclidean_distance_squared(const point &a, const point &b);
    double euclidean_distance(const point &a, const point &b);
    double euclidean_distance_scaled(const point &a, const point &b);

    double learningRate();
    double learningRateStatic();
    double learningRateLinear();
    double learningRateExponential();

    int radius();
    int radiusStatic();
    int radiusLinear();
    int radiusExponential();

    double influence(const point &node, const point &bmu);
    double influence_static();
    double influence_gaussian(const point &node, const point &bmu);

    int num_of_cities;
    int num_of_nodes;
    int num_of_iterations = 20;
    cityMap cities;
    cityMap nodes;

    vector<int> tour_indexes;

    double tour_distance = 0.0;
    int epoch = 1;
    int epoch_radius;
    double epoch_learning_rate;
    vector<int> epoch_bmu_indexes;

    double x_scaling;
    double y_scaling;

    DecayType decay_type = LINEAR;
    DecayType influence_type = EXPONENTIAL;

    random_device random_engine;
};





#endif // SELFORGANIZINGMAP_H
