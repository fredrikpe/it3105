#ifndef SELFORGANIZINGMAP_H
#define SELFORGANIZINGMAP_H

#include <vector>
#include <utility>
#include <random>
#include <memory>

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
    int step = 1;

public:
    SelfOrganizingMap(cityMap &data);
    SelfOrganizingMap();
    void randomCityMap(cityMap &cm, int len);

    // Algorithm methods
    void one_iteration(point& city);
    int bestMatchingUnit(point &city);
    std::vector<int> neighborhood(int bmu_index);
    void updateWeights(std::vector<int>& neighborhood_indexes, point& city, int bmu_index);
    double euclidean_distance(point a, point b);

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
    DecayType decayType = LINEAR;

    random_device rd;
};





#endif // SELFORGANIZINGMAP_H
