#include "support.hpp"

Cnts::Cnts(vector<cnt> polys, vector<Vec4i> heir):
    contours(polys), heirarchy(heir)
{}

Cnts::Cnts(){}

bool matEq(Mat a, Mat b) {
    return countNonZero(a!=b)==0;
}

string tostr(double p) {
	stringstream out;
	out << p;
	return out.str();
}

string tostr(Point p) {
	stringstream out;
	out << "(";
	out << (double)p.x << ", ";
	out << (double)p.y << ")";
	return out.str();
}

string tostr(cnt contour) {
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


double sum(vector<double> nums){
    double out = 0;
    for (unsigned int i = 0; i < nums.size(); i++) {
        out += nums[i];
    }
    return out;
}

Point sum(cnt pts) {
    Point out = Point(0,0);
    for (unsigned int i = 0; i < pts.size(); i++) {
        out += pts[i];
    }
    return out;
}
