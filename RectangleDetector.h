/*
 * RectangleDetector.h
 *
 *  Created on: Nov 8, 2014
 *      Author: josh
 */

#ifndef RECTANGLEDETECTOR_H_
#define RECTANGLEDETECTOR_H_
#include <opencv2/opencv.hpp>
#include <vector>
#include "Rectangle.h"
class RectangleDetector {
public:
	double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	void rectDect();
	void listRects();
	void printRectangles();
	void drawRectangles();
	void findContainedRectangles();
private:
	bool rectangleIsContained(Rectangle, Rectangle);
	std::vector<Rectangle> rectList;
	std::vector<std::vector<cv::Point> > allRectangles;
	std::vector<Rectangle> containerRectangles;
	cv::Mat og;
};

#endif /* RECTANGLEDETECTOR_H_ */
