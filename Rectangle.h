/*
 * Rectangle.h
 *
 *  Created on: Nov 8, 2014
 *      Author: josh
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_
#include <opencv2/opencv.hpp>
#include <iostream>
class Rectangle {
public:
	Rectangle(std::vector<cv::Point>);
	bool containsPoint(cv::Point);

	void addContainedRectangle(int);


	const cv::Point& getBottomLeft() const {
		return bottomLeft;
	}

	const cv::Point& getBottomRight() const {
		return bottomRight;
	}

	const std::vector<int>& getContainedRectangles() const {
		return containedRectangles;
	}

	double getLengthBottom() const {
		return lengthBottom;
	}

	double getLengthLeft() const {
		return lengthLeft;
	}

	double getLengthRight() const {
		return lengthRight;
	}

	double getLengthTop() const {
		return lengthTop;
	}

	float getArea() const {
		return area;
	}

	const std::vector<cv::Point>& getRectPoints() const {
		return rectPoints;
	}

	const cv::Point& getTopLeft() const {
		return topLeft;
	}

	const cv::Point& getTopRight() const {
		return topRight;
	}

	const cv::Point& getCenter() const {
		return center;
	}

private:
	std::vector<cv::Point> rectPoints;
	cv::Point topLeft;
	cv::Point topRight;
	cv::Point bottomLeft;
	cv::Point bottomRight;
	cv::Point center;
	double lengthLeft;
	double lengthRight;
	double lengthTop;
	double lengthBottom;
	float area;
	std::vector<int> containedRectangles;
	void populate(std::vector<cv::Point> input);
	void findCenter();
	void findLengths();
};

#endif /* RECTANGLE_H_ */
