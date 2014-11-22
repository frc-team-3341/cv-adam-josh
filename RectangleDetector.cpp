/*
 * RectangleDetector.cpp
 *
 *  Created on: Nov 8, 2014
 *      Author: josh
 */

#include "RectangleDetector.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "Rectangle.h"

double RectangleDetector::angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}



void RectangleDetector::rectDect()
{
	og = cv::imread("/home/josh/Downloads/rectangles.jpg");
	//cv::imshow("The OG Image", og);
	//====Thresholding==
	cv::Mat hsv_image;
	cv::cvtColor(og, hsv_image, CV_RGB2HSV);
	std::vector<cv::Mat> victor;
	cv::split(hsv_image, victor);
	//cv::imshow("HSV", hsv_image);
	cv::Mat hue = victor.at(0);
	//cv::imshow("Hue", hue);
	cv::Mat tLowHue;
	cv::Mat tHighHue;
	cv::threshold(hue, tLowHue, 0, 255, CV_THRESH_BINARY);
	cv::threshold(hue, tHighHue, 80, 255, CV_THRESH_BINARY_INV);
	cv::Mat threshHue = tLowHue & tHighHue;

	cv::Mat value = victor.at(2);
	//cv::imshow("Value", value);
	cv::Mat tLowValue;
	cv::Mat tHighValue;
	cv::threshold(value, tLowValue, 200, 255, CV_THRESH_BINARY);
	cv::threshold(value, tHighValue, 255, 255, CV_THRESH_BINARY_INV);
	cv::Mat threshValue = tLowValue & tHighValue;

	cv::Mat finalThresh = threshHue & threshValue;
	//cv::imshow("HueThresh", threshHue);
	//cv::imshow("ValuedThresh", threshValue);
	//cv::imshow("Threshold",finalThresh);
	cv::Mat gray0(finalThresh.size(), CV_8U);
	cv::Mat gray;
	std::vector<std::vector<cv::Point> > contours;
	int ch[] = {0, 0};
	cv::mixChannels(&finalThresh, 1, &gray0, 1, ch, 1);
	cv::Canny(gray0, gray, 0, 50, 5);
	cv::dilate(gray, gray, cv::Mat(), cv::Point(-1,-1));
	cv::findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	std::vector<cv::Point> approx;
	for (size_t i = 0; i < contours.size(); ++i) {
		// approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours.at(i)), approx, cv::arcLength(cv::Mat(contours.at(i)), true) * 0.02, true);
		// rectangular contours should have 4 vertices after approximation
		// relatively large area (to filter out noisy contours)
		// and be convex.
		// Note: absolute value of an area is used because
		// area may be positive or negative - in accordance with the
		// contour orientation
		if (approx.size() == 4 &&
				fabs(cv::contourArea(cv::Mat(approx))) > 1000 &&
				cv::isContourConvex(cv::Mat(approx))) {
			double maxCosine = 0;
			for(int j = 2; j < 5; ++j) {
				// find the maximum cosine of the angle between joint edges
				double cosine = fabs(angle(approx.at(j%4), approx.at(j-2), approx.at(j-1)));
				maxCosine = MAX(maxCosine, cosine);
			}
			// if cosines of all angles are small
			// (all angles are ~90 degrees) then write quandrange
			// vertices to resultant sequence
			if(maxCosine < 0.3)
				allRectangles.push_back(approx);
		}
	}
}

void RectangleDetector::listRects()
{
	for(unsigned i = 0; i < allRectangles.size(); ++i)
	{
		Rectangle temprect = Rectangle(allRectangles.at(i));
		rectList.push_back(temprect);
	}
}

void RectangleDetector::printRectangles()
{
	for(unsigned i = 0; i < rectList.size(); i++)
	{
		std::cout << "========== Rectangle" << i << " ==========" << std::endl;
		std::cout << "Top Left: " << rectList.at(i).getTopLeft() << std::endl;
		std::cout << "Top Right: " << rectList.at(i).getTopRight() << std::endl;
		std::cout << "Bottom Left: " << rectList.at(i).getBottomLeft() << std::endl;
		std::cout << "Bottom Right: " << rectList.at(i).getBottomRight() << std::endl;
		std::cout << "Center: " << rectList.at(i).getCenter() << std::endl;
		for (unsigned j = 0; j < rectList.at(i).getContainedRectangles().size(); j++)
		{
			std::cout << "Contained Rectangle: " << rectList.at(i).getContainedRectangles().at(j) << std::endl;
		}
	}
}

void RectangleDetector::drawRectangles()
{
		for (size_t i = 0; i < allRectangles.size(); i++)
		{
			const cv::Point* p = &allRectangles.at(i).at(0);
			int n = (int)allRectangles.at(i).size();
			cv::Scalar color;
			if (rectList.at(i).getContainedRectangles().size() > 0)
			{
				color = cv::Scalar(255,0,0);
				containerRectangles.push_back(rectList.at(i));
			}
			else
				color = cv::Scalar(0,255,0);
			cv::polylines(og, &p, &n, 1, true, color, 1, CV_AA);
		}
		cv::imshow("Final Rectangles", og);
		cv::waitKey(0);
}

bool RectangleDetector::rectangleIsContained(Rectangle rectContainer, Rectangle rectContained)
{
	bool ca = rectContainer.containsPoint(rectContained.getTopLeft());
	bool cb = rectContainer.containsPoint(rectContained.getTopRight());
	bool cc = rectContainer.containsPoint(rectContained.getBottomLeft());
	bool cd = rectContainer.containsPoint(rectContained.getBottomRight());
	/*
	std::cout << "Top Left: " << ca << std::endl;
	std::cout << "Top Right: " << cb << std::endl;
	std::cout << "Bottom Left: " << cc << std::endl;
	std::cout << "Bottom Right: " << cd << std::endl;
	*/
	return (ca && cb && cc && cd);
}

void RectangleDetector::findContainedRectangles()
{
	for (unsigned i = 0; i < rectList.size(); i++)
	{
		for (unsigned j = 0; j < rectList.size(); j++)
		{
			if (i == j) continue;
			if (rectangleIsContained(rectList.at(i), rectList.at(j)))
			{
				rectList.at(i).addContainedRectangle(j);
			}
		}
	}
}


