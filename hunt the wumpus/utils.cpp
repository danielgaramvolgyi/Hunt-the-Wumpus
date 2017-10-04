#include "utils.h"

#include <random>
#include <numeric>
#include <algorithm>

bool getRandomBool(double probability) {
    bool randomBool = false;
    if (getRandomNumber(0, 99) < 100 * probability) randomBool = true;
    return randomBool;
}

int getRandomNumber(int min, int max) {
	static std::random_device seeder;
	static std::mt19937 engine(seeder());
	std::uniform_int_distribution<int> dist(min, max);

	return dist(engine);
}

std::vector<int> getRandomPermutation(int size) {
    std::vector<int> permutation(size);
    std::iota(permutation.begin(), permutation.end(), 0);
    static std::random_device seeder;
    static std::mt19937 rng(seeder());
    std::shuffle(std::begin(permutation), std::end(permutation), rng);
    return permutation;
}