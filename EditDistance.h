//
// Created by Niccolò Arati on 08/01/2024.
//

#ifndef PARALLELPROGRAMMING_EDITDISTANCE_H
#define PARALLELPROGRAMMING_EDITDISTANCE_H

#include <string>
#include <vector>

int levenshteinDistFM(const std::string& word1, const std::string& word2, int length1, int length2);

int levenshteinDistSD(const std::string& word1, const std::string& word2, int length1, int length2);

int levenshteinDistMR(const std::string& word1, const std::string& word2, int length1, int length2);


#endif //PARALLELPROGRAMMING_EDITDISTANCE_H
