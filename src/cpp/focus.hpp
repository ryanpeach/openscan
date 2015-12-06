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

    int findInnerBorder(vector<cnt> cnts, double angleTol, double distRatio);

    Fp (vector<cnt> conts, cnt cont, Point cent, int d, int s);
    Fp (vector<cnt> conts, double angleTol, double distRatio);
    Fp (vector<cnt> conts);
    Fp ();

    bool operator== (Fp newFp);

    bool operator!= (Fp newFp);

    bool isValid ();

    Fp copy ();
};

typedef vector<Fp> Fps;

Point centroid(vector<Fp> fps);
Point centroid(Fp f);
cnt centroids(vector<Fp> fps);

string tostr(Fp fp);

/**
 * Test that the center of all focus points are inside the poly.
 * @param  cnt poly:       Any cnt you wish to test as the boundary.
 * @param  vector<Fp> fps: A list of Fp's to test locations of.
 * @return True / False
 * @complexity O(?)
 */
bool allInside(cnt poly, vector<Fp> fps);

// Extended from geometry.cpp for easy Fp processing
vector<vector<Fp>> hasRectangles(vector<Fp> fps, double angleTol, double distRatio, int n = 1);
vector<Fp> hasRectangle(vector<Fp> fps, double angleTol, double distRatio);
vector<double> angs(Point x, vector<Fp> fours);
vector<Fp> toFps(cnt,vector<Fp>);
#endif
