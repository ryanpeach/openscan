/**
 * focus.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "focus.hpp"

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

vector<double> angs (Point x, vector<Fp> fours) {
	vector<double> out;
	for (Fp y : fours) {for (Fp z : fours) {if (x != y.center && y != z && x != z.center) {out.push_back(angle(x,y.center,z.center));}}}
	return out;
}
