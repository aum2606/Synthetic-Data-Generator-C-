#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include <filesystem>
#include <vector>
#include <string>
#include <memory>

// We'll use our own simple image representation instead of OpenCV
struct RGBPixel {
    unsigned char r, g, b;
};

struct Image {
    int width;
    int height;
    int channels;
    std::vector<unsigned char> data;
    
    Image(int w, int h, int c) : width(w), height(h), channels(c) {
        data.resize(width * height * channels, 0);
    }
    
    unsigned char& at(int y, int x, int channel) {
        return data[(y * width + x) * channels + channel];
    }
    
    const unsigned char& at(int y, int x, int channel) const {
        return data[(y * width + x) * channels + channel];
    }
    
    RGBPixel getPixel(int y, int x) const {
        RGBPixel pixel;
        if (channels == 1) {
            pixel.r = pixel.g = pixel.b = at(y, x, 0);
        } else {
            pixel.r = at(y, x, 0);
            pixel.g = at(y, x, 1);
            pixel.b = at(y, x, 2);
        }
        return pixel;
    }
    
    void setPixel(int y, int x, const RGBPixel& pixel) {
        if (channels == 1) {
            at(y, x, 0) = (pixel.r + pixel.g + pixel.b) / 3;
        } else {
            at(y, x, 0) = pixel.r;
            at(y, x, 1) = pixel.g;
            at(y, x, 2) = pixel.b;
        }
    }
};

enum class ImageType {
    RANDOM_NOISE,
    GEOMETRIC_SHAPES,
    GRADIENT,
    PATTERN
};

class ImageData {
public:
    ImageData(int numImage, int width, int height, int channels);

    void setImageType(ImageType type);
    void generate();
    void exportToDirectory(const std::string& directory) const;
    
    std::vector<Image> getImages() const;
    
private:
    Image generateRandomNoise();
    Image generateGeometricShapes();
    Image generateGradientImage();
    Image generatePatternImage();
    
    std::vector<Image> images;
    int numImages;
    int width;
    int height;
    int channels;
    ImageType imageType;
};

#endif // !IMAGE_DATA_H
