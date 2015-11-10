/**
 * cvmethods.hpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#ifndef CVMETHODS
#define CVMETHODS

#include <cmath>

#include "focus.hpp"

// -------------- Feature Detection ----------------
// Checks shape of each contour from last to -5 and finds the first 'square.'
// Returns 0 if none exists.
int findInnerBorder(vector<cnt> cnts, double tol);
int findInnerBorder(Cnts cnts, double tol);

// Filters the img, finds the contours, and returns the Cnts.
Cnts findPolys(Mat img, double tol);

// Find all the focus points within an image.
vector<Fp> findFocusPoints(Cnts polys);

// Classifies squares and selects the four most likely to be corners
// Null-Condition: Returns null
vector<Fp> getCorners(vector<Fp> focusPoints, double angleTol);

// Sort edges by distance.
// Corners must be a rectangle
// Null-Condition: Returns corners
vector<Fp> sortCorners(vector<Fp> corners);

// Null-Condition: Returns fps[0]
Fp getRef(vector<Fp> fps);

// Null-Condition: Returns contour[0]
Point getRef(cnt contour);

// ------------ Image Manipulation --------------
// wSize must be an odd number, will be rounded up.
Mat importFilter(Mat img, int tol1, int tol2, int wSize);

Mat outputFilter(Mat img, int wSize, int C);

Mat cropImage(Mat img, int R);

// Reference: Modified from http://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
Mat fixPerspective(Mat img, vector<cnt> border, Point ref);

bool isColor(Mat img);

#endif
