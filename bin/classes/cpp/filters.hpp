#ifndef FILTERS
#define FILTERS

#include "cvmethods.hpp"

/**
 * @brief Returns the grayscale conversion of an image.
 * @param img: any image.
 * @return grayscale version of image.
 * @return same image if already grayscale.
 * @complexity O(1)
 * @tested works
 */
Mat toGray(Mat img);
Mat toColor(Mat img);
void Gray(Mat *img);
void Color(Mat *img);

/**
 * @brief Returns the edges of an image by Canny method.
 * @param img: any image.
 * @param tol1: the lower tolerance for what counts as an edge (between 0 and 255).
 * @param tol2: the upper tolerance for what counts as an edge (between 0 and 255).
 * @param wSize: the size of the window to convolve across the image. Must be odd, will convert in code.
 * @param bSize: the size of the window to convolve for the gaussian blur.
 * @param bSigma: the sigma value for gausian blur.
 * @prarm l2grad: the l2grad for the Canny detector.
 * @return edge detect version of image.
 * @complexity O(1)
 * @tested works
 */
Mat edgesCanny(Mat* img, int tol1, int tol2, int wSize, int bSize, double bSigma, bool l2grad);

/**
 * @brief Returns the adaptive gaussian of an image, ideal for black and white pencil drawings.
 * @param img: any image.
 * @param wSize: the size of the window to convolve across the image. Must be odd, will convert in code.
 * @param C: The "C" variable described in OpenCV documentation for "adaptiveThreshold" method.
 * @return mean of vector
 * @complexity O(N)
 * @tested works
 */
Mat adaptiveGaussian(Mat *img, int wSize = 3, int C = 1);

#endif
