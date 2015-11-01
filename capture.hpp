/*
 * capture.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: ryanp
 */

#include <opencv2/opencv.hpp>
#include <vector>
#include <geometry.hpp>
#include <capture.hpp>

using namespace std;
using namespace cv;

typedef vector<Point> cnt;

struct Cnts {
	vector<cnt> contours;
    vector<Vec4i> heirarchy;
};

struct Fp {
	vector<cnt> contours;
	cnt contour;
	Point center;
	int depth, shape;

	Fp (vector<cnt> conts);
};


// -------------- Feature Detection ----------------
//Checks shape of each contour from last to -5 and finds the first 'square.' Returns 0 if none exists.
int findInnerBorder(vector<cnt> cnts);
int findInnerBorder(Cnts cnts);

//Filters the img, finds the contours, and returns the Cnts.
Cnts findPolys (Mat img);
Cnts findPolys (Mat img, int tol);

//Find all the focus points within an image.
vector<Fp> findFocusPoints (Cnts polys);

//Classifies squares and selects the four most likely to be corners
//Null-Condition: Returns null
vector<Fp> getCorners(vector<Fp> focusPoints);
vector<Fp> getCorners(vector<Fp> focusPoints, int tol);

//Sort edges by distance.
//Corners must be a rectangle
//Null-Condition: Returns corners
vector<Fp> sortCorners(vector<Fp> corners);

//Null-Condition: Returns fps[0]
Fp getRef(vector<Fp> fps);

//Null-Condition: Returns contour[0]
Point getRef(cnt contour);

// ------------ Image Manipulation --------------
Mat importFilter(Mat img);
Mat importFilter(Mat img, int tol1, int tol2, int wSize);

Mat outputFilter(Mat img);
Mat outputFilter(Mat img, int wSize, int C);

Mat cropImage(Mat img, int R);

//Reference: Modified from http://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
Mat fixPerspective (Mat img, vector<cnt> border, Point ref);
Mat fixPerspective (Mat img, vector<cnt> border, Point ref, double aspectRatio);

bool isColor(Mat img);
