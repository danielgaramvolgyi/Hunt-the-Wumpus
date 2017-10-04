#include "utils.h"

#include<random>

int getRandomNumber(int min, int max) {
	static std::random_device seeder;
	static std::mt19937 engine(seeder());
	std::uniform_int_distribution<int> dist(min, max);

	return dist(engine);
}