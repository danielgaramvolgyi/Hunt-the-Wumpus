#pragma once

// generates random number in the given range (inclusive)
int getRandomNumber(int min, int max);

// pretty print numbers from a container
template<typename Iterator>
void printNumbers(Iterator first, Iterator last) {
	while (first + 2 < last) {
		std::cout << *first << ", ";
		++first;
	}
	if (first + 1 < last) {
		std::cout << *first << " and ";
		++first;
	}
	if (first < last) {
		std::cout << *first;
	}
}