//
// Created by Niccolò Arati on 08/01/2024.
//

#include "KMeansOMP.h"
#include <cfloat>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>

KMeansOMP::KMeansOMP(int K, int epochs) {
    this->K = K;
    this->epochs = epochs;
    dimensions = 0;
    nPoints = 0;
}

void KMeansOMP::clearClusters() {
    for (int i = 0; i < K; i ++) {
        clusters[i].removeAllPoints();
    }
}

int KMeansOMP::getNearestClusterId(const Point& p) {
    double sum, min_dist = DBL_MAX;
    int nearestClusterId;
    for (int i = 0; i < K; i ++) {
        double dist;
        sum = 0.0;
        if (dimensions == 1) {
            dist = fabs(clusters[i].getCentroidPos(0) - p.getVal(0));
        }
        else {
            for (int j = 0; j < dimensions; j ++) {
                sum += pow(clusters[i].getCentroidPos(j) - p.getVal(j), 2.0);
            }
            dist = sqrt(sum);
        }
        if (dist < min_dist) {
            min_dist = dist;
            nearestClusterId = clusters[i].getClusterId();
        }
    }
    return nearestClusterId;
}

void KMeansOMP::run(std::vector<Point> algPoints, int seed, int threads) {
    nPoints = (int)algPoints.size();
    dimensions = algPoints[0].getDimensions();

    //initializing clusters
    std::vector<int> usedPointsIds;
    std::random_device rd;
    std::default_random_engine eng(rd());
    eng.seed(seed);
    std::uniform_int_distribution<int> distr(0, nPoints);
    for (int i = 1; i <= K; i ++) {
        int index;
        do {
            index = distr(eng);
        } while (std::find(usedPointsIds.begin(), usedPointsIds.end(), index) != usedPointsIds.end());
        usedPointsIds.push_back(index);
        int j = (int)clusters.size() + 1;
        algPoints[index].setClusterId(j);
        Cluster cluster(j, algPoints[index]);
        clusters.push_back((cluster));
    }
    std::cout << "Clusters initialized = " << clusters.size() << std::endl << std::endl;

    std::cout << "Running K-Means OMP clustering.." << std::endl;
    int epoch = 1;
    bool run = true;
    while(run) {
        std::cout << "Epoch " << epoch << " / " << epochs << std::endl;
        int changed = 0;

        //add all points to their nearest cluster
        #pragma omp parallel for default(none) shared(algPoints, changed) num_threads(threads)
        for (int i = 0; i < nPoints; i ++) {
            int currentClusterId = algPoints[i].getClusterId();
            int nearestClusterId = getNearestClusterId(algPoints[i]);
            if (currentClusterId != nearestClusterId) {
                #pragma omp atomic
                changed ++;
                algPoints[i].setClusterId(nearestClusterId);
            }
        }

        //clear all existing clusters
        clearClusters();

        //reassign points to their new clusters
        for (int i = 0; i < nPoints; i++) {
            //cluster index is ID-1
            clusters[algPoints[i].getClusterId() - 1].addPoint(algPoints[i]);
        }

        //recalculating the center of each cluster
        for (int i = 0; i < K; i++) {
            int clusterSize = clusters[i].getClusterSize();
            for (int j = 0; j < dimensions; j++) {
                double sum = 0.0;
                #pragma omp parallel for default(none) firstprivate(i, j) shared(clusterSize) \
                reduction(+: sum) num_threads(threads)
                for (int p = 0; p < clusterSize; p++) {
                    sum += clusters[i].getPoint(p).getVal(j);
                }
                if (clusterSize > 0) {
                    clusters[i].setCentroidPos(j, sum / clusterSize);
                }
            }
        }
        if ((float)changed / (float)nPoints <= 0.001 || epoch >= epochs) {
            std::cout << "Clustering completed in epoch : " << epoch << std::endl << std::endl;
            run = false;
        }
        epoch++;
    }
}