//
// Created by Niccolò Arati on 08/01/2024.
//

#include <vector>
#include "EditDistance.h"


int levenshteinDistFM(const std::string& word1, const std::string& word2, int length1, int length2) {
    int N = length1 + 1;
    int M = length2 + 1;

    std::vector<int> distMatrix(N * M);

    if (length1 == 0) {
        return length2;
    }
    if (length2 == 0) {
        return length1;
    }
    for (int i = 0; i <= length1; i ++) {
        distMatrix[i] = i;
    }
    for (int j = 0; j <= length2; j ++) {
        distMatrix[j * M] = j;
    }
    //matrix filling
    for (int i = 1; i <= length1; i ++) {
        for (int j = 1; j <= length2; j ++) {
            int substitutionCost = (word1[i - 1] == word2[j - 1]) ? 0 : 1;
            distMatrix[i * N +j] = std::min(std::min(distMatrix[(i - 1) * M + j] + 1, //deletion cost
                                                 distMatrix[i * M + j - 1] + 1), //insertion cost
                                        distMatrix[(i - 1) * M + j - 1] + substitutionCost); //substitution cost
        }
    }

    int result = distMatrix[length1 * N + length2];
    return result;
}

int levenshteinDistSD(const std::string& word1, const std::string& word2, int length1, int length2) {
    int N = length1 + 1;
    int M = length2 + 1;

    std::vector<int> distMatrix(N * M);

    if (length1 == 0) {
        return length2;
    }
    if (length2 == 0) {
        return length1;
    }

    for (int i = 0; i < M; i ++) {
        distMatrix[i] = i;
    }
    for (int j = 0; j < N; j ++) {
        distMatrix[j * M] = j;
    }

    //begin algorithm
    int dMIN = 1 - M;
    int dMAX = N - 1;
    for (int d = dMIN; d <= dMAX; d ++) {
        int iMIN = std::max(d, 1);
        int iMAX = std::min(M + d, N - 1);
        for (int i = iMIN; i <= iMAX; i ++) {
            int k = d < 0 ? 1 : -1;
            int j = M + d - i + k;
            if (word1[i - 1] != word2[j - 1]) {
                distMatrix[i * N + j] = std::min(std::min(distMatrix[(i - 1) * M + j], distMatrix[i * M + j - 1]), distMatrix[(i - 1) * M + j - 1]) + 1;
            }
            else {
                distMatrix[i * N + j] = distMatrix[(i - 1) * N + j - 1];
            }
        }
        if (d == -1) {
            d += 2;
        }
    }
    int result = distMatrix[length1 * N + length2];
    return result;
}

int levenshteinDistMR(const std::string& word1, const std::string& word2, int length1, int length2) {
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