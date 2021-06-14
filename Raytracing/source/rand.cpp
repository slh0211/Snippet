#include "rand.h"

std::random_device device;
std::mt19937_64 RNGen(device());
std::uniform_real_distribution<> myrandom(0.0, 1.0);

float getARandomFloat(void)
{
	return myrandom(RNGen);
}

int getARandomInt(int min, int max)
{
	thread_local std::mt19937 engine(std::random_device{}());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(engine);
}
