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
template <typename Container> Container rotateVec (Container vec);

//template vector<int> rotateVec <vector<int>, int> (vector<int>);
//template vector<double> rotateVec <vector<double>, double> (vector<double>);
//template cnt rotateVec <cnt, Point> (cnt);
//template vector<cnt> rotateVec <vector<cnt>, cnt> (vector<cnt>);

/**
 * Tests whether or not item is within lst.
 * @param  lst: Any list or vector.
 * @return True/False item is inside lst.
 * @complexity O(?)
 */
template <typename Container, typename T> bool contains(Container lst, T item);

//template bool contains (vector<int>, int);
//template bool contains (vector<double>, double);
//template bool contains (cnt, Point);
//template bool contains (vector<cnt>, cnt);

/**
 * Tests whether or not item is within vec.
 * @param  lst: Any list or vector.
 * @return first index of item inside lst.
 * @return -1 if none found.
 * @complexity O(n)
 */
template <typename Container, typename T> int index(Container lst, T item);

//template int index (vector<int>, int);
//template int index (vector<double>, double);
//template int index (cnt, Point);
//template int index (vector<cnt>, cnt);

string tostr(double p);
string tostr(Point p);
string tostr(cnt contour);
template <typename Container> string vtostr(Container vec);
template <> string vtostr(Cnts vec);

template <typename T, typename G> void printr(string name, T value, G test, G expect);

template <typename T, typename G> void printr(String name, G value, T test);

template <typename T> vector<T> initVec (const T array[], unsigned int length);

//template vector<Point> initVec (const Point[], unsigned int);
//template vector<cnt> initVec (const cnt array[], unsigned int);

#endif
