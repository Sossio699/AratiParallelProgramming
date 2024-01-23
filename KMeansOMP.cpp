//
// Created by Niccolò Arati on 08/01/2024.
//

#include "KMeansOMP.h"
#include <cfloat>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>

KMeansOMP::KMeansOMP(int K, int epochs, const std::string& output_dir) {
    this->K = K;
    this->epochs = epochs;
    this->output_dir = output_dir;
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
    #pragma omp parallel for default(none) private(sum) shared(p, min_dist, nearestClusterId) num_threads(K)
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
            #pragma omp critical
            {
                min_dist = dist;
                nearestClusterId = clusters[i].getClusterId();
            }
        }
    }
    return nearestClusterId;
}

void KMeansOMP::run(std::vector<Point> algPoints, int threads) {
    nPoints = (int)algPoints.size();
    dimensions = algPoints[0].getDimensions();
    //initializing clusters
    std::cout << "Initializing clusters" << std::endl;
    std::vector<int> usedPointsIds;
    bool init;
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution distr(0, nPoints);
    #pragma omp parallel for default(none) private(init) shared(algPoints, usedPointsIds, distr, eng, std::cout) \
    num_threads(K)
    for (int i = 1; i <= K; i ++) {
        int index = distr(eng);
        std::cout << index << std::endl;
        while (std::find(usedPointsIds.begin(), usedPointsIds.end(), index) != usedPointsIds.end()) {
            index = distr(eng);
            std::cout << index << std::endl;
        }
        #pragma omp critical
        {
            usedPointsIds.push_back(index);
            int j = (int)clusters.size() + 1;
            algPoints[index].setClusterId(j);
            Cluster cluster(j, algPoints[index]);
            clusters.push_back((cluster));
        }
    }
    std::cout << "Clusters initialized = " << clusters.size() << std::endl << std::endl;
    std::cout << clusters[0].getClusterId() << " " << clusters[1].getClusterId() << std::endl;
    std::cout << "Running K-Means clustering.." << std::endl;
    int epoch = 1;
    bool run = true;
    while(run) {
        std::cout << "Epoch " << epoch << " / " << epochs << std::endl;
        bool changed = false;
        //add all points to their nearest cluster
        std::cout << "Adding all points to their nearest cluster" << std::endl;
        #pragma omp parallel for default(none) shared(algPoints, std::cout) reduction(||:changed) num_threads(threads)
        for (int i = 0; i < nPoints; i ++) {
            int currentClusterId = algPoints[i].getClusterId();
            int nearestClusterId = getNearestClusterId(algPoints[i]);
            std::cout << "Current: " << currentClusterId << ", nearest: " << nearestClusterId << std::endl;
            if (currentClusterId != nearestClusterId) {
                #pragma omp critical
                {
                    algPoints[i].setClusterId(nearestClusterId);
                    changed = true;
                }
            }
        }
        //clear all existing clusters
        std::cout << "Clearing all existing clusters" << std::endl;
        clearClusters();
        //reassign points to their new clusters
        std::cout << "Reassigning points to their new clusters" << std::endl;
        for (int i = 0; i < nPoints; i ++) {
            //cluster index is ID-1
            clusters[algPoints[i].getClusterId() - 1].addPoint(algPoints[i]);
        }
        //recalculating the center of each cluster
        std::cout << "Recalculating the center of each cluster" << std::endl;
        for (int i = 0; i < K; i ++) {
            int clusterSize = clusters[i].getClusterSize();
            for (int j = 0; j < dimensions; j ++) {
                double sum = 0.0;
                #pragma omp parallel for default(none) firstprivate(i, j) shared(clusterSize) reduction(+: sum) \
                num_threads(threads)
                for (int p = 0; p < clusterSize; p++) {
                    sum += clusters[i].getPoint(p).getVal(j);
                }
                if (clusterSize > 0) {
                    clusters[i].setCentroidPos(j, sum / clusterSize);
                }
            }
        }
        if (!changed || epoch >= epochs) {
            std::cout << "Clustering completed in epoch : " << epoch << std::endl << std::endl;
            run = false;
        }
        epoch++;
    }
    //writing results
    std::ofstream pointsFile;
    pointsFile.open(output_dir + "/" + std::to_string(K) + "MeansOMP-points.csv", std::ios::out);
    if (pointsFile.is_open()) {
        std::cout << "Writing assigned cluster for each point" << std::endl;
        for (int i = 0; i < nPoints; i++) {
            pointsFile << algPoints[i].getId() << "," << algPoints[i].getClusterId() << "\n";
        }
        pointsFile.close();
    }
    else {
        std::cout << "Error: unable to write point results" << std::endl;
    }
    //write cluster centers to file
    std::ofstream outFile;
    outFile.open(output_dir + "/" + std::to_string(K) + "MeansOMP-clusters.csv", std::ios::out);
    if (outFile.is_open()) {
        std::cout << "Writing cluster coordinates" << std::endl;
        for (int i = 0; i < K; i ++) {
            outFile << clusters[i].getClusterId() << ",";
            for (int j = 0; j < dimensions; j ++) {
                outFile << clusters[i].getCentroidPos(j) << ","; //output to file
            }
            outFile << "\n";
        }
        outFile.close();
    }
    else {
        std::cout << "Error: unable to write cluster coordinates" << std::endl;
    }
    std::cout << "END" << std::endl;
}