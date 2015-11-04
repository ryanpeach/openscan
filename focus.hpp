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

    Fp (vector<cnt> conts, double angleTol, double distTol);
    Fp (vector<cnt> conts);
    Fp () {}

    bool operator== (Fp newFp);

    bool operator!= (Fp newFp);
}

Point centroid(vector<Fp> fps);

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

list<double> angs (Point x, list<Fp> fours);
