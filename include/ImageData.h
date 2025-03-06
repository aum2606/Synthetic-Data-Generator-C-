#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include <filesystem>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

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

	std::vector<cv::Mat>getImages() const;

private:
	int numImages;
	int width;
	int height;
	int channels;
	ImageType imageType;
	std::vector<cv::Mat>images;
	cv::Mat generateRandomNoise();
	cv::Mat generateGeometricShapes();
	cv::Mat generateGradientImage();
	cv::Mat generatePatternImage();
};

#endif // !IMAGE_DATA_H

