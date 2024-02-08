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

std::vector<int> stringSearchFM(const std::vector<std::string>& vocabulary, const std::string& target);

std::vector<int> stringSearchMR(const std::vector<std::string>& vocabulary, const std::string& target);

std::vector<int> stringSearchRec(const std::vector<std::string>& vocabulary, const std::string& target);

#endif //PARALLELPROGRAMMING_EDITDISTANCE_H
