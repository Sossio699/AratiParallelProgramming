//
// Created by Niccolò Arati on 08/01/2024.
//

#include <vector>
#include "EditDistanceOMP.h"


int levenshteinDistSD_OMP(const std::string& word1, const std::string& word2, int length1, int length2, int threads) {
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
    #pragma omp parallel default(none) firstprivate(dMIN, dMAX, M, N) \
    shared(distMatrix, word1, word2) num_threads(threads)
    {
        for (int d = dMIN; d <= dMAX; d++) {
            int iMIN = std::max(d, 1);
            int iMAX = std::min(M + d, N - 1);
            #pragma omp for
            for (int i = iMIN; i <= iMAX; i++) {
                int k = d < 0 ? 1 : -1;
                int j = M + d - i + k;
                if (word1[i - 1] != word2[j - 1]) {
                    distMatrix[i * N + j] = std::min(std::min(distMatrix[(i - 1) * M + j],
                                                              distMatrix[i * M + j - 1]),
                                                     distMatrix[(i - 1) * M + j - 1]) + 1;
                } else {
                    distMatrix[i * N + j] = distMatrix[(i - 1) * N + j - 1];
                }
            }
            if (d == -1) {
                d += 2;
            }
        }
    }
    int result = distMatrix[length1 * N + length2];
    return result;
}

int levenshteinDistSD_OMP2(const std::string& word1, const std::string& word2, int length1, int length2, int threads) {
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
    for (int d = dMIN; d <= dMAX; d++) {
        int iMIN = std::max(d, 1);
        int iMAX = std::min(M + d, N - 1);
        #pragma omp parallel for default(none) firstprivate(iMIN, iMAX, d, M, N) \
        shared(distMatrix, word1, word2) num_threads(threads)
        for (int i = iMIN; i <= iMAX; i++) {
            int k = d < 0 ? 1 : -1;
            int j = M + d - i + k;
            if (word1[i - 1] != word2[j - 1]) {
                distMatrix[i * N + j] = std::min(std::min(distMatrix[(i - 1) * M + j],
                                                          distMatrix[i * M + j - 1]),
                                                 distMatrix[(i - 1) * M + j - 1]) + 1;
            } else {
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