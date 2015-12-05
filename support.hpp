/**
 * support.hpp
 *
 * @date Oct 31, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#ifndef SUPPORT
#define SUPPORT

#include <vector>
#include "opencv2/core.hpp"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;
using namespace cv;

typedef vector<Point> cnt;
typedef vector<Point> Points; //This is an unordered cnt.

struct Cnts {
    vector<cnt> contours;
    vector<Vec4i> heirarchy;

    Cnts(vector<cnt> polys, vector<Vec4i> heir);
    Cnts();
    bool empty();
    int size();
};

double distTol(cnt poly, double distRatio);

bool matEq(Mat m1, Mat m2);

/**
 * Shifts all cells of lst, vec, or contour right by one cell, moves the last cell first.
 * @param  Any list/vector/cnt you wish to rotate.
 * @return The rotated list/vector/contour.
 * @complexity O(?)
 * @tested untested
 */
template <typename Container>
Container rotateVec (Container vec){
	Container out;
	for (unsigned int i = 1; i < vec.size(); i++) {
		out.push_back(vec[i]);
	}
	out.push_back(vec[0]);
	return out;
}


/**
 * Tests whether or not item is within lst.
 * @param  lst: Any list or vector.
 * @return True/False item is inside lst.
 * @complexity O(?)
 * @tested untested
 */
template <typename Container, typename T>
bool contains(Container lst, T item) {
	return find(lst.begin(),lst.end(),item)!=lst.end();
}


/**
 * Tests whether or not item is within vec.
 * @param  lst: Any list or vector.
 * @return first index of item inside lst.
 * @return -1 if none found.
 * @complexity O(n)
 * @tested untested
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


/**
 * Converts any type into a formatted string, vstring does the same for vectors
 * @param  Any compatible type or vector of types to convert to string.
 * @return Display string
 * @complexity O(1), O(N)
 * @tested works
 */
string tostr(double p);
string tostr(Point p);
string tostr(cnt contour);

template <typename Container>
string vtostr(Container vec) {
	stringstream out;
	out << "{";
	for (auto v : vec) {
		out << tostr(v);
		out << ", ";
	}
	out << "}";
	return out.str();
}

template<> string vtostr(Cnts vec);


/**
 * Returns the sum of a vector of items which support addition.
 * @param vector of numbers
 * @return sum of numers
 * @complexity O(N)
 * @tested works
 */
double sum(vector<double> nums);
Point sum(Points pts);


/**
 * Returns the mean of a vector of summable and divisible items.
 * @param vector
 * @return mean of vector
 * @complexity O(N)
 * @tested works
 */
template <typename T>
T mean(vector<T> nums){
    return sum(nums) / nums.size();
}

/**
 * Rounds n to the nearest odd number.
 * @param any int
 * @return none, modifies the int directly via pointers
 * @complexity O(1)
 * @tested untested
 */
void Odd(int *n);
int toOdd(int n);

/**
 * Returns whether a == b given a certain tolerance.
 * @param any two numbers
 * @return whether abs(a-b)<=tol.
 * @complexity O(1)
 * @tested works
 */
bool tolEq(double a, double b, double tol);
bool tolEq(int a, int b, int tol);

#endif
