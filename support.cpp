#include "support.hpp"

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

template <typename Container, typename T>
bool contains(Container lst, T item) {
	return find(lst.begin(),lst.end(),item)!=lst.end();
}

template bool contains (vector<int>, int);
template bool contains (vector<double>, double);
template bool contains (cnt, Point);
template bool contains (vector<cnt>, cnt);

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
template vector<Fp> initVec (const Fp array[], unsigned int);

