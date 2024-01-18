//
// Created by Niccolò Arati on 08/01/2024.
//

#ifndef PARALLELPROGRAMMING_EDITDISTANCE_H
#define PARALLELPROGRAMMING_EDITDISTANCE_H

#include <string>
#include <vector>

int levenshteinDistFM(const std::string& word1, const std::string& word2);

int levenshteinDistMR(const std::string& word1, const std::string& word2);

int levenshteinDistRec(const std::string& word1, const std::string& word2, int m, int n);

std::vector<std::string> stringSearchFM(const std::vector<std::string>& vocabulary,
                                        const std::string& target, int threshold);

std::vector<std::string> stringSearchMR(const std::vector<std::string>& vocabulary,
                                        const std::string& target, int threshold);

std::vector<std::string> stringSearchRec(const std::vector<std::string>& vocabulary,
                                         const std::string& target, int threshold);

#endif //PARALLELPROGRAMMING_EDITDISTANCE_H
