#include<vector>
#include<list>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

typedef vector<Point> cnt;

/**
 * Shifts all cells of lst, vec, or contour right by one cell, moves the last cell first.
 * @param  Any list/vector/cnt you wish to rotate.
 * @return The rotated list/vector/contour.
 * @complexity O(?)
 */
template <typename Container, typename G> Container rotateVec (Container vec);

template vector<int> rotateVec<vector<int>,int>();
template vector<double> rotateVec<vector<double>,double>();
template cnt rotateVec<cnt,Point>();
template vector<cnt> rotateVec<vector<cnt>,cnt>();

/**
 * Tests whether or not item is within lst.
 * @param  lst: Any list or vector.
 * @return True/False item is inside lst.
 * @complexity O(?)
 */
template <typename Container, typename T> bool contains(Container lst, T item);

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
template <typename T, typename U> int index(U lst, T item);

template int index (vector<int>, int);
template int index (vector<double>, double);
template int index (cnt, Point);
template int index (vector<cnt>, cnt);
