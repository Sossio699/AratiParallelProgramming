//
// Created by Giulia on 16/01/2024.
//

#ifndef PARALLELPROGRAMMING_EDITDISTANCEOMP_H
#define PARALLELPROGRAMMING_EDITDISTANCEOMP_H

#include <vector>
#include <string>

int levenshteinDistFM_OMP(const std::string& word1, const std::string& word2, int threads);

int levenshteinDistMR_OMP(const std::string& word1, const std::string& word2, int threads);

std::vector<std::string> stringSearchFM_OMP(const std::vector<std::string>& vocabulary,
                                            const std::string& target, int threshold, int threads);

std::vector<std::string> stringSearchFM_OMP2(const std::vector<std::string>& vocabulary,
                                             const std::string& target, int threshold, int threads);

std::vector<std::string> stringSearchMR_OMP(const std::vector<std::string>& vocabulary,
                                            const std::string& target, int threshold, int threads);

std::vector<std::string> stringSearchMR_OMP2(const std::vector<std::string>& vocabulary,
                                             const std::string& target, int threshold, int threads);

#endif //PARALLELPROGRAMMING_EDITDISTANCEOMP_H
