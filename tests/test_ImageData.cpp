#include "ImageData.h"
#include <iostream>
#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;

void testImageDataGeneration() {
    std::cout << "Testing ImageData generation..." << std::endl;

    // Test random noise image generation
    ImageData images1(5, 32, 32, 3);
    images1.setImageType(ImageType::RANDOM_NOISE);
    images1.generate();
    auto imageData1 = images1.getImages();

    assert(imageData1.size() == 5);
    assert(imageData1[0].rows == 32);
    assert(imageData1[0].cols == 32);
    assert(imageData1[0].channels() == 3);

    // Test geometric shapes image generation
    ImageData images2(3, 64, 64, 1);
    images2.setImageType(ImageType::GEOMETRIC_SHAPES);
    images2.generate();
    auto imageData2 = images2.getImages();

    assert(imageData2.size() == 3);
    assert(imageData2[0].rows == 64);
    assert(imageData2[0].cols == 64);
    assert(imageData2[0].channels() == 1);

    // Test export to directory
    std::string outputDir = "test_images";
    images1.exportToDirectory(outputDir);
    assert(fs::exists(outputDir));
    assert(fs::is_directory(outputDir));

    // Count files in the output directory
    int fileCount = 0;
    for (const auto& entry : fs::directory_iterator(outputDir)) {
        if (fs::is_regular_file(entry)) {
            fileCount++;
        }
    }
    assert(fileCount == 5);

    // Clean up test directory
    fs::remove_all(outputDir);

    std::cout << "ImageData tests passed!" << std::endl;
}

int main() {
    testImageDataGeneration();
    return 0;
}