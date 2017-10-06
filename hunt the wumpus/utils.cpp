#include "utils.h"

#include <random>
#include <numeric>
#include <algorithm>

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