/*
 * geometry.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Ryan Peach
 */

#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <geometry.hpp>
#include <capture.hpp>

using namespace std;
using namespace cv;

typedef vector<Point> cnt;

#define PI 3.14159265

bool allSameLength(cnt poly, int tol){
    vector<vector<Point>> pairs; vector<double> lengths, error; vector<bool> test; int i; int mean = 0;

    //Get a list of all lines in poly
    pairs.push_back({poly[poly.size()-1],poly[0]});                                          //Add the first pair to the list
    for (i = 1; i < poly.size(); i++) {pairs.push_back({poly[i-1],poly[i]});}                //Add the rest
    for (i = 0; i < poly.size(); i++) {lengths.push_back(dist(pairs[i][0],pairs[i][1]));}    //Get a list of the length of all pairs in poly

    //Calculate Mean
    for (i = 0; i<lengths.size(); i++) {mean += lengths[i];}
    mean = mean / lengths.size();

    //Get error from mean and test if it is within tolerance
    for (i = 0; i < poly.size(); i++) {error.push_back(abs(lengths[i]-mean));}               //Get the error from the mean of each length
    for (i = 0; i < poly.size(); i++) {test.push_back(error[i] < tol);}                      //Check if the error is within tolerance
    return find(test.begin(), test.end(), false)!=test.end();                                //Test and return to see if there is a false within the test vector
}

//Test that all focus points are inside the poly
bool allInside(cnt poly, vector<Fp> fps) {
    for (Fp f : fps) {
        if (pointPolygonTest(poly, f.center, false) < 0) {
            return false;
        }
    }
    return true;
}

template <typename G>
vector<G> rotateVec(vector<G> vec){
	cnt out = cnt().reserve(vec.size());
	for (int i = 1; i < vec.size(); i++) {
		out.push_back(vec[i]);
	}
	out.push_back(vec[0]);
	return out;
}
template <typename F>
list<F> rotateLst(list<F> lst){
	return rotateVec(vector<F>(lst));
}
cnt rotateCnt(cnt contour){
	return rotateVec(contour);
}

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
	Point diff = a-b;
	double pow1 = pow((double)diff[0],2.0);
	double pow2 = pow((double)diff[1],2.0);
    return sqrt(pow1+pow2);
}

double angle(Point origin, Point a){
    Point v = a - origin;
    return atan2((float)v[0],(float)v[1]) * 180.0 / PI;
}

double angle(Point origin, Point c2, Point c3) {
    Point a1 = origin; Point a2 = c2; Point a3 = origin; Point a4 = c3;
    Point diff1 = a2-a1; Point diff2 = a4-a3;
    double norm1 = norm(diff1); double norm2 = norm(diff2);
    return acos(diff1.dot(diff2)/(norm1*norm2)) * 180.0 / PI;
}

//Runs the polygon rules of this application, that all valid shapes are convex, all size 4 shapes have all right angles within tolerance, and optionally all sides are the same length
bool isPoly(cnt poly, int size, int regular, double angleTol) {
    if (poly.size()==size && isContourConvex(poly)) {
        if (size == 4) {
            auto angles = angles(poly);
            for (double a : angles) {if (abs(a-90.0)>angleTol) {return false;}}    //Test that all angles are within tolerance of 90
        }
        if (regular) {return allSameLength(poly, angleTol);}
        else {return true;}
    }
    else {return false;}
}
bool isRectangle(cnt poly, bool square, double angleTol) {return isPoly(poly,4,square,angleTol);}
bool isSquare(cnt poly, double angleTol) {return isPoly(poly,4,true,angleTol);}

//Finds if there is a rectangle within poly
bool hasRectangle(vector<Fp> poly, double tol) {
    if (poly.size()!=4) {return false;}
    //check all combinations of poly
    for (Fp a1 : poly) {
        for (Fp a2 : poly) {
            for (Fp a3 : poly) {
                for (Fp a4 : poly) {
                    if (isRectangle((cnt){a1.center,a2.center,a3.center,a4.center},false,tol)) {
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
    return out;
}

//Other functions
template <typename T>
bool contains(list<T> lst, T item) {
	return find(lst.begin(),lst.end(),item);
}

template <typename E>
bool contains(vector<E> lst, E item) {
	return find(lst.begin(),lst.end(),item);
}

template <typename G>
//Null-Condition: returns -1
int index(list<G> lst, G item) {
	for (int i = 0; i<lst.size(); i++){
		if (lst[i]==item) {
			return i;
		}
	}
	return -1;
}
