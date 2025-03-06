#ifndef  CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

//configuration parameters for synthetic data generation
namespace Config {
	//general configuration parameters
	const int DEFAULT_NUM_SAMPLES = 100;
	const std::string DEFAULT_OUTPUT_DIRECTORY = "../output";

	//tabular data configuration parameters
	const int DEFAULT_TABULAR_COLUMNS = 5;
	const int DEFAULT_TABULAR_ROWS = 100;

	//Image data configuration
	const int DEFAULT_IMAGE_WIDTH = 64;
	const int DEFAULT_IMAGE_HEIGHT = 64;
	const int DEFAULT_IMAGE_CHANNELS = 3;

	//text data configuration
	const int DEFAULT_TEXT_SAMPLES = 50;
	const int DEFAULT_WORDS_PER_SAMPLE = 100;

	//Time series data configuration
	const int DEFAULT_TIME_SERIES_POINTS = 100;
	const int DEFAULT_TIME_SERIES_DIMENSIONS = 3;
	const int DEFAULT_TIME_STEP_SECONDS = 3600; // 1 hour

	// Audio data condiguration
	const int DEFAULT_AUDIO_SAMPLES_RATE = 44100;  // 44.1 kHz
	const int DEFAULT_AUDIO_DURATION_SECONDS = 5; // 5 seconds
	const int DEFAULT_AUDIO_CHANNELS = 1;

	//random seed for reproducibility
	const unsigned int DEFAULT_RANDOM_SEED = 42;

	//load configuration from file
	std::unordered_map<std::string, std::string> loadConfig(const std::string& filename);

	//save configuration to file
	void saveConfigToFile(const std::string& filename,
		const std::unordered_map<std::string, std::string>& config);

}

#endif // ! CONFIG_H
