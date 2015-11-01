/*
 * geometry.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: ryanp
 */

using namespace std;

#include <vector>
#include "geometry.h"

typedef vector<Point> cnt;

bool allSameLength(cnt poly){
    vector<vector<Point>> pairs; vector<double> lengths, error; vector<bool> test; int i;

    //Get a list of all lines in poly
    pairs.push_back({poly[poly.size()-1],poly[0]}) 											//Add the first pair to the list
    for (i = 1; i < poly.size(); i++) {pairs.push_back({poly[i-1],poly[i]});} 				//Add the rest
    for (i = 0; i < poly.size(); i++) {lengths.push_back(dist(pairs[i][0],pairs[i][1]));} 	//Get a list of the length of all pairs in poly
    double mean = accumulate(lengths.begin(), lengths.end(), 0.0) / lengths.size();
    for (i = 0; i < poly.size(); i++) {error.push_back(abs(lengths[i]-mean));} 			//Get the error from the mean of each length
    for (i = 0; i < poly.size(); i++) {test.push_back(error[i] < distTol);}				//Check if the error is within tolerance
    return find(test.begin(), test.end(), false)!=v.end();									//Test and return to see if there is a false within the test vector
}

//Test that all focus points are inside the poly
bool allInside(cnt poly, vector<FP> fps) {
    for (Fp f : fps) {
        if (!pointPolygonTest(polys[i], f.center, false) {
            return false;
        }
    }
    return true;
}

cnt rotateCnt(cnt contour, int n) {
    cnt AB = cnt().reserve(contour.size());
    AB.insert( AB.end(), contour[n:].begin(), contour[n:].end() );
    AB.insert( AB.end(), contour[:n].begin(), contour[:n].end() );
    return AB;
}
cnt rotateCnt(cnt contour) {return rotateCnt(contour,1);}

//Returns the center of a contour, or of a bunch of contours, or of a bunch of points
Point centroid(cnt contour) {
    Point sum = Point(0,0);
    for (Point p : contour) {sum += p;}
    return sum / contour.size();
}
Point centroid(vector<cnt> contours) {
    Point sum = Point(0,0);
    for (cnt c : contours) {sum += centroid(c);}
    return sum / contours.size();
}
Point centroid(Cnts contours) {return centroid(contours.contours);}
Point centroid(vector<Fp> fps) {
    vector<cnt> contours;
    for (Fp f : fps) {
        contours.push_back(f.contour);
    }
    return centroid(contours);
}

double dist(Point a, Point b){
    return sqrt((a[0]-b[0])**2+(a[1]-b[1])**2);
}

double angle(Point origin, Point a){
    Point v = a - origin;
    return atan2(v[0],v[1]) * 180 / PI;
}

double angle(Point origin, Point c2, Point c3) {
    Point *a1 = origin; Point *a2 = c2; Point *a3 = origin; Point *a4 = c3;
    return acos(dot((*a2-*a1),(*a4-*a3))/(norm(*a2-*a1)*norm(*a4-*a3)))) * 180 / PI;
}

//Runs the polygon rules of this application, that all valid shapes are convex, all size 4 shapes have all right angles within tolerance, and optionally all sides are the same length
bool isPoly(cnt poly, int size, int regular) {
    if (poly.size()==size && isContourConvex(poly)) {
        if (size == 4) {
            auto angles = angles(r);
            for (a : angles) {if (abs(a-90.0)>angleTol) {return false;}}    //Test that all angles are within tolerance of 90
        }
        if (regular) {return allSameLength(poly);}
        else {return true;}
    }
    else {return false;}
}
bool isRectangle(cnt poly, bool square) {return isPoly(poly,4,square);}
bool isSquare(cnt poly) {return isPoly(poly,4,true);}

//Finds if there is a rectangle within poly
bool hasRectangle(vector<Fp> poly) {
    if (sizeOf(poly)!=4) {return false;}
    //check all combinations of poly
    for (a1 : poly) {
        for (a2 : poly) {
            for (a3 : poly) {
                for (a4 : poly) {
                    if (isRectangle({a1.center,a2.center,a3.center,a4.center},false)) {
                        return true;
    }}}}}
    return false;
}

//Returns a vector of angles for the polygon
vector<double> angles(cnt poly) {
    int a = 0; int b = poly.size()-1; int c = 1;
    vector<double> out;
    while (a < poly.size()) {
        out.push_back(angle(poly[a],poly[b],poly[c]));
        a++;b++;c++;
        if (c==poly.size()) {c=0;}
        if (b==poly.size()) {b=0;}
    }
    return out;
}

//Returns a vector of distances for the polygon
vector<double> dists(cnt poly) {
    int a = 0; int b = 1;
    vector<double> out;
    while (a < poly.size()) {
        out.push_back(dist(poly[a],poly[b]));
        a++; b++;
        if (b==poly.size()) {b=0;}
    }
}
