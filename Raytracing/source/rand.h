#pragma once
// A good quality *thread-safe* Mersenne Twister random number generator.
#include <random>
//extern std::random_device device;
//extern std::mt19937_64 RNGen;
//extern std::uniform_real_distribution<> myrandom;
// Call myrandom(RNGen) to get a uniformly distributed random number in [0,1].

float getARandomFloat(void);
int getARandomInt(int min, int max);