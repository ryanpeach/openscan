/**
 * capture.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "capture.hpp"
#define TEST

void Capture::Frame(Mat img) {
	frame = &img;
	edges = NULL;
	polys = NULL;
	fps = NULL;
}

Mat* Capture::getEdges() {

	if (edges == NULL) {
		auto out = edgesCanny(*frame, etol1, etol2, eSize);
		edges = &out;
	}

#ifdef TEST
	if (edges != NULL) {imshow("Canny",*edges);}
#endif

	return edges;
}

Cnts* Capture::getPolys() {
	if (polys == NULL && getEdges() != NULL) {
		auto out = findPolys(*getEdges(), polyTol);
		polys = &out;
	}
	return polys;
}

Fps* Capture::getFps() {

	if (fps == NULL && getPolys() != NULL) {
		auto out = findFocusPoints(*getPolys(), angleTol, distTol);
		fps = &out;
	}

#ifdef TEST
    cout << "Fp's Found: " << (*fps).size() << endl;
#endif

	return fps;
}

void Capture::set(cnt corners) {
	Points cent = corners;
	Point r = calcRef(corners); ref = &r;
	cnt b = sortCorners(cent,*getRef()); border = &b;
}

void Capture::set(Fps corners) {
	Points cent = centroids(corners);
	Point r = centroid(calcRef(corners)); ref = &r;
	cnt b = sortCorners(cent,*getRef()); border = &b;
}

Point* Capture::getRef() {
	if (ref == NULL) {
		getBorder();
	}
	return ref;
}

cnt* Capture::getBorder() {
	switch(sel) {
	case fpcorners:
		if (border == NULL && getFps() != NULL) {
			vector<Fp> corners = calcCorners(*getFps(), angleTol, distTol);
			if (border == NULL && corners.size() == 4) {
				set(corners);
			}
		} break;

	case strongborder:
		vector<cnt> rects = hasRectangles((*getPolys()).contours, angleTol, distTol);
		vector<cnt> check;
		for (cnt r : rects) {
		    if (validRect(r)) {check.push_back(r);};
		}
	    vector<cnt> similar = findSimilar(check, distTol);
	    cnt corners = largest(similar);
	    set(corners);
	    break;
	}
	return border;
}

// Uses polyTol, angleTol, distTol, wSize, C;
vector<Mat*> Capture::process() {
#ifdef TEST
    cout << "Running Capture::process..." << endl;
#endif
    // Variable Declaration
    Mat warp, drawing;
    vector<Mat*> out;

    if (getBorder() != NULL && getRef() != NULL) {
		// Get border from focus points and warp
		warp = fixPerspective(*frame, *getBorder(), *getRef());

		warp = toColor(warp);

		Scalar color = Scalar(255, 0, 0);
		drawing = warp;

		drawContours(drawing, *getBorder(), 0, color, 3, 8);
		out = vector<Mat*>{&drawing, &warp};
		return out;
    } else {
    	out = vector<Mat*>{frame,NULL};
    	return out;
    }
}

bool Capture::validRect(cnt r) {
    return contourArea(r) >= sizeRatio*(*frame).cols*(*frame).rows
            && isAspectRatio(r, aspectRatio, ratioTol)
    		&& ((*fps).size()==0 || allInside(r, *fps));
}
