//============================================================================
// Name        : blurTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "RectangleDetector.h"

int main()
{
	RectangleDetector rDec = RectangleDetector();
	rDec.rectDect();
	rDec.listRects();
	rDec.findContainedRectangles();
	rDec.printRectangles();
	rDec.drawRectangles();
	return 0;
}
