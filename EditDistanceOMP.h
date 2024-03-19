//
// Created by Niccolò Arati on 08/01/2024.
//

#ifndef PARALLELPROGRAMMING_EDITDISTANCEOMP_H
#define PARALLELPROGRAMMING_EDITDISTANCEOMP_H

#include <vector>
#include <string>

int levenshteinDistSD_OMP(const std::string& word1, const std::string& word2, int length1, int length2, int threads);

int levenshteinDistSD_OMP2(const std::string& word1, const std::string& word2, int length1, int length2, int threads);

#endif //PARALLELPROGRAMMING_EDITDISTANCEOMP_H
