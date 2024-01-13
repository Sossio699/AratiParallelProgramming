//
// Created by Niccolò Arati on 08/01/2024.
//

#include "EditDistance.h"

//with full matrix approach
int levenshteinDist(std::string word1, std::string word2) {
    int length1 = (int)word1.length();
    int length2 = (int)word2.length();
    int distMatrix[length1 + 1][length2 + 1]; //will store the calculated distance

    if (length1 == 0) {
        return length2;
    }
    if (length2 == 0) {
        return length1;
    }
    for (int i = 0; i <= length1; i ++) {
        distMatrix[i][0] = i;
    }
    for (int j = 0; j <= length2; j ++) {
        distMatrix[0][j] = j;
    }
    //matrix filling
    for (int i = 1; i <= length1; i ++) {
        for (int j = 1; j <= length2; j ++) {
            int substitutionCost = (word1[i - 1] == word2[j - 1]) ? 0 : 1;
            distMatrix[i][j] = std::min(std::min(distMatrix[i - 1][j] + 1, //deletion cost
                                                 distMatrix[i][j - 1] + 1), //insertion cost
                                        distMatrix[i - 1][j - 1] + substitutionCost); //substitution cost
        }
    }
    return distMatrix[length1][length2];
}

std::vector<std::string> stringSearch(std::vector<std::string> vocabulary, const std::string& target, int threshold) {
    std::vector<std::string> results;
    for (int i = 0; i < (int)vocabulary.size(); i ++) {
        if (levenshteinDist(vocabulary[i], target) <= threshold) {
            results.push_back(vocabulary[i]);
        }
    }
    return results;
}

