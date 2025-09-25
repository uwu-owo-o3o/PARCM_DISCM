#pragma once
#include "opencv2/opencv.hpp"
#include "vector"

class PPMConverter
{
	public:
		PPMConverter();
		~PPMConverter();
	public:
		void initialize(int image_height, int image_width);
		void setPixels(int x, int y, double r, double g, double b, int samplePerPixel);
		void draw();
	public:
		float image_height;
		float image_width;
		std::unique_ptr<cv::Mat> pixels;
		std::vector<cv::Mat> imgChannels;
};

