/**
 * focus.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "focus.hpp"
//#define TEST

int Fp::findInnerBorder(vector<cnt> cnts, double angleTol, double distRatio) {
#ifdef TEST
    cout << "Running Fp::findInnerBorder" << endl;
#endif
    for (int x = cnts.size()-1; x > 0; x--) {
        if (isSquare(cnts[x], angleTol, distRatio)) {return x;}
    }

    return -1;
}

Fp::Fp(vector<cnt> conts, cnt cont, Point cent, int d, int s)
    : contours(conts), contour(cont), center(cent), depth(d), shape(s)
{}

Fp::Fp() {
    center = Point(0,0);
    depth = 0;
    shape = 0;
}

Fp::Fp(vector<cnt> conts, double angleTol, double distRatio) : contours(conts)
{
#ifdef TEST
    cout << "Initializing Fp..." << endl;
#endif
    center = centroid(contours);
    depth = Fp::findInnerBorder(contours,angleTol,distRatio);
    if (depth!=-1) {
        contour = contours[depth];
        shape = contour.size();
    }
    else {
        contour = cnt();
        shape = 0;
    }
}

Fp::Fp(vector<cnt> conts) {Fp(conts, 10.0, 5.0);}

Fp Fp::copy() {
    return Fp(contours, contour, center, depth, shape);
}

bool Fp::isValid() {return shape > 0;}

bool Fp::operator==(Fp newFp) {
#ifdef TEST
    cout << "Running Fp::operator==..." << endl;
#endif
    bool test1 = (contours == newFp.contours);
    bool test2 = (contour == newFp.contour);
    bool test3 = (center == newFp.center);
    bool test4 = (depth == newFp.depth);
    bool test5 = (shape == newFp.shape);
    return test1 && test2 && test3 && test4 && test5;
}
bool Fp::operator!=(Fp newFp) {return !((*this) == newFp);}

string tostr(Fp fp) {return tostr(fp.center);}

Point centroid(vector<Fp> fps) {
#ifdef TEST
    cout << "Running centroid(vector<Fp>)..." << endl;
#endif
    cnt out;
    for (Fp f : fps) {
        out.push_back(f.center);
    }
    return centroid(out);
}

Point centroid(Fp f) {
    return f.center;
}

cnt centroids(vector<Fp> fps) {
#ifdef TEST
    cout << "Running centroids(vector<Fp>)..." << endl;
#endif
    cnt out;
    for (Fp fp : fps) {
        out.push_back(centroid(fp));
    }
    return out;
}

bool allInside(cnt poly, vector<Fp> fps) {
#ifdef TEST
    cout << "Running allInside..." << endl;
#endif
    for (Fp f : fps) {
        if (pointPolygonTest(poly, f.center, false) < 0) {
            return false;
        }
    }
    return true;
}

//Could be a template
vector<vector<Fp>> hasRectangles(vector<Fp> fps, double angleTol, double distRatio, int n) {
#ifdef TEST
    cout << "Running hasRectangles(vector<Fp>...) ..." << endl;
#endif
    cout << fps.size() << endl;
    if (fps.size()<4 || n < 1) {return vector<vector<Fp>>{{}};}
    cout << "here" << endl;
    // Check all combinations of poly
    vector<vector<Fp>> out;
    for (Fp a1 : fps) {
    for (Fp a2 : fps) {
    for (Fp a3 : fps) {
    for (Fp a4 : fps) {
    if (a1 != a2 && a1 != a3 && a1 != a4 && a2 != a3 && a2 != a4 && a3 != a4) {
            cout << "hasRectangles: " << n << endl;
            vector<Fp> found = vector<Fp>{a1, a2, a3, a4};
            if (isRectangle(centroids(found), false, angleTol, distRatio)) {
                    out.push_back(found); n--;
                    if (n <= 0) {return out;}
    }}}}}}
    return out;
}

vector<Fp> hasRectangle(vector<Fp> fps, double angleTol, double distRatio) {
    return hasRectangles(fps, angleTol, distRatio, 1)[0];
}

vector<double> angs(Point x, vector<Fp> fours) {
    return angs(x, centroids(fours));
}

vector<Fp> toFps(cnt cents, vector<Fp> set){
    vector<Fp> out; vector<Fp> next = set;
    for (Point x : cents) {
        for (Fp f : set) {
            if (f.center == x) {
                out.push_back(f);
                break;
            }
            else {next.push_back(f);}
        }
        set = next;
    }
    if (out.size() != cents.size()) {cout<<"ERROR toFps"<<endl;}
    return out;
}
