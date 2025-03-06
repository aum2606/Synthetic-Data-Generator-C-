#include "TimeSeriesData.h"
#include <iostream>
#include <cassert>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;

void testTimeSeriesDataGeneration() {
    std::cout << "Testing TimeSeriesData generation..." << std::endl;

    // Test random walk time series generation
    TimeSeriesData ts1(100, 3);
    ts1.setPattern(TimeSeriesPattern::RANDOM_WALK);
    ts1.generate();
    auto data1 = ts1.getTimeSeries();

    assert(data1.size() == 100);
    assert(data1[0].values.size() == 3);

    // Test trend time series generation
    TimeSeriesData ts2(50, 2);
    ts2.setPattern(TimeSeriesPattern::TREND);
    ts2.setStartTime(std::time(nullptr) - 86400);  // Start 1 day ago
    ts2.setTimeStep(1800);  // 30 minutes
    ts2.generate();
    auto data2 = ts2.getTimeSeries();

    assert(data2.size() == 50);
    assert(data2[0].values.size() == 2);

    // Test seasonal time series generation
    TimeSeriesData ts3(200, 1);
    ts3.setPattern(TimeSeriesPattern::SEASONAL);
    ts3.generate();
    auto data3 = ts3.getTimeSeries();

    assert(data3.size() == 200);
    assert(data3[0].values.size() == 1);

    // Test export to CSV
    std::string csvFilename = "test_timeseries.csv";
    ts1.exportToCSV(csvFilename);
    assert(fs::exists(csvFilename));

    // Clean up test file
    fs::remove(csvFilename);

    std::cout << "TimeSeriesData tests passed!" << std::endl;
}

int main() {
    testTimeSeriesDataGeneration();
    return 0;
}