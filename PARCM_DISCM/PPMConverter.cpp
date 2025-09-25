#include "PPMConverter.h"
#include <algorithm>

PPMConverter::PPMConverter()
{
	this->imgChannels = std::vector<cv::Mat>();
	this->imgChannels.resize(3);
}

PPMConverter::~PPMConverter()
{
}

void PPMConverter::initialize(int image_width, int image_height) {
	this->image_width = image_width;
	this->image_height = image_height;

	this->imgChannels[0] = cv::Mat(this->image_height, this->image_width, CV_64F, cv::Scalar(0));
	this->imgChannels[1] = cv::Mat(this->image_height, this->image_width, CV_64F, cv::Scalar(0));
	this->imgChannels[2] = cv::Mat(this->image_height, this->image_width, CV_64F, cv::Scalar(0));
}

void PPMConverter::setPixels(int x, int y, double r, double g, double b, int samplePerPixel)
{
	float scale = 1.0 / samplePerPixel;
	
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);
	
	imgChannels[0].at<double>(y, x) = r;
	imgChannels[1].at<double>(y, x) = g;
	imgChannels[2].at<double>(y, x) = b;
}

void PPMConverter::draw() {
	cv::Mat r, g, b;
	imgChannels[0].convertTo(r, CV_8U, 256.0, 0);
	imgChannels[1].convertTo(g, CV_8U, 256.0, 0);
	imgChannels[2].convertTo(b, CV_8U, 256.0, 0);

	cv::Mat rgb, channels[] = { b, g, r };
	cv::merge(channels, 3, rgb);
	cv::imwrite("MyImage.png", rgb);
}