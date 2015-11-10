#include "support.hpp"

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
