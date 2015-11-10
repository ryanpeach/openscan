/*
 * main.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Ryan Peach
 */

#include "cvmethods.hpp"

void testGeometry(){
	//Variable Declarations
//	const Point a = Point(0,0), b = Point(1,1), c = Point(0,1);
//	const Point abA[] = {a,b}; const Point abcA[] = {a,b,c};
//	const vector<Point> ab = initVec(abA,2); const vector<Point> abc = initVec(abcA,3);
//	const Point testPolyA[] = {Point(0,0),Point(1,1),Point(2,1)};
//	const string testPolyS = "{Point(0,0),Point(1,1),Point(2,1)}";
//
//	const cnt testFp1A[] = {bigTestPoly, testPoly};
//	const cnt testFp2A[] = {bigTestPoly};
//	const cnt testFp3A[] = {testPoly};
//	const Fp testFp1 = Fp(initVec(testFp1A,2));
//	const Fp testFp2 = Fp(initVec(testFp2A,1));
//	const Fp testFp3 = Fp(initVec(testFp3A,1));
//	const Fp testFpsA[] = {testFp1,testFp2,testFp3};
//	const Fp testFpsB[] = {testFp2,testFp3};
//	const Fp testFpsC[] = {testFp3};
//	const vector<Fp> testFps1 = initVec(testFpsA,3);
//	const vector<Fp> testFps2 = initVec(testFpsB,2);
//	const vector<Fp> testFps3 = initVec(testFpsC,1);
//
//	const Point testLstA[] = {Point(0,0),Point(1,1),Point(2,2)};
//	const vector<Point> testLst = initVec(testLstA,3);
//	const vector<cnt> conts = vector<cnt>(initVec(testFp1A,2));

	//Test Methods
        //Test allSameLength()
	cnt testPoly {Point(0,0),Point(1,1),Point(2,1)};
        bool test1a = allSameLength(testPoly, 0); //False
	printr("allSameLength",testPoly,test1a,false);

//	const Fp ar1[] = {testFp3}; const Fp ar2[] = {testFp1};
//	bool test2a = allInside(bigTestPoly, initVec(ar1,1)); //True
//	bool test2b = allInside(testPoly, initVec(ar2,1)); //False
//	printr("allInside",bigTestPoly,test2a,true); printr("allInside",testPoly,test2b,false);
//
//	const Point ar3[] = {Point(1,1),Point(2,1),Point(0,0)};
//	const cnt test4 = rotateVec(testPoly);
//	const cnt answ4 = initVec(ar3,3);
//	printr("rotate",testPoly,test4,answ4);
//
//	Point test6 = centroid(testPoly);
//	Point test7 = centroid(testFp1);
//	Point test8 = centroid(conts);
//      Point test9 = centroid(testFps1);
//	printr("centroid",testPoly,test6); printr("centroid",testPoly,test7); printr("centroid",testPoly,test8); printr("centroid",testPoly,test9);
//
//	double test10 = dist(a, b);
//	double test11 = angle(a, b);
//	double test12 = angle(a, b, c);
//	printr("centroid",ab,test10); printr("centroid",ab,test11); printr("centroid",abc,test12);
//
//	bool test13a = isPoly(testPoly, 3, false, 0, 0); //True
//	bool test13b = isPoly(testPoly, 3, true, 0, 0); //False
//	bool test13c = isPoly(testPoly, 4, false, 0, 0); //False
//	printr("isPoly",testPoly,test13a,true); printr("isPoly",testPoly,test13b,false); printr("isPoly",testPoly,test13c,false);
//
//	bool test14 = isRectangle(bigTestPoly, false, 0, 0); //True
//	bool test15 = isSquare(bigTestPoly, 0, 0); //True
//	printr("isRectangle",bigTestPoly,test14,true); printr("isSquare",bigTestPoly,test15,true);
//
//	bool test16a = hasRectangle(testFps2, 0.0, 0.0).size()!=0; //True
//	bool test16b = hasRectangle(testFps3, 0.0, 0.0).size()!=0; //False
//	printr("hasRectangle",testFps2,test16a,true); printr("hasRectangle",testFps3,test16b,false);
//
//	vector<double> test17 = angles(testPoly);
//	vector<double> test18 = dists(testPoly);
//	printr("angles",testPoly,test17); printr("angles",testPoly,test18);
//
//	bool test19a = contains(testLst, Point(1,1));  //True
//	bool test20a = contains(testPoly, Point(1,1)); //True
//	bool test19b = contains(testLst, Point(3,3));  //True
//	bool test20b = contains(testPoly, Point(3,3)); //True
//	printr("contains",testLst,test19a,true); printr("contains",testPoly,test20a,true);
//	printr("contains",testLst,test19b,true); printr("contains",testPoly,test20b,true);
//
//	int test21 = index(testLst, Point(2,2));
//	printr("index",testLst,test21,1);

}

int main() {
	testGeometry();
	return 0;
}
