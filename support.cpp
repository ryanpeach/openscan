#include "support.hpp"

template <typename Container>
Container rotateVec (Container vec){
	Container out;
	for (unsigned int i = 1; i < vec.size(); i++) {
		out.push_back(vec[i]);
	}
	out.push_back(vec[0]);
	return out;
}

template <typename Container, typename T>
bool contains(Container lst, T item) {
	return find(lst.begin(),lst.end(),item)!=lst.end();
}

template <typename Container, typename T>
int index(Container lst, T item) {
	for (unsigned int i = 0; i<lst.size(); i++){
		if (lst[i]==item) {
			return i;
		}
	}
	return -1;
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

template<>
string vtostr(Cnts vec) {
	return vtostr(vec.contours);
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
