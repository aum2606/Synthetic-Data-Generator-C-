#include "RandomGenerators.h"
#include <chrono>
#include <stdexcept>


//static member initialization
std::mt19937 RandomGenerators::generator;
bool RandomGenerators::isInitialized = false;

void RandomGenerators::initialize(unsigned int seed)
{
	if (seed = 0) {
		//use current time as seed if none provided
		seed = static_cast<unsigned int> (std::chrono::high_resolution_clock::now().time_since_epoch().count());
	}
	generator.seed(seed);
	isInitialized = true;
}

int RandomGenerators::getRandomInt(int min, int max) {
	if (!isInitialized) {
		throw std::invalid_argument("Min value must be less than or equal to max value");
	}
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}


float RandomGenerators::getRandomFloat(float min, float max) {
	if (!isInitialized) {
		initialize();
	}
	if (min > max) {
		throw std::invalid_argument("Min value must be less than or equal to max value");
	}

	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(generator);
}

double RandomGenerators::getRandomDouble(double min, double max) {
    if (!isInitialized) {
        initialize();
    }

    if (min > max) {
        throw std::invalid_argument("Min value must be less than or equal to max value");
    }

    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

bool RandomGenerators::getRandomBool(double trueProbability) {
    if (!isInitialized) {
        initialize();
    }

    if (trueProbability < 0.0 || trueProbability > 1.0) {
        throw std::invalid_argument("Probability must be between 0.0 and 1.0");
    }

    std::bernoulli_distribution distribution(trueProbability);
    return distribution(generator);
}


std::string RandomGenerators::getRandomString(int length, bool includeUppercase,
    bool includeNumbers, bool includeSpecial) {
    if (!isInitialized) {
        initialize();
    }

    if (length <= 0) {
        throw std::invalid_argument("Length must be greater than 0");
    }

    std::string charset = "abcdefghijklmnopqrstuvwxyz";
    if (includeUppercase) {
        charset += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    if (includeNumbers) {
        charset += "0123456789";
    }
    if (includeSpecial) {
        charset += "!@#$%^&*()-_=+[]{}|;:,.<>?";
    }

    std::uniform_int_distribution<size_t> distribution(0, charset.size() - 1);

    std::string result;
    result.reserve(length);

    for (int i = 0; i < length; ++i) {
        result += charset[distribution(generator)];
    }

    return result;
}

std::mt19937& RandomGenerators::getGenerator() {
    if (!isInitialized) {
        initialize();
    }
    return generator;
}