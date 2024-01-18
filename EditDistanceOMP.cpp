//
// Created by Giulia on 16/01/2024.
//

#include "EditDistanceOMP.h"

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
    #pragma omp parallel for default (none) shared(length1, length2, distMatrix) firstprivate(word1, word2) \
    num_threads(threads)
    for (int i = 1; i <= length1; i ++) {
        for (int j = 1; j <= length2; j ++) {
            //#pragma omp flush(distMatrix)
            int substitutionCost = (word1[i - 1] == word2[j - 1]) ? 0 : 1;
            //#pragma omp critical
            distMatrix[i][j] = std::min(std::min(distMatrix[i - 1][j] + 1, //deletion cost
                                                 distMatrix[i][j - 1] + 1), //insertion cost
                                        distMatrix[i - 1][j - 1] + substitutionCost); //substitution cost
            //#pragma omp flush(distMatrix)
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
        #pragma omp parallel for default(none) shared(word1, word2, length2, prevRow, currRow) \
        firstprivate(i) num_threads(threads)
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

std::vector<std::string> stringSearchFM_OMP(const std::vector<std::string>& vocabulary,
                                            const std::string& target, int threshold, int threads) {
    std::vector<std::string> results;
    #pragma omp parallel for default(none) shared(vocabulary, target, threads, threshold, results) num_threads(threads)
    for (int i = 0; i < (int)vocabulary.size(); i ++) {
        #pragma omp flush(results)
        if (levenshteinDistFM_OMP(vocabulary[i], target, threads) <= threshold) {
            #pragma omp critical
            results.push_back(vocabulary[i]);
        }
        #pragma omp flush(results)
    }
    return results;
}

std::vector<std::string> stringSearchMR_OMP(const std::vector<std::string>& vocabulary,
                                            const std::string& target, int threshold, int threads) {
    std::vector<std::string> results;
    #pragma omp parallel for default(none) shared(vocabulary, target, threads, threshold, results) num_threads(threads)
    for (int i = 0; i < (int)vocabulary.size(); i ++) {
        #pragma omp flush(results)
        if (levenshteinDistMR_OMP(vocabulary[i], target, threads) <= threshold) {
            #pragma omp critical
            results.push_back(vocabulary[i]);
        }
        #pragma omp flush(results)
    }
    return results;
}