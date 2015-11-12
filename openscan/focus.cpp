/**
 * focus.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "focus.hpp"
#define TEST

int Fp::findInnerBorder(vector<cnt> cnts, double angleTol, double distTol) {
#ifdef TEST
    cout << "findinnerborder start" << endl;
#endif

    cnt c;
    for (int x = cnts.size()-1; x > 0; x--) {
        c = cnts[x];
        if (isPoly(c,4,true,angleTol,distTol)) {return x;}
    }
    cout << "findinnerborder complete" << endl;

    return -1;
}

Fp::Fp(vector<cnt> conts, cnt cont, Point cent, int d, int s)
    : contours(conts), contour(cont), center(cent), depth(d), shape(s)
{}

Fp::Fp (vector<cnt> conts, double angleTol, double distTol) : contours(conts)
{
	center = centroid(contours);
	depth = Fp::findInnerBorder(contours,angleTol,distTol);
        if (depth!=-1) {
            contour = contours[depth];
            shape = contour.size();
        }
        else {
            contour = cnt();
            shape = 0;
        }
}

Fp::Fp (vector<cnt> conts) {Fp(conts,10.0,5.0);}

bool Fp::operator!= (Fp newFp) {
    return !(*this == newFp);
}

Fp Fp::copy () {
    return Fp(contours, contour, center, depth, shape);
}

bool Fp::isValid() {return shape > 0;}

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

cnt centroids(vector<Fp> fps) {
    cnt out;
    for (Fp fp : fps) {
        out.push_back(centroid(fp));
    }
    return out;
}

bool allInside(cnt poly, vector<Fp> fps) {
    for (Fp f : fps) {
        if (pointPolygonTest(poly, f.center, false) < 0) {
            return false;
        }
    }
    return true;
}

vector<Fp> hasRectangle(vector<Fp> fps, double angleTol, double distTol) {
    if (fps.size()<4) {return vector<Fp>();}
    //check all combinations of poly
    for (Fp a1 : fps) {
    for (Fp a2 : fps) {
    for (Fp a3 : fps) {
    for (Fp a4 : fps) {
        if (a1 != a2 && a1 != a3 && a1 != a4 && a2 != a3 && a2 != a4 && a3 != a4) {
            if (isRectangle(cnt{a1.center,a2.center,a3.center,a4.center}, false,angleTol,distTol)) {
                    return vector<Fp>{a1,a2,a3,a4};
    }}}}}}
    return vector<Fp>();
}

vector<double> angs (Point x, vector<Fp> fours) {
	vector<double> out;
	for (Fp y : fours) {for (Fp z : fours) {if (x != y.center && y != z && x != z.center) {out.push_back(angle(x,y.center,z.center));}}}
	return out;
}
