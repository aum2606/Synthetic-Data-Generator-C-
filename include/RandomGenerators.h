#ifndef RANDOM_GENERATORS_H
#define RANDOM_GENERATORS_H

#include <random>
#include <string>
#include <vector>

class RandomGenerators {
public:
    // Initialize the random number generators
    static void initialize(unsigned int seed = 0);

    // Integer random number generation
    static int getRandomInt(int min, int max);

    // Float random number generation
    static float getRandomFloat(float min, float max);

    // Double random number generation
    static double getRandomDouble(double min, double max);

    // Boolean random generation
    static bool getRandomBool(double trueProbability = 0.5);

    // String random generation
    static std::string getRandomString(int length, bool includeUppercase = true,
        bool includeNumbers = true, bool includeSpecial = false);

    // Select a random element from a vector
    template<typename T>
    static T getRandomElement(const std::vector<T>& elements) {
        if (elements.empty()) {
            throw std::runtime_error("Cannot select a random element from an empty vector");
        }
        return elements[getRandomInt(0, elements.size() - 1)];
    }

    static std::mt19937& getGenerator();

private:
    //static std::mt19937& getGenerator();
    static std::mt19937 generator;
    static bool isInitialized;
};

#endif // RANDOM_GENERATORS_H