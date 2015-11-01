/*
 * capture.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: ryanp
 */

#include <vector>
#include <opencv.hpp>

using namespace std;

typedef vector<Point> cnt;

struct Cnts {
	vector<cnt> contours;
    vector<Vec4i> heirarchy;
}

struct Fp {
	vector<cnt> contours;
	Point center;
	int depth, shape;

	Fp (vector<cnt> conts) throws Exception {
		contours = conts;
		center = centroid(contours);
		depth = findInnerBorder(contours);
		shape = contours[this.depth].size();
	}
}


// -------------- Feature Detection ----------------
//Checks shape of each contour from last to -5 and finds the first 'square.' Returns 0 if none exists.
int findInnerBorder(vector<cnt> cnts) {
	cnt contour;
	for (int x = sizeOf(cnts); x > 0; x++) {
		contour = cnts[x]
		if (isPoly(contour,4,true)) {return x+1;}
		else {return -1;}
	}
}
int findInnerBorder(Cnts cnts) {return findInnerBorder(cnts.contours);}

//Filters the img, finds the contours, and returns the Cnts.
//Uses: polyTol
Cnts findPolys (Mat img) {
	//Find contours and heirarchy
	vector<cnt> contours, polys; vector<Vec4i> heirarchy; cnt temp;
	findContours(img, contours, heirarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	//Return approximate polygons
	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(contours[i], temp, polyTol, true);
		polys.push_back(temp);
	}

	//Return Cnts
	return {polys, heirarchy};
}

//Find all the focus points within an image.
vector<Fp> findFocusPoints (Cnts polys) {
	//Definitions
	vector<FP> out; FP tempFp; vector<vector<cnt>> cntV;
	vector<int> done; vector<cnt> contours; int k;

	for(int i = 0; i < polys.contours.size(); i++) {
		k=i; poly.clear();
		if(!done.contains(i)){        //Check that through navigation you haven't been here before
			done.push_back(i);

			//Navigate the heirarchy
			while (heirarchy[k][2] != -1) {
				k=heirarchy[k][2];
				done.push_back(k);
				contours.push_back(polys.contours[k]);
			}
			if (heirarchy[k][2] != -1) {contours.push_back(polys.contours[k]);} //Add the last element

			//Check if there are enough polys to count as a potential focus point, append them to fp
			if (poly.size() >= 5) {cntV.push_back(contours);}
		}
	}

	//Filter the focus points for their innermost border
	for (int x = 0; x < cntV.size()); x++) {
		tempFp = Fp(cntV[x]);
		if (tempFp.depth >= 0) {out.push_back(tempFp);}
	}

	//Return the focus points
	return out;
}

//Classifies squares and selects the four most likely to be corners
//Null-Condition: Returns null
//Uses: angleTol
vector<Fp> getCorners(vector<Fp> focusPoints) {
	list<Fp> fpList = list<Fp>(focusPoints);
	list<Fp> fours = fpList.remove_if([](Fp z){return z.shape == 4;});
	double angles (Point x) {
		vector<double> out;
		for (Fp y : fours) {for (Fp z : fours) {if (x != y && y != z && x != z) {out.push_back(angle(x,y,z));}}}
		return out;
	}

	//Classify corners as having 2 right angles
	list<Fp> out;
	for (f : fours) {
		if (angles(f.contours).remove_if([](double z){return math.abs(z-90.0)<angleTol;}).size()>=2 && !find(out.begin(),out.end(),f)) {
			out.push_back(f);
		}
	}

	//Return their centroids
	if (!hasRectangle(out)) {return null;}
	return vector<Fp>(out);
}

//Sort edges by distance.
//Corners must be a rectangle
//Null-Condition: Returns corners
vector<Fp> sortCorners(vector<Fp> corners) {
	Point cent = centroid(corners); list<double> polar; int n; vector<Fp> out;
	for (Fp f : corners) {polar.push_back(angle(f.center,cent));} //Calculate all the angles from the centroid, maintaining index
	//Sort "corners" by the order of sorted "polar"
	vector<Point> sorted = polar.sort();
	for (int i = 0; i<polar.size()); i++) {
		n = find(polar.begin(), polar.end(), sorted[i]) - polar.begin(); //Gets the index of sorted[i]
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
//Uses: etol1, etol2, eSize
Mat importFilter(Mat img){
	//Declarations
	Mat gray, edges;

	//Convert to gray if not already
	if (isColor(img)) {cvtColor(img,gray,COLOR_RGB2GRAY);}
	else {gray = img.copy();}

	//Return Canny Edge Detection
	Canny(gray,edges,etol1,etol2,eSize);
	return edges
}

//Uses: wSize, C
Mat outputFilter(Mat img){
	Mat gray, out;
	cvtColor(img, gray, COLOR_RGB2GRAY);
	adaptiveThreshold(gray, out, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, wSize, C);
	return out;
}

//Uses: R
Mat cropImage(Mat img){
	int sizeX = img.cols; int sizeY = img.rows;
	return img[R:sizeY-R][R:sizeX-R];
}

//Uses: aspectRatio
//Reference: Modified from http://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
Mat fixPerspective (Mat img, vector<cnt> border, Point ref) {
	//Declare variables
	cnt t1, tr, br, b1;
	Mat M, out;

	//Rotate the array until the reference is first
	while (centroid(border[0]) != ref)
	border = rotateList(border,1);

	tl = border[0]; tr = border[1]; br = border[2]; bl = border[3];

	// compute the width of the new image, which will be the
	// maximum distance between bottom-right and bottom-left
	// x-coordiates or the top-right and top-left x-coordinates
	auto widthA = math.sqrt(((br[0] - bl[0]) ** 2) + ((br[1] - bl[1]) ** 2));
	auto widthB = math.sqrt(((tr[0] - tl[0]) ** 2) + ((tr[1] - tl[1]) ** 2));
	auto maxWidth = max(int(widthA), int(widthB));

	// compute the height of the new image, which will be the
	// maximum distance between the top-right and bottom-right
	// y-coordinates or the top-left and bottom-left y-coordinates
	auto heightA = math.sqrt(((tr[0] - br[0]) ** 2) + ((tr[1] - br[1]) ** 2));
	auto heightB = math.sqrt(((tl[0] - bl[0]) ** 2) + ((tl[1] - bl[1]) ** 2));
	auto maxHeight = max(int(heightA), int(heightB));

	// now that we have the dimensions of the new image, construct
	// the set of destination points to obtain a "birds eye view",
	// (i.e. top-down view) of the image, again specifying points
	// in the top-left, top-right, bottom-right, and bottom-left
	// order
	auto dst = Rect(Point(0, 0),
	Point(maxWidth - 1, 0),
	Point(maxWidth - 1, maxHeight - 1),
	Point(0, maxHeight - 1));

	//Return Perspective Transform
	M = getPerspectiveTransform(border, dst);
	warpPerspective(img, out, M, (maxWidth, maxHeight));
	return out;
}

bool isColor(Mat img){
	if (sizeOf(img[0][0])==3) {return true;}
	else {return false}
}

