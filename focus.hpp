/**
 * focus.hpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#ifndef FOCUS
#define FOCUS

#include <typeinfo>

#include "geometry.hpp"

struct Fp {
	vector<cnt> contours;
	cnt contour;
	Point center;
	int depth, shape;

	int findInnerBorder(vector<cnt> cnts, double angleTol, double distTol);

	Fp (vector<cnt> conts, cnt cont, Point cent, int d, int s) {
		contours = conts; contour = cont; center = cent; depth = d; shape = s;
	}
	Fp (vector<cnt> conts, double angleTol, double distTol);
	Fp (vector<cnt> conts) {Fp(conts,10.0,5.0);}

	bool operator== (Fp newFp);

	bool operator!= (Fp newFp) {
		return !(*this == newFp);
	}

	Fp copy () {
		return Fp(contours, contour, center, depth, shape);
	}

};

Point centroid (vector<Fp> fps);
Point centroid (Fp f) {return f.center;}

string tostr(Fp fp);

/**
 * Test that the center of all focus points are inside the poly.
 * @param  cnt poly:       Any cnt you wish to test as the boundary.
 * @param  vector<Fp> fps: A list of Fp's to test locations of.
 * @return True / False
 * @complexity O(?)
 */
bool allInside(cnt poly, vector<Fp> fps);

/**
 * Iterates through all Fp's given to see if some set of 4 Fp's can be ordered together to create a rectangular polygon.
 * @param  vector<Fp> fps: a list of Fp's of any size >= 4.
 * @param  double angleTol: the angle tolerance of the angle equality.
 * @param  double distTol: the distance tolerance of the length equality.
 * @return Returns a found rectangle as a cnt.
 * @return Returns an empty contour if none was found.
 * @issues isPoly should have consistent angle-testing if regular among all sizes of polys.
 * @complexity O(?)
 */
cnt hasRectangle(vector<Fp> fps, double angleTol, double distTol);

vector<double> angs (Point x, vector<Fp> fours);

#endif
