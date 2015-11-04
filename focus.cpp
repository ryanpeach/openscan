/**
 * focus.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */
#include <opencv2/opencv.hpp>
#include <vector>
#include <list>
#include <cmath>
#include <typeinfo>
#include "support.hpp"

using namespace std;
using namespace cv;

struct Fp {
	vector<cnt> contours;
	cnt contour;
	Point center;
	int depth, shape;

	Fp (vector<cnt> conts, double angleTol, double distTol) {
		contours = conts;
		center = centroid(contours);
		depth = findInnerBorder(contours,angleTol,distTol);
		contour = contours[depth];
		shape = contours[depth].size();
	}
	Fp (vector<cnt> conts) {Fp(conts,10.0,5.0);}
	Fp () {}

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

Point centroid(vector<Fp> fps) {
    vector<cnt> contours;
    for (Fp f : fps) {
        contours.push_back(f.contour);
    }
    return centroid(contours);
}

bool allInside(cnt poly, vector<Fp> fps) {
    for (Fp f : fps) {
        if (pointPolygonTest(poly, f.center, false) < 0) {
            return false;
        }
    }
    return true;
}

cnt hasRectangle(vector<Fp> fps, double angleTol, double distTol) {
    if (fps.size()<4) {return cnt();}
    //check all combinations of poly
    for (Fp a1 : fps) {
        for (Fp a2 : fps) {
            for (Fp a3 : fps) {
                for (Fp a4 : fps) {
                    if (a1 != a2 && a1 != a3 && a1 != a4 && a2 != a3 && a2 != a4 && a3 != a4) {
                        if (isRectangle((cnt){a1.center,a2.center,a3.center,a4.center},false,angleTol,distTol)) {
                            return (cnt){a1.center,a2.center,a3.center,a4.center};
    }}}}}}
    return cnt();
}

list<double> angs (Point x, list<Fp> fours) {
	vector<double> out;
	for (Fp y : fours) {for (Fp z : fours) {if (x != y.center && y != z && x != z.center) {out.push_back(angle(x,y.center,z.center));}}}
	return list<double>(out.begin(),out.end());
}
