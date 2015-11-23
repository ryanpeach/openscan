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
#ifdef TEST
	cout << "Running Capture::Frame..." << endl;
#endif
	frame = &img;
	edges = NULL;
	polys = NULL;
	fps = NULL;
	rects = NULL;
}

Mat* Capture::getEdges() {
#ifdef TEST
	cout << "Running Capture::getEdges..." << endl;
#endif
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
#ifdef TEST
	cout << "Running Capture::getPolys..." << endl;
#endif
	if (polys == NULL && getEdges() != NULL) {
		auto out = findPolys(*getEdges(), polyTol);
		polys = &out;
	}
	return polys;
}

Fps* Capture::getFps() {
#ifdef TEST
	cout << "Running Capture::getFps..." << endl;
#endif
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
#ifdef TEST
	cout << "Running Capture::set(cnt)..." << endl;
#endif
	Points cent = corners;
	Point r = calcRef(corners); ref = &r;
	cnt b = sortCorners(cent,*getRef()); border = &b;
}

void Capture::set(Fps corners) {
#ifdef TEST
	cout << "Running Capture::set(Fps)..." << endl;
#endif
	Points cent = centroids(corners);
	Point r = centroid(calcRef(corners)); ref = &r;
	cnt b = sortCorners(cent,*getRef()); border = &b;
}

Point* Capture::getRef() {
#ifdef TEST
	cout << "Running Capture::getRef..." << endl;
#endif
	if (ref == NULL) {
		getBorder();
	}
	return ref;
}

vector<cnt>* Capture::getRects() {
#ifdef TEST
	cout << "Running Capture::getRects..." << endl;
#endif
	if (rects == NULL) {
		vector<cnt> r = hasRectangles((*getPolys()).contours, angleTol, distTol);
		rects = &r;
	}
	return rects;
}

cnt* Capture::getBorder() {
#ifdef TEST
	cout << "Running Capture::getBorder..." << endl;
#endif
	switch(sel) {
	case fpcorners: {
#ifdef TEST
		cout << "Capture::getBorder: fpcorners..." << endl;
#endif
		if (border == NULL && getFps() != NULL) {
			vector<Fp> corners = calcCorners(*getFps(), angleTol, distTol);
			if (corners.size() == 4) {
				set(corners);
			}
		}} break;

	case strongborder: {
#ifdef TEST
		cout << "Capture::getBorder: strongborder..." << endl;
#endif
		vector<cnt> check;
		for (cnt r : (*getRects())) {
			if (validRect(r)) {check.push_back(r);}
		}
		vector<cnt> similar = findSimilar(check, distTol);
		if (!similar.empty()) {
			cnt corners = largest(similar);
			set(corners);
		}} break;

	case regular: {
#ifdef TEST
		cout << "Capture::getBorder: regular..." << endl;
#endif
		vector<cnt> valid;
		for (cnt r : (*getRects())) {
			if (validRect(r)) {valid.push_back(r);}
		}
		if (!valid.empty()) {
			cnt corners = largest(valid);
			set(corners);
		}} break;

	case automatic: {
#ifdef TEST
		cout << "Capture::getBorder: automatic..." << endl;
#endif
		sel = fpcorners;
		if (getBorder() == NULL) {sel = strongborder;}
		if (getBorder() == NULL) {sel = regular;}
		getBorder();
		sel = automatic;
	} break;
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
#ifdef TEST
	cout << "Capture::getBorder: validRect... " << endl;
#endif
	bool out = sizeRatio*(*frame).cols*(*frame).rows
			&& isAspectRatio(r, aspectRatio, ratioTol)
			&& ((*fps).size()==0 || allInside(r, *fps));
#ifdef TEST
	cout << "Capture::getBorder: validRect = " << out << endl;
#endif
	return out;
}
