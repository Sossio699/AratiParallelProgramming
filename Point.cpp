//
// Created by Niccolò Arati on 08/01/2024.
//

#include "Point.h"

Point::Point(int id, std::string line) {
    pointId = id;
    values = linetoVec(line);
    dimensions = (int)values.size();
    clusterId = 0; //not assigned
}

int Point::getId() const {
    return pointId;
}

void Point::setId(int id) {
    pointId = id;
}

int Point::getClusterId() const {
    return clusterId;
}

void Point::setClusterId(int c) {
    clusterId = c;
}

int Point::getDimensions() const {
    return dimensions;
}

void Point::setDimensions(int d) {
    dimensions = d;
}

double Point::getVal(int pos) {
    return values[pos];
}

std::vector<double> Point::linetoVec(std::string &line) {
    std::vector<double> values;
    std::string tmp = "";
    for (int i = 0; i < (int)line.length(); i ++) {
        if ((48 <= int(line[i]) && int(line[i]) <= 57) || line[i] == '.') {
            tmp += line[i];
        }
        else if (!tmp.empty()) {
            values.push_back(std::stod(tmp));
            tmp = "";
        }
    }
    if (!tmp.empty()) {
        values.push_back(std::stod(tmp));
    }
    return values;
}
