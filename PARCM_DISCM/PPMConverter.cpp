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

	this->imgChannels[0] = cv::Mat(this->image_height, this->image_width, CV_8U, cv::Scalar(0));
	this->imgChannels[1] = cv::Mat(this->image_height, this->image_width, CV_8U, cv::Scalar(0));
	this->imgChannels[2] = cv::Mat(this->image_height, this->image_width, CV_8U, cv::Scalar(0));
}

void PPMConverter::setPixels(int x, int y, double r, double g, double b, int samplePerPixel)
{
	float scale = 1.0 / samplePerPixel;
	
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	uchar r_converted = static_cast<uchar>(256 * std::clamp(r, 0.0, 0.999));
	uchar g_converted = static_cast<uchar>(256 * std::clamp(g, 0.0, 0.999));
	uchar b_converted = static_cast<uchar>(256 * std::clamp(b, 0.0, 0.999));

	imgChannels[0].at<uchar>(y, x) = r_converted;
	imgChannels[1].at<uchar>(y, x) = g_converted;
	imgChannels[2].at<uchar>(y, x) = b_converted;
}


void PPMConverter::draw() {
	cv::Mat rgb;
	cv::Mat channels[] = { imgChannels[2], imgChannels[1], imgChannels[0] };
	cv::merge(channels, 3, rgb);
	cv::imwrite("MyImage.png", rgb);
}