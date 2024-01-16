//
// Created by Niccolò Arati on 08/01/2024.
//

#ifndef PARALLELPROGRAMMING_KMEANS_H
#define PARALLELPROGRAMMING_KMEANS_H

#include <vector>
#include <string>
#include "Cluster.h"

class KMeans {
public:
    KMeans(int K, int epochs, std::string output_dir);

    void run(std::vector<Point> allPoints);

private:
    int K, epochs, dimensions, nPoints;
    std::vector<Cluster> clusters;
    std::string output_dir;

    void clearClusters();
    int getNearestClusterId(Point p);
};

#endif //PARALLELPROGRAMMING_KMEANS_H
