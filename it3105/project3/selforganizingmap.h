#ifndef SELFORGANIZINGMAP_H
#define SELFORGANIZINGMAP_H

#include <vector>
#include <utility>
#include <random>
#include <memory>

using namespace std;

typedef pair<double, double> point;
typedef vector<point> cityMap;

enum RadiusType
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
    point randomWeightVector();

    // Algorithm methods
    void one_iteration(point& city);
    int bestMatchingUnit(point &city);
    std::vector<int> neighborhood(int bmu_index);
    void updateWeights(std::vector<int>& neighborhood_indexes, point& city, int bmu_index);
    double euclidean_distance(point a, point b);
    int radiusStatic();
    int radiusLinear();
    int radiusExponential();
    int radius();

    double learning_rate();

    int num_of_cities;
    cityMap cities;
    cityMap nodes;
    RadiusType radiusType = LINEAR;
};





#endif // SELFORGANIZINGMAP_H
