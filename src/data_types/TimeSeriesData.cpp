#include "TimeSeriesData.h"
#include "RandomGenerators.h"
#include "Distributions.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <iomanip>


constexpr double M_PI = 3.14159265358979323846;


TimeSeriesData::TimeSeriesData(int numPoints, int dimensions)
	: numPoints(numPoints), dimensions(dimensions), pattern(TimeSeriesPattern::RANDOM_WALK) {
	//deafult to current time
	startTime = std::time(nullptr);
	timeStepSeconds = 3600; //1 hr default
}

void TimeSeriesData::setPattern(TimeSeriesPattern pattern) {
	this->pattern = pattern;
}

void TimeSeriesData::setStartTime(std::time_t startTime) {
	this->startTime = startTime;
}

void TimeSeriesData::setTimeStep(int timeStepSeconds) {
	this->timeStepSeconds = timeStepSeconds;
}

void TimeSeriesData::generate() {
	timeSeries.clear();
	timeSeries.resize(numPoints);

	for (int i = 0;i < numPoints;++i) {
		TimePoint point;
		point.timestamp = startTime + i * timeStepSeconds;
		point.values.resize(dimensions);

		for (int d = 0;d < dimensions;++d) {
			std::vector<double> dimensionValues;

			switch (pattern) {
			case TimeSeriesPattern::RANDOM_WALK:
				dimensionValues = generateRandomWalk(d);
				break;
			case TimeSeriesPattern::TREND:
				dimensionValues = generateTrend(d);
				break;
			case TimeSeriesPattern::SEASONAL:
				dimensionValues = generateSeasonal(d);
				break;
			case TimeSeriesPattern::CYCLICAL:
				dimensionValues = generateCyclical(d);
				break;
			case TimeSeriesPattern::COMBINED:
				dimensionValues = generateCombined(d);
				break;
			}
			point.values[d] = dimensionValues[i];
		}
		timeSeries[i] = point;
	}
}

std::vector<double> TimeSeriesData::generateRandomWalk(int dimension) {
	std::vector<double> values(numPoints);

	// Start with a random value
	values[0] = RandomGenerators::getRandomDouble(-10.0, 10.0);

	// Generate random walk
	for (int i = 1; i < numPoints; ++i) {
		double step = RandomGenerators::getRandomDouble(-1.0, 1.0);
		values[i] = values[i - 1] + step;
	}

	return values;
}

std::vector<double> TimeSeriesData::generateTrend(int dimension) {
	std::vector<double> values(numPoints);

	//generate trend parameters
	double slope = RandomGenerators::getRandomDouble(-0.5, 0.5);
	double intercept = RandomGenerators::getRandomDouble(-10.0, 10.0);

	//generate trend with noise
	for (int i = 0; i < numPoints; ++i) {
		double noise = RandomGenerators::getRandomDouble(-1.0, 1.0);
		values[i] = intercept + slope * i + noise;
	}

	return values;
}

std::vector<double> TimeSeriesData::generateSeasonal(int dimension) {
	std::vector<double> values(numPoints);

	//generate seasonal parameters
	double amplitude = RandomGenerators::getRandomDouble(1.0, 5.0);
	double frequency = RandomGenerators::getRandomDouble(0.01, 0.1);
	double phase = RandomGenerators::getRandomDouble(0.0, 2 * M_PI);

	//generate seasonal pattern
	for(int i=0;i<numPoints;++i){
		double noise = RandomGenerators::getRandomDouble(-0.5, 0.5);
		values[i] = amplitude * std::sin(frequency * i + phase) + noise;
	}
	return values;

}

std::vector<double> TimeSeriesData::generateCyclical(int dimension) {
	std::vector<double> values(numPoints);

	// Generate cyclical parameters
	double amplitude = RandomGenerators::getRandomDouble(1.0, 5.0);
	double frequency = RandomGenerators::getRandomDouble(0.005, 0.02);
	double phase = RandomGenerators::getRandomDouble(0.0, 2.0 * M_PI);

	// Generate cyclical pattern with noise
	for (int i = 0; i < numPoints; ++i) {
		double noise = RandomGenerators::getRandomDouble(-0.5, 0.5);
		values[i] = amplitude * std::sin(frequency * i + phase) + noise;

		// Add some non-linear behavior
		if (i % 100 < 50) {
			values[i] += 2.0;
		}
	}

	return values;
}

std::vector<double> TimeSeriesData::generateCombined(int dimension) {
	std::vector<double> randomWalk = generateRandomWalk(dimension);
	std::vector<double> trend = generateTrend(dimension);
	std::vector<double> seasonal = generateSeasonal(dimension);

	std::vector<double> combined(numPoints);

	for (int i = 0; i < numPoints; ++i) {
		combined[i] = 0.3 * randomWalk[i] + 0.3 * trend[i] + 0.4 * seasonal[i];
	}

	return combined;
}

void TimeSeriesData::exportToCSV(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file for writing: " + filename);
	}

	// Write header
	file << "timestamp";
	for (int d = 0; d < dimensions; ++d) {
		file << ",dimension_" << (d + 1);
	}
	file << "\n";

	// Write data
	for (const auto& point : timeSeries) {
		// Convert timestamp to string
		std::tm tm;
		localtime_s(&tm, &point.timestamp);
		char buffer[20];
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);

		file << buffer;

		for (const auto& value : point.values) {
			file << "," << value;
		}

		file << "\n";
	}

	file.close();
}

std::vector<TimePoint> TimeSeriesData::getTimeSeries() const {
	return timeSeries;
}