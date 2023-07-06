#pragma once
#include <random>

namespace kiko
{
	void seedRandom(unsigned int seed) { srand(seed); }
	int random() { return rand(); }
	int random(unsigned int max) { return rand() % max; } // 0 - (max - 1)
	int random(unsigned int min, unsigned int max) { return min + random((max - min) + 1); } // min - max

	float randomf() { return random() / (float)RAND_MAX; }
	float randomf(float max) { return randomf() * max; }
	float randomf(float min, float max) { return min + randomf() * (max - min); }
}