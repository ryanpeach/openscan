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
template <typename Container, typename G> Container rotateVec (Container vec);

vector<int> rotateVec (vector<int>);
vector<double> rotateVec (vector<double>);
cnt rotateVec (cnt);
vector<cnt> rotateVec (vector<cnt>);

/**
 * Tests whether or not item is within lst.
 * @param  lst: Any list or vector.
 * @return True/False item is inside lst.
 * @complexity O(?)
 */
template <typename Container, typename T> bool contains(Container lst, T item);

bool contains (vector<int>, int);
bool contains (vector<double>, double);
bool contains (cnt, Point);
bool contains (vector<cnt>, cnt);

/**
 * Tests whether or not item is within vec.
 * @param  lst: Any list or vector.
 * @return first index of item inside lst.
 * @return -1 if none found.
 * @complexity O(n)
 */
template <typename Container, typename T> int index(Container lst, T item);

int index (vector<int>, int);
int index (vector<double>, double);
int index (cnt, Point);
int index (vector<cnt>, cnt);


template <typename Container, typename T>
string tostr(Container vec);
string tostr(double d);
string tostr(int i) {return tostr((double)i);}
string tostr(Point p);
string tostr(cnt contour);

template <typename T, typename G>
void printr(string name, T value, G test, G expect);

template <typename T, typename G>
void printr(String name, G value, T test);

template <typename T>
vector<T> initVec (const T array[], unsigned int length);

vector<Point> initVec (const Point array[], unsigned int length);
vector<cnt> initVec (const cnt array[], unsigned int length);

template <typename T>
vector<T> filter(vector<T> vec, bool lambda);

#endif
