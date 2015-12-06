/**
 * geometry.hpp
 *
 * @date Oct 31, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#ifndef GEOMETRY
#define GEOMETRY

#include "opencv2/imgproc/imgproc.hpp"
#include "support.hpp"

#define PI 3.14159265

/**
 * Returns the distance between point a and b.
 * @param  Point a, b.
 * @return The distance between a and b in eclidean space.
 * @complexity O(1)
 * @tested works
 */
double dist(Point a, Point b);

/**
 * Returns the unit angle of Point a using origin as the center.
 * @param  Point origin, a.
 * @return The unit angle in degrees of origin<a using origin as the center.
 * @complexity O(1)
 * @tested untested
 */
double angle(Point origin, Point a);

/**
 * Returns the angle of line c2-origin-c3.
 * @param  Point origin, c2, c3.
 * @return The angle by line-angle-line of line c2-origin-c3 in degrees.
 * @complexity O(1)
 * @tested works
 */
double angle(Point origin, Point c2, Point c3);

/*
 * Finds if any of the three points can form the given angle.
 * @param Any three points.
 * @return the existing contour with the angle in the middle
 * @return empty contour if none exists
 * @complexity WARNING: O(1), O(N^3)
 */
cnt anyAng(Point a, Point b, Point c, double v, double angTol);

/**
 * Returns the angles line-angle-line between all neighboring triplets in poly.
 * @param  cnt poly: Any contour.
 * @return Returns vector of the same length as cnt in the same order where out[n] = angle(poly[n],poly[n-1],poly[n+1])
 * @complexity O(?)
 * @tested works
 */
vector<double> angles(cnt poly);

/**
 * Returns a vector of distances between each pair in the polygon.
 * @param  cnt poly: Any contour.
 * @return Returns vector of the same length as cnt in the same order where out[n] = dist(poly[n],poly[n+1])
 * @complexity O(?)
 * @tested works
 */
vector<double> dists(cnt poly);

/**
 * Returns the center of a contour, or of a bunch of contours, or of a bunch of points.
 * @param  A cnt, vector of cnt's, vector of Fp's, or a Cnts object.
 * @return The mean of all points contained in the param returned as a Point.
 * @complexity O(?)
 * @tested works
 */
Point centroid(vector<Point> c);
Point centroid(vector<cnt> vec);
Point centroid(Cnts c);


/**
 * Test that all Points inside poly are the same distance from their neighbors as all others.
 * @param  cnt poly:       Any cnt you wish to test.
 * @param  double distRatio: The distance tolerance for calculation of equality.
 * @return True / False
 * @complexity O(?)
 * @tested works
 */
bool allSameLength(cnt poly, double distRatio);

/**
 * Test that all Points inside poly have the same angle to their neighbors as all others.
 * @param  cnt poly:        Any cnt you wish to test.
 * @param  double angleTol: The distance tolerance for calculation of equality.
 * @return True / False
 * @complexity O(?)
 * @tested untested
 */
bool regularAngles(cnt poly, double angleTol);

/**
 * Test that border is the given aspect ratio. Assumes input is rectangular.
 * @param  cnt border:      Any rectangular cnt you wish to test.
 * @param  double ratioTol: The ratio tolerance for calculation of equality.
 * @return True / False
 * @complexity O(?)
 * @tested untested
 */
bool isAspectRatio(cnt border, double aspectRatio, double ratioTol);

/**
 * Tests whether or not a contour fits certain definitions of "polygon," "rectangle," or "square."
 * @param  cnt poly, the size of the poly (4 if rect or square)
 * @param  bool regular: whether or not all sides are the same length.
 * @param  double angleTol: the angle tolerance of the angle equality.
 * @param  double distRatio: the distance tolerance of the length equality.
 * @return The angle by line-angle-line of line c2-origin-c3 in degrees.
 * @issues isPoly should have consistent angle-testing if regular among all sizes of polys.
 * @complexity O(?)
 * @tested works
 */
bool isPoly(cnt poly, int size, bool regularA, bool regularL, double angleTol, double distRatio);
bool isRectangle(cnt poly, bool square, double angleTol, double distRatio);
bool isSquare(cnt poly, double angleTol, double distRatio);

/**
 * Iterates through all points given to see if some set of 4 points
 *     can be ordered together to create a rectangular polygon.
 * @param  cnt poly: A list of unordered, potentially connected points which
 *                       might contain a rectangle.
 * @param  double angleTol: the angle tolerance of the angle equality.
 * @param  double distRatio: the distance tolerance of the length equality.
 * @param  int n: the max number of desired returns.
 * @return Returns a found rectangle as a cnt.
 * @return Returns an empty contour if none was found or if n < 1.
 * @tested untested
 * @complexity O(n*len(poly)), O(n*len(poly)^4)
 */
vector<cnt> hasRectangles(cnt poly, double angleTol, double distRatio, int n = 1);
vector<cnt> hasRectangles(vector<cnt> poly, double angleTol, double distRatio);
cnt hasRectangle(cnt poly, double angleTol, double distRatio);

/**
 * Returns all angles between x and each Point in "fours."
 * @param  Point x:  Any point you wish to use as an origin for angle calc
 * @param  cnt poly: Any vector of points you wish to test.
 *                       Does not need to be ordered or assumed connected.
 * @return True / False
 * @complexity O(?)
 * @tested untested
 */
vector<double> angs(Point x, cnt fours);

/**
 * Returns the largest contour in a vector.
 * @param  vector<cnt>:  Any vector of cnt's.
 * @param  min: allows you to set a minimum size.
 * @return cnt: largest contour
 * @complexity O(?)
 * @tested untested
 */
cnt largest(vector<cnt> v, double min = -1);

/**
 * Returns contours which are very similar, like the double edges of a line.
 * @param ref: the cnt to compare to.
 * @param check:  Any vector of cnt's to check.
 * @param r1: Only used by findSimilar, the starting index for the loop
 * @return distRatio: the distance between corners to qualify as "equal".
 * @complexity O(N)
 * @complexity O(N^2)
 * @tested untested
 */
cnt findSimilar(cnt ref, vector<cnt> check, double distRatio, int r1 = 0);
vector<vector<cnt>> findSimilar(vector<cnt> check, double distRatio);


#endif
