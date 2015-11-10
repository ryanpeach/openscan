/*
 * main.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Ryan Peach
 */

#include "cvmethods.hpp"

void testGeometry(){
    //Variable Declarations
    //Points
<<<<<<< HEAD
    const Point a = Point(0,0), b = Point(1,1), c = Point(0,1);
=======
    const Point a = Point(0,0), b = Point(1,1), c = Point(0,1),
>>>>>>> 351bcd36d0027eb1d41b2c29d98e05ef86039309
    const Point e = Point(2,0), d = Point(1,0);
    //Poly's
    const cnt tri {a,e,b}; //non-equal sides
    const cnt square {a,c,b,d};
    const cnt bigSquare {a*2-b,c*2-b,b*2-b,d*2-b};        
    //Fp's
<<<<<<< HEAD
    const Fp testFp1 = Fp({bigSquare,square});
    const Fp testFp2 = Fp({bigSquare}); //not a valid Fp
    const Fp testFp3 = Fp({square}); //not a valid Fp
=======
    const testFp1 = Fp({bigSquare,square});
    const testFp2 = Fp({bigSquare}); //not a valid Fp
    const testFp3 = Fp({square}); //not a valid Fp
>>>>>>> 351bcd36d0027eb1d41b2c29d98e05ef86039309

    //Vectors
    //Poly's
    const vector<cnt> vpoly1 {tri,square,bigSquare};
    const vector<cnt> vpoly2 {tri,square};
    const vector<cnt> vpoly3 {square,bigSquare};
    const vector<cnt> vpoly4 {tri,bigSquare};
    const vector<cnt> vpoly5 {tri};
    const vector<cnt> vpoly6 {square};
    const vector<cnt> vpoly7 {bigSquare};

    //Fp's
    const vector<Fp> vfp1 {testFp1,testFp2,testFp3};
    const vector<Fp> vfp2 {testFp1,testFp2};
    const vector<Fp> vfp3 {testFp2,testFp3};
    const vector<Fp> vfp4 {testFp1,testFp3};
    const vector<Fp> vfp5 {testFp1};
    const vector<Fp> vfp6 {testFp2};
    const vector<Fp> vfp7 {testFp3};
   
    //Test Methods
    //Test allSameLength()
    bool test1a = allSameLength(square, 0); //True
    bool test1b = allSameLength(tri, 0); //False
    printr("allSameLength",square,test1a,true); 
    printr("allSameLength",tri,test1b,false);

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
