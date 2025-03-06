# Synthetic Data Generator

A comprehensive C++ library for generating high-quality synthetic data across multiple domains.

## Overview

The Synthetic Data Generator is a powerful, modular C++ library designed to create realistic synthetic data for various applications including machine learning, software testing, simulation, and data privacy. This project provides a flexible framework for generating tabular, image, text, time-series, and audio data with customizable parameters and export options.

## Features

### Data Types
- **Tabular Data**: Generate structured data with various column types (integer, float, categorical, date, boolean)
- **Image Data**: Create synthetic images with patterns, shapes, gradients, and noise
- **Text Data**: Generate text using lorem ipsum, random words, Markov chains, or template-based approaches
- **Time Series Data**: Produce time series with random walks, trends, seasonal patterns, and cyclical behaviors
- **Audio Data**: Generate synthetic audio including sine waves, white noise, pink noise, and frequency sweeps

### Utilities
- **Random Generators**: Comprehensive random number generation with various distributions
- **Statistical Distributions**: Support for normal, uniform, exponential, Poisson, gamma, beta, and other distributions
- **File Export**: Export data to various formats including CSV, JSON, XML, and SQLite

## Project Structure

```
synthetic_data_generator/
├── src/                          # Source code for the data generator
│   ├── main.cpp                  # Main entry point for the program
│   ├── data_types/               # Different data generation modules
│   │   ├── TabularData.cpp       # Generate synthetic tabular data
│   │   ├── ImageData.cpp         # Generate synthetic image data
│   │   ├── TextData.cpp          # Generate synthetic text data
│   │   ├── TimeSeriesData.cpp    # Generate synthetic time-series data
│   │   └── AudioData.cpp         # Generate synthetic audio data
│   ├── utils/                    # Utility functions
│   │   ├── RandomGenerators.cpp  # Functions for generating random numbers
│   │   ├── Distributions.cpp     # Distribution helpers (uniform, normal, etc.)
│   │   └── FileExport.cpp        # Functions for exporting data (CSV, JSON, etc.)
├── include/                      # Header files for the data generation components
├── config/                       # Configuration files for parameters
├── tests/                        # Unit tests for various modules
├── CMakeLists.txt                # CMake build configuration file
├── requirements.txt              # Dependencies
└── Makefile                      # Alternative build system
```

## Requirements

- C++17 compatible compiler
- CMake 3.10 or higher
- Boost library (for advanced random number generation)
- OpenCV (for image data generation)
- FFmpeg (optional, for audio data generation)

## Installation

### Using CMake

```bash
# Clone the repository
git clone https://github.com/yourusername/synthetic-data-generator.git
cd synthetic-data-generator

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make

# Run tests (optional)
make test

# Install (optional)
make install
```

### Using Makefile

```bash
# Clone the repository
git clone https://github.com/yourusername/synthetic-data-generator.git
cd synthetic-data-generator

# Build the project
make

# Run tests (optional)
make test

# Install (optional)
make install
```

## Usage Examples

### Generating Tabular Data

```cpp
#include "TabularData.h"

int main() {
    // Create tabular data with 100 rows and 5 columns
    TabularData tabular(100, 5);
    
    // Generate the data
    tabular.generate();
    
    // Export to CSV
    tabular.exportToCSV("output/tabular_data.csv");
    
    // Export to JSON
    tabular.exportToJSON("output/tabular_data.json");
    
    return 0;
}
```

### Generating Image Data

```cpp
#include "ImageData.h"

int main() {
    // Create 50 RGB images of size 64x64
    ImageData images(50, 64, 64, 3);
    
    // Set image type to geometric shapes
    images.setImageType(ImageType::GEOMETRIC_SHAPES);
    
    // Generate the images
    images.generate();
    
    // Export to directory
    images.exportToDirectory("output/images");
    
    return 0;
}
```

### Generating Text Data

```cpp
#include "TextData.h"

int main() {
    // Create 20 text samples with 100 words each
    TextData text(20, 100);
    
    // Set text type to Markov chain
    text.setTextType(TextType::MARKOV_CHAIN);
    
    // Generate the text
    text.generate();
    
    // Export to file
    text.exportToFile("output/synthetic_text.txt");
    
    return 0;
}
```

### Generating Time Series Data

```cpp
#include "TimeSeriesData.h"

int main() {
    // Create time series with 1000 points and 3 dimensions
    TimeSeriesData timeSeries(1000, 3);
    
    // Set pattern to seasonal
    timeSeries.setPattern(TimeSeriesPattern::SEASONAL);
    
    // Generate the time series
    timeSeries.generate();
    
    // Export to CSV
    timeSeries.exportToCSV("output/time_series_data.csv");
    
    return 0;
}
```

### Generating Audio Data

```cpp
#include "AudioData.h"

int main() {
    // Create 10 audio samples at 44.1kHz, 5 seconds each
    AudioData audio(10, 44100, 5);
    
    // Set audio type to sine wave
    audio.setAudioType(AudioType::SINE_WAVE);
    
    // Generate the audio
    audio.generate();
    
    // Export to directory
    audio.exportToDirectory("output/audio");
    
    return 0;
}
```

### Command Line Interface

The project includes a command-line interface for generating data without writing code:

```bash
# Generate tabular data
./synthetic_data_generator tabular 100 output/tabular_data.csv

# Generate image data
./synthetic_data_generator image 50 output/images

# Generate text data
./synthetic_data_generator text 20 output/synthetic_text.txt

# Generate time series data
./synthetic_data_generator timeseries 1000 output/time_series_data.csv

# Generate audio data
./synthetic_data_generator audio 10 output/audio
```

## Configuration

The project uses a configuration system to customize data generation parameters. You can modify these parameters in the `config/config.h` file or provide them at runtime.

## Extending the Project

The modular design of this project makes it easy to extend with new data types or features:

1. Create a new header file in the `include/` directory
2. Implement the corresponding source file in the `src/data_types/` directory
3. Add tests in the `tests/` directory
4. Update the main program to include your new data type

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- The project uses [Boost](https://www.boost.org/) for advanced random number generation
- Image generation is powered by [OpenCV](https://opencv.org/)
- Audio processing uses components from [FFmpeg](https://ffmpeg.org/)
