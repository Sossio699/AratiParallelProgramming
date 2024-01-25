//
// Created by Niccolò Arati on 08/01/2024.
//

#include "Cluster.h"

Cluster::Cluster(int clusterId, const Point& centroid) {
    this->clusterId = clusterId;
    for (int i = 0; i < centroid.getDimensions(); i ++) {
        this->centroid.push_back(centroid.getVal(i));
    }
    this->addPoint(centroid);
}

int Cluster::getClusterId() const {
    return clusterId;
}

void Cluster::setClusterId(int id) {
    clusterId = id;
}

double Cluster::getCentroidPos(int pos) {
    return centroid[pos];
}

void Cluster::setCentroidPos(int pos, double value) {
    this->centroid[pos] = value;
}

void Cluster::addPoint(Point p) {
    p.setClusterId(this->clusterId);
    points.push_back(p);
}

void Cluster::removeAllPoints() {
    points.clear();
}

Point Cluster::getPoint(int pos) {
    return points[pos];
}

int Cluster::getClusterSize() {
    return (int)points.size();
}
