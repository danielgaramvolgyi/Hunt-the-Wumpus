#include "utils.h"

#include <random>
#include <numeric>
#include <algorithm>
#include <sstream> 

bool getRandomBool(double probability) {
    bool randomBool = false;
    if (getRandomNumber(0, 99) < 100 * probability) randomBool = true;
    return randomBool;
}

auto& getRng() {
    static std::random_device seeder;
    static std::mt19937 rng(seeder());
    return rng;
}

int getRandomNumber(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(getRng());
}

std::vector<int> getRandomPermutation(int size) {
    // generate the vector {0,1,...,size-1}
    std::vector<int> permutation(size);
    std::iota(permutation.begin(), permutation.end(), 0);

    std::shuffle(std::begin(permutation), std::end(permutation), getRng());
    return permutation;
}

std::vector<int> parseNumberSequence(const std::string& sequence)
{
    std::vector<int> numbers{};
    int currentNumber;
    char ch;
    std::istringstream seq{ sequence };
    while (seq >> currentNumber >> ch) {
        numbers.push_back(currentNumber);
    }
    numbers.push_back(currentNumber);
    return numbers;
}

void cleanUpCin() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}