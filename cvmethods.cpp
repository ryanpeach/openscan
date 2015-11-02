/*
 * cvmethods.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Ryan Peach
 */

#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <geometry.hpp>
#include <cvmethods.hpp>

using namespace std;
using namespace cv;

typedef vector<Point> cnt;

struct Cnts {
	vector<cnt> contours;
    vector<Vec4i> heirarchy;
};

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


// -------------- Feature Detection ----------------

//Filters the img, finds the contours, and returns the Cnts.
//Uses: polyTol
Cnts findPolys (Mat img, double tol) {
	//Find contours and heirarchy
	vector<cnt> contours, polys; vector<Vec4i> heirarchy; cnt temp;
	findContours(img, contours, heirarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	//Return approximate polygons
	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(contours[i], temp, tol, true);
		polys.push_back(temp);
	}

	//Return Cnts
	return {polys, heirarchy};
}

//Find all the focus points within an image.
vector<Fp> findFocusPoints (Cnts polys, double angleTol, double distTol) {
	//Definitions
	vector<Fp> out; Fp tempFp; vector<vector<cnt>> cntV;
	vector<int> done; vector<cnt> contours; int k;
	cnt poly;

	for(int i = 0; i < polys.contours.size(); i++) {
		k=i; poly.clear();
		if(!contains(done,i)){        //Check that through navigation you haven't been here before
			done.push_back(i);

			//Navigate the heirarchy
			while (polys.heirarchy[k][2] != -1) {
				k=polys.heirarchy[k][2];
				done.push_back(k);
				contours.push_back(polys.contours[k]);
			}
			if (polys.heirarchy[k][2] != -1) {contours.push_back(polys.contours[k]);} //Add the last element

			//Check if there are enough polys to count as a potential focus point, append them to fp
			if (poly.size() >= 5) {cntV.push_back(contours);}
		}
	}

	//Filter the focus points for their innermost border
	for (int x = 0; x < cntV.size(); x++) {
		tempFp = Fp(cntV[x],angleTol,distTol);
		if (tempFp.depth >= 0) {out.push_back(tempFp);}	//Check that cntV[x] is a valid Fp
	}

	//Return the focus points
	return out;
}

//Classifies squares and selects the four most likely to be corners
//Null-Condition: Returns null
//Uses: angleTol
vector<Fp> getCorners(vector<Fp> focusPoints, double angleTol, double distTol) {
	list<Fp> fpList = list<Fp>(focusPoints);
	list<Fp> fours = fpList;
	fours.remove_if([](Fp z){return z.shape != 4;});  //Make fours a list of only size four Fp's
	list<double> angles (Point x) {
		vector<double> out;
		for (Fp y : fours) {for (Fp z : fours) {if (x != y && y != z && x != z) {out.push_back(angle(x,y.center,z.center));}}}
		return out;
	}

	//Classify corners as having 2 right angles
	vector<Fp> out;
	for (Fp f : fours) {
		auto angs = angles(f.center);
		angs.remove_if([](double z){return abs(z-90.0)<angleTol;});
		if (angs.size()>=2 && !contains(out,f)) {
			out.push_back(f);
		}
	}

	//Return their centroids
	if (!hasRectangle(out, angleTol, distTol)) {return vector<Fp>();}
	return vector<Fp>(out);
}

//Sort edges by distance.
//Corners must be a rectangle
//Null-Condition: Returns corners
vector<Fp> sortCorners(vector<Fp> corners) {
	Point cent = centroid(corners); list<double> polar; int n; vector<Fp> out;
	for (Fp f : corners) {polar.push_back(angle(f.center,cent));} //Calculate all the angles from the centroid, maintaining index
	list<double> sorted = polar.sort();
	//Sort "corners" by the order of sorted "polar"
	for (int i = 0; i<polar.size(); i++) {
		n = index(polar, (double)sorted[i]);
		out.push_back(corners[n]); //Return sorted corners
	}

	return out;
}

//Null-Condition: Returns fps[0]
Fp getRef(vector<Fp> fps) {
	Fp maxFp = fps[0]; int max = maxFp.depth; //sets default values
	for (Fp fp : fps) {
		if (fp.depth > max) {
			maxFp = fp;
			max = fp.depth;
		}
	}
	return maxFp;
}

