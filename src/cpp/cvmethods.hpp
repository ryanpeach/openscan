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
// Filters the img, finds the contours, and returns the Cnts.
Cnts findPolys(Mat * img, double distRatio);

// Find all the focus points within an image.
Fps findFocusPoints(Cnts polys, double angleTol, double distRatio);

// Classifies squares and selects the four most likely to be corners
// Null-Condition: Returns null
Fps calcCorners(Fps focusPoints, double angleTol, double distRatio);

// Sort edges by distance.
// Corners must be a rectangle
// Null-Condition: Returns corners
cnt sortCorners(cnt corners, Point ref, double distRatio = 0);
cnt sortCorners(cnt corners);

// Null-Condition: Returns fps[0]
Fp calcRef(Fps fps);

// Null-Condition: Returns contour[0]
Point calcRef(cnt contour);

// ------------ Image Manipulation --------------
Mat cropImage(Mat * img, int R);

// Reference: Modified from http://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
Mat fixPerspective(Mat * img, cnt border, Point ref);
Mat fixPerspective(Mat * img, Fps border, Fp ref);

bool isColor(Mat * img);

#endif
