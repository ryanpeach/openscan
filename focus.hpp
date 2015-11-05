/**
 * focus.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#ifndef FOCUS
#define FOCUS

#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <typeinfo>

#include "support.hpp"
#include "geometry.hpp"

using namespace std;
using namespace cv;

struct Fp {
	vector<cnt> contours;
	cnt contour;
	Point center;
	int depth, shape;

	Fp (vector<cnt> conts, cnt cont, Point cent, int d, int s) {
		contours = conts; contour = cont; center = cent; depth = d; shape = s;
	}
	Fp (vector<cnt> conts, double angleTol, double distTol) {
		contours = conts;
		center = centroid(contours);
		depth = findInnerBorder(contours,angleTol,distTol);
		contour = contours[depth];
		shape = contours[depth].size();
	}
	Fp (vector<cnt> conts) {Fp(conts,10.0,5.0);}

	bool operator== (Fp newFp) {
		bool test1 = contours == newFp.contours;
		bool test2 = contour == newFp.contour;
		bool test3 = center == newFp.center;
		bool test4 = depth == newFp.depth;
		bool test5 = shape == newFp.shape;
		return test1 && test2 && test3 && test4 && test5;
	}

	bool operator!= (Fp newFp) {
		return !(*this == newFp);
	}

	Fp copy () {
		return Fp(contours, contour, center, depth, shape);
	}

	private:
		//Checks shape of each contour from last to -5 and finds the first 'square.' Returns 0 if none exists.
		//Null-Condition: returns -1;
		int findInnerBorder(vector<cnt> cnts, double angleTol, double distTol) {
			cnt contour;
			for (int x = cnts.size(); x > 0; x++) {
				contour = cnts[x];
				if (isPoly(contour,4,true,angleTol,distTol)) {return x+1;}
			}
			return -1;
		}

};

Point centroid (vector<Fp> fps);
Point centroid(Fp fp) {return fp.center;}

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
