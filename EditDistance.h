//
// Created by Niccolò Arati on 08/01/2024.
//

#ifndef PARALLELPROGRAMMING_EDITDISTANCE_H
#define PARALLELPROGRAMMING_EDITDISTANCE_H

#include <string>
#include <vector>

int levenshteinDist(std::string word1, std::string word2);

std::vector<std::string> stringSearch(std::vector<std::string> vocabulary, const std::string& target, int threshold);

#endif //PARALLELPROGRAMMING_EDITDISTANCE_H
