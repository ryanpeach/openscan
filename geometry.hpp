/**
 * geometry.hpp
 *
 * @date Oct 31, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#ifndef GEOMETRY
#define GEOMETRY

#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <typeinfo>

#include "support.hpp"

using namespace std;
using namespace cv;

#define PI 3.14159265

/**
 * Returns the distance between point a and b.
 * @param  Point a, b.
 * @return The distance between a and b in eclidean space.
 * @complexity O(1)
 */
double dist(Point a, Point b);

/**
 * Returns the unit angle of Point a using origin as the center.
 * @param  Point origin, a.
 * @return The unit angle in degrees of origin<a using origin as the center.
 * @complexity O(1)
 */
double angle(Point origin, Point a);

/**
 * Returns the angle of line c2-origin-c3.
 * @param  Point origin, c2, c3.
 * @return The angle by line-angle-line of line c2-origin-c3 in degrees.
 * @complexity O(1)
 */
double angle(Point origin, Point c2, Point c3);

/**
 * Returns the angles line-angle-line between all neighboring triplets in poly.
 * @param  cnt poly: Any contour.
 * @return Returns vector of the same length as cnt in the same order where out[n] = angle(poly[n],poly[n-1],poly[n+1])
 * @complexity O(?)
 */
vector<double> angles(cnt poly);

/**
 * Returns a vector of distances between each pair in the polygon.
 * @param  cnt poly: Any contour.
 * @return Returns vector of the same length as cnt in the same order where out[n] = dist(poly[n],poly[n+1])
 * @complexity O(?)
 */
vector<double> dists(cnt poly);

/**
 * Returns the center of a contour, or of a bunch of contours, or of a bunch of points.
 * @param  A cnt, vector of cnt's, vector of Fp's, or a Cnts object.
 * @return The mean of all points contained in the param returned as a Point.
 * @complexity O(?)
 */
Point centroid(Point p) {return p;}
template <typename U> Point centroid(vector<U> c);
Point centroid (cnt);
Point centroid (vector<cnt>);
Point centroid (Cnts contours) {return centroid(contours.contours);}

/**
 * Test that all Points inside poly are the same distance from their neighbors as all others.
 * @param  cnt poly:    Any cnt you wish to test.
 * @param  int distTol: The distance tolerance for calculation of equality.
 * @return True / False
 * @complexity O(?)
 */
bool allSameLength(cnt poly, int distTol);

/**
 * Tests whether or not a contour fits certain definitions of "polygon," "rectangle," or "square."
 * @param  cnt poly, the size of the poly (4 if rect or square)
 * @param  bool regular: whether or not all sides are the same length.
 * @param  double angleTol: the angle tolerance of the angle equality.
 * @param  double distTol: the distance tolerance of the length equality.
 * @return The angle by line-angle-line of line c2-origin-c3 in degrees.
 * @issues isPoly should have consistent angle-testing if regular among all sizes of polys.
 * @complexity O(?)
 */
bool isPoly(cnt poly, int size, bool regular, double angleTol, double distTol);
bool isRectangle(cnt poly, bool square, double angleTol, double distTol);
bool isSquare(cnt poly, double angleTol, double distTol);

#endif
