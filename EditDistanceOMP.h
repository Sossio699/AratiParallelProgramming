//
// Created by Giulia on 16/01/2024.
//

#ifndef PARALLELPROGRAMMING_EDITDISTANCEOMP_H
#define PARALLELPROGRAMMING_EDITDISTANCEOMP_H

#include <vector>
#include <string>

int levenshteinDistOMP(const std::string& word1, const std::string& word2, int threads);

std::vector<std::string> stringSearchOMP(const std::vector<std::string>& vocabulary,
                                         const std::string& target, int threshold, int threads);

#endif //PARALLELPROGRAMMING_EDITDISTANCEOMP_H
