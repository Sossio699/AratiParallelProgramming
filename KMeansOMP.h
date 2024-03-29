//
// Created by Niccolò Arati on 08/01/2024.
//

#ifndef PARALLELPROGRAMMING_KMEANSOMP_H
#define PARALLELPROGRAMMING_KMEANSOMP_H

#include <vector>
#include <string>
#include "Cluster.h"

class KMeansOMP {
public:
    KMeansOMP(int K, int epochs);

    void run(std::vector<Point> allPoints, int seed, int threads);

private:
    int K, epochs, dimensions, nPoints;
    std::vector<Cluster> clusters;

    void clearClusters();
    int getNearestClusterId(const Point& p);
};

#endif //PARALLELPROGRAMMING_KMEANSOMP_H
