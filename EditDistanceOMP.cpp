//
// Created by Niccolò Arati on 08/01/2024.
//

#include <iostream>
#include "EditDistanceOMP.h"
#include "EditDistance.h"

//with full matrix approach
int levenshteinDistFM_OMP(const std::string& word1, const std::string& word2, int threads) {
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
    #pragma omp parallel for default (none) collapse(2) shared(length1, length2, distMatrix) firstprivate(word1, word2) \
    num_threads(threads)
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

int levenshteinDistMR_OMP(const std::string& word1, const std::string& word2, int threads) {
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
        #pragma omp parallel for default(none) shared(length2, prevRow, currRow) \
        firstprivate(i, word1, word2) num_threads(threads)
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


std::vector<int> stringSearchFM_OMP(const std::vector<std::string>& vocabulary,
                                            const std::string& target, int threads) {
    std::vector<int> results;
    results.reserve((int)vocabulary.size());
    int result;
    #pragma omp parallel for default(none) shared(vocabulary, target, threads, results) private(result) \
    num_threads(threads)
    for (int i = 0; i < (int)vocabulary.size(); i ++) {
        result = levenshteinDistFM_OMP(vocabulary[i], target, threads);
        #pragma omp critical
        {
            results.push_back(result);
        }
    }
    return results;
}

std::vector<int> stringSearchFM_OMP2(const std::vector<std::string>& vocabulary,
                                             const std::string& target, int threads) {
    std::vector<int> results;
    results.reserve((int)vocabulary.size());
    int result;
    #pragma omp parallel for default(none) shared(vocabulary, target, results) private(result) num_threads(threads)
    for (int i = 0; i < (int)vocabulary.size(); i ++) {
        result = levenshteinDistFM(vocabulary[i], target);
        #pragma omp critical
        {
            results.push_back(result);
        }
    }
    return results;
}

std::vector<int> stringSearchMR_OMP(const std::vector<std::string>& vocabulary,
                                            const std::string& target, int threads) {
    std::vector<int> results;
    results.reserve((int)vocabulary.size());
    int result;
    #pragma omp parallel for default(none) shared(vocabulary, target, threads, results) private(result) num_threads(threads)
    for (int i = 0; i < (int)vocabulary.size(); i ++) {
        result = levenshteinDistMR_OMP(vocabulary[i], target, threads);
        #pragma omp critical
        {
            results.push_back(result);
        }
    }
    return results;
}

std::vector<int> stringSearchMR_OMP2(const std::vector<std::string>& vocabulary,
                                             const std::string& target, int threads) {
    std::vector<int> results;
    results.reserve((int)vocabulary.size());
    int result;
    #pragma omp parallel for default(none) shared(vocabulary, target, results) private(result) num_threads(threads)
    for (int i = 0; i < (int)vocabulary.size(); i ++) {
        result = levenshteinDistMR(vocabulary[i], target);
        #pragma omp critical
        {
            results.push_back(result);
        }
    }
    return results;
}