//Null-Condition: Returns contour[0]
Point getRef(cnt contour) {
	auto D = dists(contour); int a = 0; int b = 1;
	while (D[a]<=D[b]){
		a++; b++;
		if (b>=contour.size()){b=0;}
		if (a==contour.size()){return contour[0];}
	}
	return contour[a];
}

// ------------ Image Manipulation --------------
//wSize must be an odd number, will be rounded up.
Mat importFilter(Mat img, int tol1, int tol2, int wSize){
	//Testing & Declarations
	Mat gray, edges;
	while(wSize%2!=1) {wSize++;}	//wSize must be an odd number

	//Convert to gray if not already
	if (isColor(img)) {cvtColor(img,gray,COLOR_RGB2GRAY);}
	else {gray = img;}

	//Return Canny Edge Detection
	Canny(gray,edges,tol1,tol2,wSize);
	return edges;
}

//Uses: wSize, C
Mat outputFilter(Mat img, int wSize, int C){
	//Testing & Declarations
	Mat gray, out;
	while(wSize%2!=1) {wSize++;}	//wSize must be an odd number

	//Convert to gray if not already
	if (isColor(img)) {cvtColor(img,gray,COLOR_RGB2GRAY);}
	else {gray = img;}

	adaptiveThreshold(gray, out, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, wSize, C);
	return out;
}

//Uses: R
Mat cropImage(Mat img, int R){
	int sizeX = img.cols; int sizeY = img.rows;
	Mat out = Mat(sizeY-2*R,sizeX-2*R,img.type());
	for (int x = R; x<sizeX-R; x++) {
		for (int y = R; y<sizeY-R; y++) {
			out[y-R][x-R]=img[y][x];
		}
	}
	return out;
}

//Uses: aspectRatio
//Reference: Modified from http://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
Mat fixPerspective (Mat img, vector<cnt> border, Point ref) {
	//Declare variables
	cnt tl, tr, bl, br;
	Mat out;

	//Rotate the array until the reference is first
	while (centroid(border[0]) != ref)
	border = rotateVec(border);

	tl = border[0]; tr = border[1]; br = border[2]; bl = border[3];

	// compute the width of the new image, which will be the
	// maximum distance between bottom-right and bottom-left
	// x-coordiates or the top-right and top-left x-coordinates
	auto widthA = sqrt(pow((double)(br[0] - bl[0]), 2.0) + pow((double)(br[1] - bl[1]), 2.0));
	auto widthB = sqrt(pow((double)(tr[0] - tl[0]), 2.0) + pow((double)(tr[1] - tl[1]), 2.0));
	int maxWidth = max(int(widthA), int(widthB));

	// compute the height of the new image, which will be the
	// maximum distance between the top-right and bottom-right
	// y-coordinates or the top-left and bottom-left y-coordinates
	auto heightA = sqrt(pow((double)(tr[0] - br[0]), 2.0) + pow((double)(tr[1] - br[1]), 2.0));
	auto heightB = sqrt(pow((double)(tl[0] - bl[0]), 2.0) + pow((double)(tl[1] - bl[1]), 2.0));
	int maxHeight = max((int)heightA, (int)heightB);

	// now that we have the dimensions of the new image, construct
	// the set of destination points to obtain a "birds eye view",
	// (i.e. top-down view) of the image, again specifying points
	// in the top-left, top-right, bottom-right, and bottom-left
	// order
	auto dst = vector<Point2f>({Point(0, 0),
	Point(maxWidth - 1, 0),
	Point(maxWidth - 1, maxHeight - 1),
	Point(0, maxHeight - 1)});
	auto src = vector<Point2f>({border[0],border[1],border[2],border[3]});

	//Return Perspective Transform
	auto M = getPerspectiveTransform(src, dst);
	warpPerspective(img, out, M, Size(maxWidth, maxHeight));
	return out;
}

bool isColor(Mat img){
	if (sizeof(img[0][0])==3) {return true;}
	else {return false;}
}
