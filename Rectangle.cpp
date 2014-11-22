/*
 * Rectangle.cpp
 *
 *  Created on: Nov 8, 2014
 *      Author: josh
 */

#include "Rectangle.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
Rectangle::Rectangle(std::vector<cv::Point> input)
{
	populate(input);
	findCenter();
	findLengths();
}

void Rectangle::populate(std::vector<cv::Point> input)
{
	double minLength = 6e6;
	double maxLength = 0;
	int indexTopLeft = -1;
	int indexTopRight = -1;
	int indexBottomLeft = -1;
	int indexBottomRight = -1;
	for (unsigned i = 0; i < input.size(); ++i)
	{
		double tempLength = sqrt(pow(input.at(i).x, 2) + pow(input.at(i).y, 2));
		if (tempLength < minLength){
			minLength = tempLength;
			indexTopLeft = i;
		}
		if (tempLength > maxLength){
			maxLength = tempLength;
			indexBottomRight = i;
		}
	}

	for(unsigned i = 0; i < input.size(); ++i)
	{
		if (i == indexTopLeft || i == indexBottomRight)
			continue;
		if (indexTopRight < 0)
			indexTopRight = i;
		else if (indexBottomLeft < 0)
			indexBottomLeft = i;
	}

	if(input.at(indexTopRight).x < input.at(indexBottomLeft).x){
		cv::Point swap = input.at(indexTopRight);
		input.at(indexTopRight) = input.at(indexBottomLeft);
		input.at(indexBottomLeft) = swap;
	}
	topLeft = input.at(indexTopLeft);
	topRight = input.at(indexTopRight);
	bottomLeft = input.at(indexBottomLeft);
	bottomRight = input.at(indexBottomRight);
	rectPoints = input;
	area = fabs(cv::contourArea(cv::Mat(rectPoints)));
}

void Rectangle::findCenter()
{
	center = topLeft + topRight + bottomLeft + bottomRight;
	center.x /= 4;
	center.y /= 4;
}

void Rectangle::findLengths()
{
	lengthLeft = sqrt(pow(topLeft.x - bottomLeft.x,2) + pow(topLeft.y - bottomLeft.y,2));
	lengthRight = sqrt(pow(topRight.x - bottomRight.x,2) + pow(topRight.y - bottomRight.y,2));
	lengthTop = sqrt(pow(topLeft.x - topRight.x,2) + pow(topLeft.y - topRight.y,2));
	lengthBottom = sqrt(pow(bottomLeft.x - bottomRight.x,2) + pow(bottomLeft.y - bottomRight.y,2));
}

bool Rectangle::containsPoint(cv::Point otherPoint)
{
	return (otherPoint.x > topLeft.x && otherPoint.x < topRight.x && otherPoint.y > topLeft.y && otherPoint.y < bottomRight.y);
}

void Rectangle::addContainedRectangle(int rect)
{
	containedRectangles.push_back(rect);
}
