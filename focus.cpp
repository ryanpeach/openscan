/**
 * focus.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "focus.hpp"

int Fp::findInnerBorder(vector<cnt> cnts, double angleTol, double distTol) {
	cnt contour;
	for (int x = cnts.size(); x > 0; x++) {
		contour = cnts[x];
		if (isPoly(contour,4,true,angleTol,distTol)) {return x+1;}
	}
	return -1;
}

Fp::Fp (vector<cnt> conts, double angleTol, double distTol) {
	contours = conts;
	center = centroid(contours);
	depth = Fp::findInnerBorder(contours,angleTol,distTol);
	contour = contours[depth];
	shape = contours[depth].size();
}

bool Fp::operator== (Fp newFp) {
	bool test1 = contours == newFp.contours;
	bool test2 = contour == newFp.contour;
	bool test3 = center == newFp.center;
	bool test4 = depth == newFp.depth;
	bool test5 = shape == newFp.shape;
	return test1 && test2 && test3 && test4 && test5;
}

string tostr(Fp fp) {return tostr(fp.center);}

Point centroid (vector<Fp> fps) {
	cnt out;
	for (Fp f : fps) {
		out.push_back(f.center);
	}
	return centroid(out);
}

Point centroid (Fp f) {
    return f.center;
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

vector<double> angs (Point x, vector<Fp> fours) {
	vector<double> out;
	for (Fp y : fours) {for (Fp z : fours) {if (x != y.center && y != z && x != z.center) {out.push_back(angle(x,y.center,z.center));}}}
	return out;
}
