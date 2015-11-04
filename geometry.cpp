/**
 * geometry.hpp
 *
 * @date Oct 31, 2015
 * @author Ryan Peach
 * @version v0.1
 */
#include <opencv2/opencv.hpp>
#include <vector>
#include <list>
#include <cmath>
#include <typeinfo>

using namespace std;
using namespace cv;

typedef vector<Point> cnt;

#define PI 3.14159265

struct Cnts {
	vector<cnt> contours;
    vector<Vec4i> heirarchy;
};

double dist(Point a, Point b){
	Point diff = a-b;
	double pow1 = pow((double)diff.x,2.0);
	double pow2 = pow((double)diff.y,2.0);
    return sqrt(pow1+pow2);
}

double angle(Point origin, Point a){
    Point v = a - origin;
    return atan2((float)v.x,(float)v.y) * 180.0 / PI;
}

double angle(Point origin, Point c2, Point c3) {
    Point a1 = origin; Point a2 = c2; Point a3 = origin; Point a4 = c3;
    Point diff1 = a2-a1; Point diff2 = a4-a3;
    double norm1 = norm(diff1); double norm2 = norm(diff2);
    return acos(diff1.dot(diff2)/(norm1*norm2)) * 180.0 / PI;
}

vector<double> angles(cnt poly) {
    unsigned int a = 0; unsigned int b = poly.size()-1; unsigned int c = 1;
    vector<double> out;
    while (a < poly.size()) {
        out.push_back(angle(poly[a],poly[b],poly[c]));
        a++;b++;c++;
        if (c==poly.size()) {c=0;}
        if (b==poly.size()) {b=0;}
    }
    return out;
}

vector<double> dists(cnt poly) {
    unsigned int a = 0; unsigned int b = 1;
    vector<double> out;
    while (a < poly.size()) {
        out.push_back(dist(poly[a],poly[b]));
        a++; b++;
        if (b==poly.size()) {b=0;}
    }
    return out;
}

bool allSameLength(cnt poly, int distTol){
    vector<vector<Point>> pairs; vector<double> lengths, error; vector<bool> test; unsigned int i = 0; int mean = 0;

    //Get a list of all lines in poly
    pairs.push_back({poly[poly.size()-1],poly[0]});                                          //Add the first pair to the list
    for (i = 1; i < poly.size(); i++) {pairs.push_back({poly[i-1],poly[i]});}                //Add the rest
    for (i = 0; i < poly.size(); i++) {lengths.push_back(dist(pairs[i][0],pairs[i][1]));}    //Get a list of the length of all pairs in poly

    //Calculate Mean
    for (i = 0; i<lengths.size(); i++) {mean += lengths[i];}
    mean = mean / lengths.size();

    //Get error from mean and test if it is within tolerance
    for (i = 0; i < poly.size(); i++) {error.push_back(abs(lengths[i]-mean));}               //Get the error from the mean of each length
    for (i = 0; i < poly.size(); i++) {test.push_back(error[i] < distTol);}                      //Check if the error is within tolerance
    return find(test.begin(), test.end(), false)!=test.end();                                //Test and return to see if there is a false within the test vector
}

//Point centroid (Point contour) {return contour;}
Point centroid(Point p) {return p;}
template <typename U>
Point centroid(vector<U> c) {
    Point sum = Point(0,0);

    for (U p : c) {
    	sum += centroid(p);
    }
    int s = c.size();
    Point out;
    out.x = sum.x / s;
    out.y = sum.y / s;
    return out;
}
template Point centroid<Point> (cnt);
template Point centroid<cnt> (vector<cnt>);
Point centroid (Cnts contours) {return centroid(contours.contours);}

template <typename G>
vector<G> rotateVec(vector<G> vec){
	cnt out;
	for (unsigned int i = 1; i < vec.size(); i++) {
		out.push_back(vec[i]);
	}
	out.push_back(vec[0]);
	return out;
}

template <typename F>
list<F> rotateLst(list<F> lst){
	return (list<F>)(rotateVec((vector<F>)lst));
}

cnt rotateCnt(cnt contour){
	return rotateVec(contour);
}

bool isPoly(cnt poly, int size, int regular, double angleTol, double distTol) {
	vector<double> angs;
    if (poly.size()==(unsigned int)size && isContourConvex(poly)) {
        if (size == 4) {
            angs = angles(poly);
            for (double a : angs) {if (abs(a-90.0)>angleTol) {return false;}}    //Test that all angles are within tolerance of 90
        }
        if (regular) {return allSameLength(poly, distTol);}
        else {return true;}
    }
    else {return false;}
}
bool isRectangle(cnt poly, bool square, double angleTol, double distTol) {return isPoly(poly,4,square,angleTol,distTol);}
bool isSquare(cnt poly, double angleTol, double distTol) {return isPoly(poly,4,true,angleTol,distTol);}

template <typename T, typename U >
bool contains(U lst, T item) {
	return find(lst.begin(),lst.end(),item)!=lst.end();
}

template bool contains (vector<int>, int);

template <typename T, typename U>
int index(U lst, T item) {
	for (int i = 0; i<lst.size(); i++){
		if (lst[i]==item) {
			return i;
		}
	}
	return -1;
}

struct Fp {
	vector<cnt> contours;
	cnt contour;
	Point center;
	int depth, shape;

	Fp (vector<cnt> conts, double angleTol, double distTol) {
		contours = conts;
		center = centroid(contours);
		depth = findInnerBorder(contours,angleTol,distTol);
		contour = contours[depth];
		shape = contours[depth].size();
	}
	Fp (vector<cnt> conts) {Fp(conts,10.0,5.0);}
	Fp () {}

	bool operator== (Fp newFp) {
		bool test1 = contours == newFp.contours;
		bool test2 = contour == newFp.contour;
		bool test3 = center == newFp.center;
		bool test4 = depth == newFp.depth;
		bool test5 = shape == newFp.shape;
		return test1 && test2 && test3 && test4 && test5;
	}

	bool operator!= (Fp newFp) {
		return !(*this == newFp);
	}

	private:
		//Checks shape of each contour from last to -5 and finds the first 'square.' Returns 0 if none exists.
		//Null-Condition: returns -1;
		int findInnerBorder(vector<cnt> cnts, double angleTol, double distTol) {
			cnt contour;
			for (int x = cnts.size(); x > 0; x++) {
				contour = cnts[x];
				if (isPoly(contour,4,true,angleTol,distTol)) {return x+1;}
			}
			return -1;
		}

};

Point centroid(vector<Fp> fps) {
    vector<cnt> contours;
    for (Fp f : fps) {
        contours.push_back(f.contour);
    }
    return centroid(contours);
}

bool allInside(cnt poly, vector<Fp> fps) {
    for (Fp f : fps) {
        if (pointPolygonTest(poly, f.center, false) < 0) {
            return false;
        }
    }
    return true;
}

cnt hasRectangle(vector<Fp> fps, double angleTol, double distTol) {
    if (fps.size()<4) {return cnt();}
    //check all combinations of poly
    for (Fp a1 : fps) {
        for (Fp a2 : fps) {
            for (Fp a3 : fps) {
                for (Fp a4 : fps) {
                    if (a1 != a2 && a1 != a3 && a1 != a4 && a2 != a3 && a2 != a4 && a3 != a4) {
                        if (isRectangle((cnt){a1.center,a2.center,a3.center,a4.center},false,angleTol,distTol)) {
                            return (cnt){a1.center,a2.center,a3.center,a4.center};
    }}}}}}
    return cnt();
}
