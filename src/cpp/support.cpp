/**
 * support.cpp
 *
 * @date Oct 31, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "support.hpp"
//#define TEST

Cnts::Cnts(vector<cnt> polys, vector<Vec4i> heir):
    contours(polys), heirarchy(heir)
{}

Cnts::Cnts() {}

bool Cnts::empty() {
	return contours.empty();
}

int Cnts::size() {
	return contours.size();
}

bool matEq(Mat a, Mat b) {
#ifdef TEST
    cout << "Running matEq..." << endl;
#endif
    return countNonZero(a != b) == 0;
}

string tostr(double p) {
#ifdef TEST
    cout << "Running tostr(double)..." << endl;
#endif
    stringstream out;
    out << p;
    return out.str();
}

string tostr(Point p) {
#ifdef TEST
    cout << "Running tostr(Point)..." << endl;
#endif
    stringstream out;
    out << "(";
    out << (double)p.x << ", ";
    out << (double)p.y << ")";
    return out.str();
}

string tostr(cnt contour) {
#ifdef TEST
    cout << "Running tostr(cnt)..." << endl;
#endif
    stringstream out;
    for (Point p : contour) {
        out << tostr(p);
        out << ", ";
    }
    return out.str();
}

template<>
string vtostr(Cnts vec) {
    return vtostr(vec.contours);
}


double sum(vector<double> nums) {
#ifdef TEST
    cout << "Running sum(vector<double>)..." << endl;
#endif
    double out = 0;
    for (unsigned int i = 0; i < nums.size(); i++) {
        out += nums[i];
    }
    return out;
}

Point sum(Points pts) {
#ifdef TEST
    cout << "Running sum(vector<double>)..." << endl;
#endif
    Point out = Point(0, 0);
    for (unsigned int i = 0; i < pts.size(); i++) {
        out += pts[i];
    }
    return out;
}

void Odd(int *n){
	while((*n)%2!=1) {(*n)++;}
}

int toOdd(int n){
	while((n)%2!=1) {(n)++;}
	return n;
}

bool tolEq(double a, double b, double tol) {
	return abs(a-b)<=tol;
}
bool tolEq(int a, int b, int tol) {
	return abs(a-b)<=tol;
}
