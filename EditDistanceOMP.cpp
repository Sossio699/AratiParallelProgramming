//
// Created by Giulia on 16/01/2024.
//

#include "EditDistanceOMP.h"

//with full matrix approach
int levenshteinDistOMP(std::string word1, std::string word2, int threads) {
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
        #pragma omp parallel for default (none) shared(length2, word1, word2, distMatrix) firstprivate(i) \
        num_threads(threads)
        for (int j = 1; j <= length2; j ++) {
            #pragma omp flush(distMatrix)
            int substitutionCost = (word1[i - 1] == word2[j - 1]) ? 0 : 1;
            #pragma omp critical
            distMatrix[i][j] = std::min(std::min(distMatrix[i - 1][j] + 1, //deletion cost
                                                 distMatrix[i][j - 1] + 1), //insertion cost
                                        distMatrix[i - 1][j - 1] + substitutionCost); //substitution cost
            #pragma omp flush(distMatrix)
        }
    }
    return distMatrix[length1][length2];
}

std::vector<std::string> stringSearchOMP(std::vector<std::string> vocabulary,
                                         const std::string& target, int threshold, int threads) {
    std::vector<std::string> results;
    #pragma omp parallel for default(none) shared(vocabulary, target, threads, threshold, results) num_threads(threads)
    for (int i = 0; i < (int)vocabulary.size(); i ++) {
        #pragma omp flush(results)
        if (levenshteinDistOMP(vocabulary[i], target, threads) <= threshold) {
            #pragma omp critical
            results.push_back(vocabulary[i]);
        }
        #pragma omp flush(results)
    }
    return results;
}