#include "ImageData.h"
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <filesystem>
#include "RandomGenerators.h"

ImageData::ImageData(int numImages, int width, int height, int channels)
    : numImages(numImages), width(width), height(height), channels(channels), imageType(ImageType::RANDOM_NOISE) {
}

void ImageData::setImageType(ImageType type) {
    imageType = type;
}

void ImageData::generate() {
    images.clear();
    
    for (int i = 0; i < numImages; i++) {
        Image img(width, height, channels);
        
        switch (imageType) {
            case ImageType::RANDOM_NOISE:
                img = generateRandomNoise();
                break;
            case ImageType::GEOMETRIC_SHAPES:
                img = generateGeometricShapes();
                break;
            case ImageType::GRADIENT:
                img = generateGradientImage();
                break;
            case ImageType::PATTERN:
                img = generatePatternImage();
                break;
        }
        
        images.push_back(img);
    }
    
    std::cout << "Generated " << numImages << " synthetic images" << std::endl;
}

Image ImageData::generateRandomNoise() {
    Image img(width, height, channels);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGBPixel pixel;
            pixel.r = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
            pixel.g = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
            pixel.b = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
            img.setPixel(y, x, pixel);
        }
    }
    
    return img;
}

Image ImageData::generateGeometricShapes() {
    Image img(width, height, channels);
    
    // Fill with white background
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGBPixel pixel = {255, 255, 255};
            img.setPixel(y, x, pixel);
        }
    }
    
    // Draw random shapes
    int numShapes = RandomGenerators::getRandomInt(1, 5);
    
    for (int i = 0; i < numShapes; i++) {
        // Random shape type (0: rectangle, 1: circle)
        int shapeType = RandomGenerators::getRandomInt(0, 1);
        
        // Random color
        RGBPixel color;
        color.r = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
        color.g = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
        color.b = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
        
        if (shapeType == 0) {
            // Rectangle
            int x1 = RandomGenerators::getRandomInt(0, width - 1);
            int y1 = RandomGenerators::getRandomInt(0, height - 1);
            int x2 = RandomGenerators::getRandomInt(x1, width - 1);
            int y2 = RandomGenerators::getRandomInt(y1, height - 1);
            
            for (int y = y1; y <= y2; y++) {
                for (int x = x1; x <= x2; x++) {
                    img.setPixel(y, x, color);
                }
            }
        } else {
            // Circle
            int centerX = RandomGenerators::getRandomInt(0, width - 1);
            int centerY = RandomGenerators::getRandomInt(0, height - 1);
            int radius = RandomGenerators::getRandomInt(5, std::min(width, height) / 4);
            
            for (int y = std::max(0, centerY - radius); y < std::min(height, centerY + radius); y++) {
                for (int x = std::max(0, centerX - radius); x < std::min(width, centerX + radius); x++) {
                    int dx = x - centerX;
                    int dy = y - centerY;
                    if (dx*dx + dy*dy <= radius*radius) {
                        img.setPixel(y, x, color);
                    }
                }
            }
        }
    }
    
    return img;
}

Image ImageData::generateGradientImage() {
    Image img(width, height, channels);
    
    // Choose gradient direction (0: horizontal, 1: vertical, 2: diagonal)
    int direction = RandomGenerators::getRandomInt(0, 2);
    
    // Choose start and end colors
    RGBPixel startColor, endColor;
    startColor.r = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
    startColor.g = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
    startColor.b = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
    
    endColor.r = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
    endColor.g = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
    endColor.b = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float factor = 0.0f;
            
            switch (direction) {
                case 0: // Horizontal
                    factor = static_cast<float>(x) / width;
                    break;
                case 1: // Vertical
                    factor = static_cast<float>(y) / height;
                    break;
                case 2: // Diagonal
                    factor = static_cast<float>(x + y) / (width + height);
                    break;
            }
            
            RGBPixel pixel;
            pixel.r = static_cast<unsigned char>(startColor.r * (1 - factor) + endColor.r * factor);
            pixel.g = static_cast<unsigned char>(startColor.g * (1 - factor) + endColor.g * factor);
            pixel.b = static_cast<unsigned char>(startColor.b * (1 - factor) + endColor.b * factor);
            
            img.setPixel(y, x, pixel);
        }
    }
    
    return img;
}

Image ImageData::generatePatternImage() {
    Image img(width, height, channels);
    
    // Choose pattern type (0: checkerboard, 1: stripes)
    int patternType = RandomGenerators::getRandomInt(0, 1);
    
    // Choose colors
    RGBPixel color1, color2;
    color1.r = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
    color1.g = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
    color1.b = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
    
    color2.r = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
    color2.g = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
    color2.b = static_cast<unsigned char>(RandomGenerators::getRandomInt(0, 255));
    
    // Choose pattern size
    int patternSize = RandomGenerators::getRandomInt(5, 30);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            bool useColor1 = false;
            
            if (patternType == 0) {
                // Checkerboard
                int cellX = x / patternSize;
                int cellY = y / patternSize;
                useColor1 = (cellX + cellY) % 2 == 0;
            } else {
                // Stripes
                int cell = x / patternSize;
                useColor1 = cell % 2 == 0;
            }
            
            img.setPixel(y, x, useColor1 ? color1 : color2);
        }
    }
    
    return img;
}

void ImageData::exportToDirectory(const std::string& directory) const {
    std::filesystem::create_directories(directory);
    
    for (size_t i = 0; i < images.size(); i++) {
        // Create a simple PPM (Portable Pixmap) file
        std::string filename = directory + "/image_" + std::to_string(i + 1) + ".ppm";
        std::ofstream file(filename, std::ios::binary);
        
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            continue;
        }
        
        const Image& img = images[i];
        
        // PPM header
        file << "P6\n" << img.width << " " << img.height << "\n255\n";
        
        // Write pixel data
        for (int y = 0; y < img.height; y++) {
            for (int x = 0; x < img.width; x++) {
                RGBPixel pixel = img.getPixel(y, x);
                file.write(reinterpret_cast<char*>(&pixel.r), 1);
                file.write(reinterpret_cast<char*>(&pixel.g), 1);
                file.write(reinterpret_cast<char*>(&pixel.b), 1);
            }
        }
        
        file.close();
    }
    
    std::cout << "Exported " << images.size() << " images to " << directory << std::endl;
}

std::vector<Image> ImageData::getImages() const {
    return images;
}