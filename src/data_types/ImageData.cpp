#include "ImageData.h"
#include "RandomGenerators.h"
#include <filesystem>
#include <stdexcept>
#include <iostream>

namespace fs = std::filesystem;

ImageData::ImageData(int numImages, int width, int height, int channels)
    : numImages(numImages), width(width), height(height), channels(channels), imageType(ImageType::RANDOM_NOISE) {
}

void ImageData::setImageType(ImageType type) {
	imageType = type;
}

void ImageData::generate() {
	images.clear();
	for (int i = 0; i < numImages; ++i) {
		cv::Mat image;
		switch (imageType) {
		case ImageType::RANDOM_NOISE:
			image = generateRandomNoise();
			break;
		case ImageType::GEOMETRIC_SHAPES:
			image = generateGeometricShapes();
			break;
		case ImageType::GRADIENT:
			image = generateGradientImage();
			break;
		case ImageType::PATTERN:
			image = generatePatternImage();
			break;
		}
		images.push_back(image);
	}
}

cv::Mat ImageData::generateRandomNoise() {
	cv::Mat image(height, width, channels==1 ? CV_8UC1 : CV_8UC3);

	//generate random noise
	cv::randu(image, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
	return image;
}

cv::Mat ImageData::generateGeometricShapes() {
	cv::Mat image = cv::Mat::zeros(height, width, channels == 1 ? CV_8UC1 : CV_8UC3);

	//generate a background color
	cv::Scalar bgColor;
	if (channels == 1) {
		bgColor = cv::Scalar(RandomGenerators::getRandomInt(0, 255));
	}
	else {
		bgColor = cv::Scalar(RandomGenerators::getRandomInt(0, 255),
			RandomGenerators::getRandomInt(0, 255),
			RandomGenerators::getRandomInt(0, 255));
	}
	image = bgColor;

	//Number of shapes to draw
	int numShapes = RandomGenerators::getRandomInt(1, 10);

	for (int i = 0;i < numShapes;++i) {
		//generate a random shape color
		cv::Scalar shapeColor;
		if (channels == 1) {
			shapeColor = cv::Scalar(RandomGenerators::getRandomInt(0, 255));
		}
		else {
			shapeColor = cv::Scalar(RandomGenerators::getRandomInt(0, 255),
				RandomGenerators::getRandomInt(0, 255),
				RandomGenerators::getRandomInt(0, 255));
		}

		//Generate a random shape type (0:circlle, 1:rectangle, 2:line)
		int shapeType = RandomGenerators::getRandomInt(0, 2);


		//generate random position and size
		int x = RandomGenerators::getRandomInt(0, width - 1);
		int y = RandomGenerators::getRandomInt(0, height - 1);
		int size = RandomGenerators::getRandomInt(10, std::min(width, height) / 4);

		//Draw the shape
		switch (shapeType) {
		case 0: //circle
			cv::circle(image, cv::Point(x, y), size, shapeColor, -1);
			break;
		case 1: //rectangle
			cv::rectangle(image, cv::Point(x, y), cv::Point(x + size, y + size), shapeColor, -1);
			break;
		case 3: //line
			cv::line(image, cv::Point(x, y), cv::Point(x + size, y + size), shapeColor, 2);
			break;
		}
	}
	return image;
}

cv::Mat ImageData::generateGradientImage() {
	cv::Mat image(height, width, channels == 1 ? CV_8UC1 : CV_8UC3);

	//determine gradient direction ( 0:horizontal, 1:vertical, 2:diagonal)
	int direction = RandomGenerators::getRandomInt(0, 2);

	//generate start and end coloors
	cv::Scalar startColor, endColor;
	if (channels == 1) {
		startColor = cv::Scalar(RandomGenerators::getRandomInt(0, 255));
		endColor = cv::Scalar(RandomGenerators::getRandomInt(0, 255));
	}
	else {
		startColor = cv::Scalar(RandomGenerators::getRandomInt(0, 255),
			RandomGenerators::getRandomInt(0, 255),
			RandomGenerators::getRandomInt(0, 255));
		endColor = cv::Scalar(RandomGenerators::getRandomInt(0, 255),
			RandomGenerators::getRandomInt(0, 255),
			RandomGenerators::getRandomInt(0, 255));
	}

	//generate gradient
	for (int y = 0;y < height;++y) {
		for (int x = 0;x < width;++x) {
			double ratio;

			switch (direction) {
			case 0: //horizontal
				ratio = static_cast<double>(x) / width;
				break;
			case 1: //vertical
				ratio = static_cast<double>(y) / height;
				break;
			case 2: //diagonal
				ratio = static_cast<double>(x + y) / (width + height);
				break;
			default:
				ratio = 0.5;
			}

			if (channels == 1) {
				image.at<uchar>(y, x) = static_cast<uchar>(
					startColor.val[0] * (1 - ratio) + endColor.val[0] * ratio);
			}
			else {
				image.at<cv::Vec3b>(y, x) = cv::Vec3b(
					static_cast<uchar>(startColor.val[0] * (1 - ratio) + endColor.val[0] * ratio),
					static_cast<uchar>(startColor.val[1] * (1 - ratio) + endColor.val[1] * ratio),
					static_cast<uchar>(startColor.val[2] * (1 - ratio) + endColor.val[2] * ratio)
				);
			}
		}
	}
	return image;
}

cv::Mat ImageData::generatePatternImage() {
	cv::Mat image(height, width, channels == 1 ? CV_8UC1 : CV_8UC3);

	//pattern type (0:checkboard, 1:stripes, 2:dots)
	int patternType = RandomGenerators::getRandomInt(0, 2);

	// Generate colors
	cv::Scalar color1, color2;
	if (channels == 1) {
		color1 = cv::Scalar(RandomGenerators::getRandomInt(0, 255));
		color2 = cv::Scalar(RandomGenerators::getRandomInt(0, 255));
	}
	else {
		color1 = cv::Scalar(
			RandomGenerators::getRandomInt(0, 255),
			RandomGenerators::getRandomInt(0, 255),
			RandomGenerators::getRandomInt(0, 255)
		);
		color2 = cv::Scalar(
			RandomGenerators::getRandomInt(0, 255),
			RandomGenerators::getRandomInt(0, 255),
			RandomGenerators::getRandomInt(0, 255)
		);
	}

	//pattern size
	int patternSize = RandomGenerators::getRandomInt(5, 20);

	//generate pattern
	switch (patternType) {
	case 0: // Checkerboard
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				bool isColor1 = ((x / patternSize) + (y / patternSize)) % 2 == 0;

				if (channels == 1) {
					image.at<uchar>(y, x) = isColor1 ? color1[0] : color2[0];
				}
				else {
					image.at<cv::Vec3b>(y, x) = isColor1 ?
						cv::Vec3b(color1[0], color1[1], color1[2]) :
						cv::Vec3b(color2[0], color2[1], color2[2]);
				}
			}
		}
		break;

	case 1: // Stripes
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				bool isColor1 = (x / patternSize) % 2 == 0;

				if (channels == 1) {
					image.at<uchar>(y, x) = isColor1 ? color1[0] : color2[0];
				}
				else {
					image.at<cv::Vec3b>(y, x) = isColor1 ?
						cv::Vec3b(color1[0], color1[1], color1[2]) :
						cv::Vec3b(color2[0], color2[1], color2[2]);
				}
			}
		}
		break;

	case 2: // Dots
		image = color1;

		for (int y = patternSize / 2; y < height; y += patternSize) {
			for (int x = patternSize / 2; x < width; x += patternSize) {
				cv::circle(image, cv::Point(x, y), patternSize / 3, color2, -1);
			}
		}
		break;
	}
	return image;
}

void ImageData::exportToDirectory(const std::string& directory) const {
	//create directory if it doesn't exist
	if (!fs::exists(directory)) {
		fs::create_directory(directory);
	}

	//export image
	for (size_t i = 0;i < images.size();++i) {
		std::string filename = directory + "/image" + std::to_string(i) + ".png";
		if (!cv::imwrite(filename, images[i])) {
			throw std::runtime_error("Failed to write image to file: " + filename);
		}
	}
}

std::vector<cv::Mat> ImageData::getImages() const {
	return images;
}