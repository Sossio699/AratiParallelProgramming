//
// Created by Niccolò Arati on 08/01/2024.
//

#include "Point.h"

Point::Point(int id, const std::string& line, bool csv) {
    pointId = id;
    if (csv) {
        values = linetoVecCSV(line);
    }
    else {
        values = linetoVecTXT(line);
    }
    dimensions = (int)values.size();
    clusterId = 0; //not assigned
}

Point::Point(const Point &p) {
    this->pointId = p.getId();
    for (int i = 0; i < p.getDimensions(); i ++) {
        this->values.push_back(p.getVal(i));
    }
    this->dimensions = p.getDimensions();
    this->clusterId = p.getClusterId();
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

double Point::getVal(int pos) const {
    return values[pos];
}

std::vector<double> Point::linetoVecTXT(const std::string &line) {
    std::vector<double> values;
    std::string tmp = "";
    for (int i = 0; i < (int)line.length(); i ++) {
        if ((48 <= int(line[i]) && int(line[i]) <= 57) || line[i] == '.' || line[i] == '-') {
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

std::vector<double> Point::linetoVecCSV(const std::string &line) {
    std::vector<double> values;
    std::string coord;
    std::stringstream s(line);
    while(getline(s, coord, ',')) {
        if ((48 <= int(coord[0]) && int(coord[0]) <= 57 || coord[0] == '-') && (std::stoi(coord) != pointId))  {
            values.push_back(std::stod(coord));
        }
    }
    return values;
}