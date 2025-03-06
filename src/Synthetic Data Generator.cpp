// Synthetic Data Generator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string>
#include "TabularData.h"
#include "ImageData.h"
#include "TextData.h"
#include "TimeSeriesData.h"
#include "AudioData.h"

void printUsage() {
    std::cout << "Synthetic Data Generator\n";
    std::cout << "Usage: synthetic_data_generator [data_type] [num_samples] [output_path]\n";
    std::cout << "  data_type: tabular, image, text, timeseries, audio\n";
    std::cout << "  num_samples: Number of samples to generate\n";
    std::cout << "  output_path: Path to save the generated data\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printUsage();
        return 1;
    }

    std::string dataType = argv[1];
    int numSamples = std::stoi(argv[2]);
    std::string outputPath = argv[3];

    try {
        if (dataType == "tabular") {
            TabularData tabular(numSamples, 5);  // 5 columns by default
            tabular.generate();
            tabular.exportToCSV(outputPath);
            std::cout << "Generated " << numSamples << " samples of tabular data to " << outputPath << std::endl;
        }
        else if (dataType == "image") {
            ImageData images(numSamples, 64, 64, 3);  // 64x64 RGB images by default
            images.generate();
            images.exportToDirectory(outputPath);
            std::cout << "Generated " << numSamples << " synthetic images to " << outputPath << std::endl;
        }
        else if (dataType == "text") {
            TextData text(numSamples, 100);  // 100 words per sample by default
            text.generate();
            text.exportToFile(outputPath);
            std::cout << "Generated " << numSamples << " text samples to " << outputPath << std::endl;
        }
        else if (dataType == "timeseries") {
            TimeSeriesData timeSeries(numSamples, 5);  // 5 dimensions by default
            timeSeries.generate();
            timeSeries.exportToCSV(outputPath);
            std::cout << "Generated time-series data with " << numSamples << " time points to " << outputPath << std::endl;
        }
        else if (dataType == "audio") {
            AudioData audio(numSamples, 44100, 5);  // 5 second clips at 44.1kHz by default
            audio.generate();
            audio.exportToDirectory(outputPath);
            std::cout << "Generated " << numSamples << " audio samples to " << outputPath << std::endl;
        }
        else {
            std::cerr << "Unknown data type: " << dataType << std::endl;
            printUsage();
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
