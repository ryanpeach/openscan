#include<vector>
#include<list>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

typedef vector<Point> cnt;

template <typename Container, typename G>
Container rotateVec (Container vec){
	G out;
	for (unsigned int i = 1; i < vec.size(); i++) {
		out.push_back(vec[i]);
	}
	out.push_back(vec[0]);
	return out;
}

template vector<int> rotateVec<vector<int>,int>();
template vector<double> rotateVec<vector<double>,double>();
template cnt rotateVec<cnt,Point>();
template vector<cnt> rotateVec<vector<cnt>,cnt>();

template <typename Container, typename T>
bool contains(Container lst, T item) {
	return find(lst.begin(),lst.end(),item)!=lst.end();
}

template bool contains (vector<int>, int);
template bool contains (vector<double>, double);
template bool contains (cnt, Point);
template bool contains (vector<cnt>, cnt);


template <typename T, typename U>
int index(U lst, T item) {
	for (int i = 0; i<lst.size(); i++){
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

