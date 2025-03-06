#ifndef  TIME_SERIES_DATA_H
#define TIME_SERIES_DATA_H

#include <vector>
#include<string>
#include <ctime>

enum class TimeSeriesPattern {
	RANDOM_WALK,
	TREND,
	SEASONAL,
	CYCLICAL,
	COMBINED
};

struct TimePoint {
	std::time_t timestamp;
	std::vector<double> values;
};

class TimeSeriesData {
public:
	TimeSeriesData(int numPoints, int dimensions);
	void setPattern(TimeSeriesPattern pattern);
	void setStartTime(std::time_t startTime);
	void setTimeStep(int timeStepSeconds);
	void generate();
	void exportToCSV(const std::string& filename) const;

	std::vector<TimePoint> getTimeSeries() const;

private:
	int numPoints;
	int dimensions;
	TimeSeriesPattern pattern;
	std::time_t startTime;
	int timeStepSeconds;
	std::vector<TimePoint> timeSeries;
	std::vector<double> generateRandomWalk(int dimension);
	std::vector<double> generateTrend(int dimension);
	std::vector<double> generateSeasonal(int dimension);
	std::vector<double> generateCyclical(int dimension);
	std::vector<double> generateCombined(int dimension);
};


#endif // ! TIME_SERIES_DATA_H
