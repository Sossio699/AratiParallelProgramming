//
// Created by Niccolò Arati on 08/01/2024.
//

#include "KMeansOMP.h"
#include <cstdlib>
#include <cfloat>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>

KMeansOMP::KMeansOMP(int K, int epochs, std::string output_dir) {
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
    for (int i = 0; i < K; i ++) {
        double dist;
        sum = 0.0;
        if (dimensions == 1) {
            dist = fabs(clusters[i].getCentroidPos(0) - p.getVal(0));
        }
        else {
            #pragma omp parallel for default(none) firstprivate(p, i) num_threads(2) \
            reduction(+: sum)
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

void KMeansOMP::run(std::vector<Point> algPoints, int threads) {
    nPoints = (int)algPoints.size();
    dimensions = algPoints[0].getDimensions();
    //initializing clusters
    std::vector<int> usedPointsIds;
    bool init;
    #pragma omp parallel for default(none) private(init) shared(algPoints, usedPointsIds) num_threads(2)
    for (int i = 1; i <= K; i ++) {
        init = false;
        while(!init) {
            int index = rand() % nPoints;
            #pragma omp flush(usedPointsIds)
            if (std::find(usedPointsIds.begin(), usedPointsIds.end(), index) == usedPointsIds.end()) {
                #pragma omp critical
                usedPointsIds.push_back(index);
                algPoints[index].setClusterId(i);
                Cluster cluster(i, algPoints[index]);
                clusters.push_back(cluster);
                init = true;
                #pragma omp flush(usedPointsIds)
            }
        }
    }
    std::cout << "Clusters initialized = " << clusters.size() << std::endl << std::endl;
    std::cout << "Running K-Means clustering.." << std::endl;
    int epoch = 1;
    bool run = true;
    while(run) {
        std::cout << "Epoch " << epoch << " / " << epochs << std::endl;
        bool changed = false;
        //add all points to their nearest cluster
        #pragma omp parallel for default(none) firstprivate(algPoints) reduction(||: changed) num_threads(2)
        for (int i = 0; i < nPoints; i ++) {
            int currentClusterId = algPoints[i].getClusterId();
            int nearestClusterId = getNearestClusterId(algPoints[i]);
            if (currentClusterId != nearestClusterId) {
                algPoints[i].setClusterId(nearestClusterId);
                changed = true;
            }
        }
        //clear all existing clusters
        clearClusters();
        //reassign points to their new clusters
        #pragma omp parallel for default(none) shared(algPoints) num_threads(2)
        for (int i = 0; i < nPoints; i ++) {
            //cluster index is ID-1
            clusters[algPoints[i].getClusterId() - 1].addPoint(algPoints[i]);
        }
        //recalculating the center of each cluster
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
    pointsFile.open(output_dir + "/" + std::to_string(K) + "MeansOMP-points.txt", std::ios::out);
    if (pointsFile.is_open()) {
        std::cout << "Writing assigned cluster for each point" << std::endl;
        for (int i = 0; i < nPoints; i++) {
            pointsFile << algPoints[i].getId() << " " << algPoints[i].getClusterId() << std::endl;
        }
        pointsFile.close();
    }
    else {
        std::cout << "Error: unable to write point results" << std::endl;
    }
    //write cluster centers to file
    std::ofstream outfile;
    outfile.open(output_dir + "/" + std::to_string(K) + "MeansOMP-clusters.txt");
    if (outfile.is_open()) {
        std::cout << "Writing cluster coordinates" << std::endl;
        for (int i = 0; i < K; i ++) {
            outfile << clusters[i].getClusterId() << " ";
            for (int j = 0; j < dimensions; j ++) {
                outfile << clusters[i].getCentroidPos(j) << " "; //output to file
            }
            outfile << std::endl;
        }
        outfile.close();
    }
    else {
        std::cout << "Error: unable to write cluster coordinates" << std::endl;
    }
}