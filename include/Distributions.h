#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

#include <random>
#include <vector>

class Distributions {
public:
    // Normal (Gaussian) distribution
    static double getNormal(double mean, double stddev);

    // Uniform distribution
    static double getUniform(double min, double max);

    // Exponential distribution
    static double getExponential(double lambda);

    // Poisson distribution
    static int getPoisson(double mean);

    // Binomial distribution
    static int getBinomial(int trials, double probability);

    // Gamma distribution
    static double getGamma(double shape, double scale);

    // Beta distribution
    static double getBeta(double alpha, double beta);

    // Log-normal distribution
    static double getLogNormal(double mean, double stddev);

    // Weibull distribution
    static double getWeibull(double shape, double scale);

    // Generate samples from a mixture of distributions
    static std::vector<double> getMixture(const std::vector<double>& weights,
        const std::vector<std::pair<double, double>>& normalParams,
        int numSamples);

private:
    static std::mt19937& getGenerator();
};

#endif // DISTRIBUTIONS_H