#ifndef SUPPORT
#define SUPPORT

#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <sstream>

using namespace std;
using namespace cv;

typedef vector<Point> cnt;

struct Cnts {
    vector<cnt> contours;
    vector<Vec4i> heirarchy;
};

#include "focus.hpp"

/**
 * Shifts all cells of lst, vec, or contour right by one cell, moves the last cell first.
 * @param  Any list/vector/cnt you wish to rotate.
 * @return The rotated list/vector/contour.
 * @complexity O(?)
 */
template <typename Container, typename G>
Container rotateVec (Container vec){
	Container out;
	for (unsigned int i = 1; i < vec.size(); i++) {
		out.push_back(vec[i]);
	}
	out.push_back(vec[0]);
	return out;
}

template vector<int> rotateVec <vector<int>, int> (vector<int>);
template vector<double> rotateVec <vector<double>, double> (vector<double>);
template cnt rotateVec <cnt, Point> (cnt);
template vector<cnt> rotateVec <vector<cnt>, cnt> (vector<cnt>);

/**
 * Tests whether or not item is within lst.
 * @param  lst: Any list or vector.
 * @return True/False item is inside lst.
 * @complexity O(?)
 */
template <typename Container, typename T>
bool contains(Container lst, T item) {
	return find(lst.begin(),lst.end(),item)!=lst.end();
}

template bool contains (vector<int>, int);
template bool contains (vector<double>, double);
template bool contains (cnt, Point);
template bool contains (vector<cnt>, cnt);

/**
 * Tests whether or not item is within vec.
 * @param  lst: Any list or vector.
 * @return first index of item inside lst.
 * @return -1 if none found.
 * @complexity O(n)
 */
template <typename Container, typename T>
int index(Container lst, T item) {
	for (unsigned int i = 0; i<lst.size(); i++){
		if (lst[i]==item) {
			return i;
		}
	}
	return -1;
}

template int index (vector<int>, int);
template int index (vector<double>, double);
template int index (cnt, Point);
template int index (vector<cnt>, cnt);


template <typename Container, typename T>
string tostr(Container vec) {
	stringstream out;
	out << "{";
	for (T v : vec) {
		out << tostr(v);
		out << ", ";
	}
	out << "}";
	return out.str();
}

string tostr(double p);
string tostr(Point p);
string tostr(cnt contour);

template <typename T, typename G>
void printr(string name, T value, G test, G expect){
	string v = tostr(value); string t = tostr(test); string e = tostr(expect); string r = tostr(test==expect);
	cout << name << " - (" << v << ") " << t << ", " << e << ": (" << r << ")\n";
}

template <typename T, typename G>
void printr(String name, G value, T test){
	cout << name << " - (" << tostr(value) << ") " << tostr(test);
}

template <typename T>
vector<T> initVec (const T array[], unsigned int length) {
	vector<T> out;
	for (unsigned int i = 0; i < length; i++) {
		out.push_back(array[i]);
	}
	return out;
}

template vector<Point> initVec (const Point[], unsigned int);
template vector<cnt> initVec (const cnt array[], unsigned int);

template <typename T>
vector<T> filter(vector<T> vec, bool lambda) {
    vector<T> out = vec;
    auto rem = remove_if(out.begin(),out.end(),!lambda);
    out.erase(rem, out.end());
    return out;
}

#endif
