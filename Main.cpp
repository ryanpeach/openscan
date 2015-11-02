/*
 * Main.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Ryan Peach
 */

#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <geometry.hpp>
#include <cvmethods.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void main() {
	testGeometry();
}

void testGeometry(){
	//Variable Declarations
	Point a = Point(0,0); Point b = Point(1,1); Point c = Point(0,1);
	cnt testPoly = cnt({Point(0,0),Point(1,1),Point(2,1)});
	cnt bigTestPoly = cnt({Point(-2,-2),Point(-2,2),Point(2,2),Point(2,-2)});
	int distTol = 0;
	Fp testFp1 = Fp(vector<cnt>({bigTestPoly,testPoly}));
	Fp testFp2 = Fp(vector<cnt>({bigTestPoly}));
	Fp testFp3 = Fp(vector<cnt>({testPoly}));
	list<Point> testLst = list<Point>({Point(0,0),Point(1,1),Point(2,2)});
	Cnts contour = Cnts();
	vector<cnt> conts = vector<cnt>({testPoly,bigTestPoly});
	Cnts contours = Cnts();
	contours.contours = conts;

	//Test Methods
	bool test1a = allSameLength(testPoly, 0); //False
	bool test1b = allSameLength(bigTestPoly, 0); //True
	printr("allSameLength",testPoly,test1a,false);printr("allSameLength",bigTestPoly,test1b,true);

	bool test2a = allInside(bigTestPoly, vector<Fp>({testFp3})); //True
	bool test2b = allInside(testPoly, vector<Fp>({testFp1})); //False
	printr("allInside",bigTestPoly,test2a,true); printr("allInside",testPoly,test2b,false);

	auto test3 = rotateLst(testLst);
	auto test4 = rotateVec(vector<Point>(testLst));
	cnt  test5 = rotateCnt(testPoly);
	printr("rotate",testLst,test3,list<Point>({Point(1,1),Point(2,2),Point(0,0)}));
	printr("rotate",testLst,test4,vector<Point>({Point(1,1),Point(2,2),Point(0,0)}));
	printr("rotate",testPoly,test5,vector<Point>({Point(1,1),Point(2,1),Point(0,0)}));

	Point test6 = centroid(testPoly);
	Point test7 = centroid(vector<cnt>({testPoly,bigTestPoly}));
	Point test8 = centroid(contours);
	Point test9 = centroid(vector<Fp>({testFp1,testFp2,testFp3}));
	printr("centroid",testPoly,test6); printr("centroid",testPoly,test7); printr("centroid",testPoly,test8); printr("centroid",testPoly,test9);

	double test10 = dist(a, b);
	double test11 = angle(a, b);
	double test12 = angle(a, b, c);
	printr("centroid",vector<Point>({a,b}),test10); printr("centroid",vector<Point>({a,b}),test11); printr("centroid",vector<Point>({a,b,c}),test12);

	bool test13a = isPoly(testPoly, 3, false, 0); //True
	bool test13b = isPoly(testPoly, 3, true, 0); //False
	bool test13c = isPoly(testPoly, 4, false, 0); //False
	printr("isPoly",testPoly,test13a,true); printr("isPoly",testPoly,test13b,false); printr("isPoly",testPoly,test13c,false);

	bool test14 = isRectangle(bigTestPoly, false, 0); //True
	bool test15 = isSquare(bigTestPoly, 0); //True
	printr("isRectangle",bigTestPoly,test14,true); printr("isSquare",bigTestPoly,test15,true);

	bool test16a = hasRectangle(vector<Fp>({testFp2,testFp3}), 0); //True
	bool test16b = hasRectangle(vector<Fp>({testFp3}), 0); //False
	printr("hasRectangle",vector<Fp>({testFp2,testFp3}),test16a,true); printr("hasRectangle",vector<Fp>({testFp3}),test16b,false);

	vector<double> test17 = angles(testPoly);
	vector<double> test18 = dists(testPoly);
	printr("angles",testPoly,test17); printr("angles",testPoly,test18);

	bool test19a = contains(testLst, Point(1,1));  //True
	bool test20a = contains(testPoly, Point(1,1)); //True
	bool test19b = contains(testLst, Point(3,3));  //True
	bool test20b = contains(testPoly, Point(3,3)); //True
	printr("contains",testLst,test19a,true); printr("contains",testPoly,test20a,true);
	printr("contains",testLst,test19b,true); printr("contains",testPoly,test20b,true);

	int test21 = index(testLst, Point(2,2));
	printr("index",testLst,test21,1);

}

template <typename T, typename G>
void printr(String name, G value, T test, T expect){
	cout << name << " - (" << value << ") " << test << ", " << expect << ": (" << test == expect << ")";
}

template <typename T, typename G>
void printr(String name, G value, T test){
	cout << name << " - (" << value << ") " << test;
}
