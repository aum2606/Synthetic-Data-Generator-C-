#include "Distributions.h"
#include "RandomGenerators.h"
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <chrono>

double Distributions::getNormal(double mean, double stddev) {
    std::normal_distribution<double> distribution(mean, stddev);
    return distribution(RandomGenerators::getGenerator());
}

double Distributions::getUniform(double min, double max) {
    return RandomGenerators::getRandomDouble(min, max);
}

double Distributions::getExponential(double lambda) {
    if (lambda <= 0.0) {
        throw std::invalid_argument("Lambda must be greater than 0");
    }

    std::exponential_distribution<double> distribution(lambda);
    return distribution(RandomGenerators::getGenerator());
}

int Distributions::getPoisson(double mean) {
    if (mean <= 0.0) {
        throw std::invalid_argument("Mean must be greater than 0");
    }

    std::poisson_distribution<int> distribution(mean);
    return distribution(RandomGenerators::getGenerator());
}

int Distributions::getBinomial(int trials, double probability) {
    if (trials <= 0) {
        throw std::invalid_argument("Number of trials must be greater than 0");
    }

    if (probability < 0.0 || probability > 1.0) {
        throw std::invalid_argument("Probability must be between 0.0 and 1.0");
    }

    std::binomial_distribution<int> distribution(trials, probability);
    return distribution(RandomGenerators::getGenerator());
}

double Distributions::getGamma(double shape, double scale) {
    if (shape <= 0.0 || scale <= 0.0) {
        throw std::invalid_argument("Shape and scale must be greater than 0");
    }

    std::gamma_distribution<double> distribution(shape, scale);
    return distribution(RandomGenerators::getGenerator());
}

double Distributions::getBeta(double alpha, double beta) {
    if (alpha <= 0.0 || beta <= 0.0) {
        throw std::invalid_argument("Alpha and beta must be greater than 0");
    }

    // Generate beta distribution using gamma distributions
    double gamma1 = getGamma(alpha, 1.0);
    double gamma2 = getGamma(beta, 1.0);

    return gamma1 / (gamma1 + gamma2);
}

double Distributions::getLogNormal(double mean, double stddev) {
    std::lognormal_distribution<double> distribution(mean, stddev);
    return distribution(RandomGenerators::getGenerator());
}

double Distributions::getWeibull(double shape, double scale) {
    if (shape <= 0.0 || scale <= 0.0) {
        throw std::invalid_argument("Shape and scale must be greater than 0");
    }

    std::weibull_distribution<double> distribution(shape, scale);
    return distribution(RandomGenerators::getGenerator());
}

std::vector<double> Distributions::getMixture(const std::vector<double>& weights,
    const std::vector<std::pair<double, double>>& normalParams,
    int numSamples) {
    if (weights.size() != normalParams.size()) {
        throw std::invalid_argument("Number of weights must match number of distribution parameters");
    }

    if (numSamples <= 0) {
        throw std::invalid_argument("Number of samples must be greater than 0");
    }

    // Normalize weights to sum to 1
    double weightSum = std::accumulate(weights.begin(), weights.end(), 0.0);
    std::vector<double> normalizedWeights(weights.size());

    for (size_t i = 0; i < weights.size(); ++i) {
        normalizedWeights[i] = weights[i] / weightSum;
    }

    // Generate samples
    std::vector<double> samples(numSamples);

    for (int i = 0; i < numSamples; ++i) {
        // Select a distribution based on weights
        double r = RandomGenerators::getRandomDouble(0.0, 1.0);
        double cumulativeWeight = 0.0;
        size_t selectedDist = 0;

        for (size_t j = 0; j < normalizedWeights.size(); ++j) {
            cumulativeWeight += normalizedWeights[j];
            if (r <= cumulativeWeight) {
                selectedDist = j;
                break;
            }
        }

        // Generate a sample from the selected distribution
        double mean = normalParams[selectedDist].first;
        double stddev = normalParams[selectedDist].second;
        samples[i] = getNormal(mean, stddev);
    }

    return samples;
}

std::mt19937& Distributions::getGenerator() {
    static bool initialized = false;
    static std::mt19937 generator;

    if (!initialized) {
        unsigned int seed = static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        generator.seed(seed);
        initialized = true;
    }

    return generator;
}