//
// Created by Niccolò Arati on 08/01/2024.
//

#include "EditDistance.h"
#include <cassert>

int levenshteinDistFM(const std::string& word1, const std::string& word2) {
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

int levenshteinDistMR(const std::string& word1, const std::string& word2) {
    int length1 = (int)word1.length();
    int length2 = (int)word2.length();
    std::vector<int> prevRow(length2 + 1, 0);
    std::vector<int> currRow(length2 + 1, 0);

    if (length1 == 0) {
        return length2;
    }
    if (length2 == 0) {
        return length1;
    }
    for (int j = 0; j <= length2; j ++) {
        prevRow[j] = j;
    }
    for (int i = 0; i < length1; i ++) {
        currRow[0] = i + 1;
        for (int j = 0; j < length2; j ++) {
            int deletionCost = prevRow[j + 1] + 1;
            int insertionCost = currRow[j] + 1;
            int substitutionCost;
            if (word1[i] == word2[j]) {
                substitutionCost = prevRow[j];
            }
            else {
                substitutionCost = prevRow[j] + 1;
            }
            currRow[j + 1] = std::min(std::min(deletionCost, insertionCost), substitutionCost);
        }
        prevRow = currRow;
    }
    return currRow[length2];
}

int levenshteinDistRec(const std::string& word1, const std::string& word2, int m, int n) {
    if (m == 0) {
        return n;
    }
    if (n == 0) {
        return m;
    }
    if (word1[m - 1] == word2[n - 1]) {
        return levenshteinDistRec(word1, word2, m - 1, n - 1);
    }
    return 1 + std::min(std::min(levenshteinDistRec(word1, word2, m - 1, n), //deletion cost
                                 levenshteinDistRec(word1, word2, m, n - 1)), //insertion cost
                        levenshteinDistRec(word1, word2, m - 1, n - 1)); //substitution cost
}

std::vector<std::string> stringSearchFM(const std::vector<std::string>& vocabulary,
                                        const std::string& target, int threshold) {
    std::vector<std::string> results;
    for (int i = 0; i < (int)vocabulary.size(); i ++) {
        if (levenshteinDistFM(vocabulary[i], target) <= threshold) {
            results.push_back(vocabulary[i]);
        }
    }
    return results;
}

std::vector<std::string> stringSearchMR(const std::vector<std::string>& vocabulary,
                                        const std::string& target, int threshold) {
    std::vector<std::string> results;
    for (int i = 0; i < (int)vocabulary.size(); i ++) {
        if (levenshteinDistMR(vocabulary[i], target) <= threshold) {
            results.push_back(vocabulary[i]);
        }
    }
    return results;
}

std::vector<std::string> stringSearchRec(const std::vector<std::string>& vocabulary,
                                         const std::string& target, int threshold) {
    std::vector<std::string> results;
    for (int i = 0; i < (int)vocabulary.size(); i ++) {
        if (levenshteinDistMR(vocabulary[i], target) <= threshold) {
            results.push_back(vocabulary[i]);
        }
    }
    return results;
}
