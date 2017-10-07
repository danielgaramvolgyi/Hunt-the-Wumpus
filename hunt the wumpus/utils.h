#pragma once

#include <vector>
#include <iostream>

bool getRandomBool(double probability);
int getRandomNumber(int min, int max);

// returns a vector of the numbers 0..(size-1) in random order
std::vector<int> getRandomPermutation(int size);

// extracts numbers from a sequence of the form a-b-c-...-z, with a..z being numbers
std::vector<int> parseNumberSequence(const std::string& sequence);

// clears and flushes cin
void cleanUpCin();

// replace integers in a container according to the rule i -> shuffledNumbers[i]
// expects the elements to be in the range [0,shuffledNumbers.size())
template<typename Iterator>
void replaceNumbers(Iterator first, Iterator last, std::vector<int> shuffledNumbers) {
    for (auto it = first; it < last; ++it) {
        *it = shuffledNumbers[*it];
    }
}

// pretty prints numbers from a vector
template<typename Iterator>
void printNumbers(Iterator first, Iterator last) {
    if (first == last) return;
    for (auto it = first; it < last - 1; ++it) {
        if (it != first) std::cout << ", ";
        std::cout << *it;
	}
    if (first != last - 1) std::cout << " and ";
    std::cout << *(last - 1);
}