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
	polys = Cnts(); cout << 3 << endl;
	fps.clear();
	rects.clear();
	border.clear();
	ref = Point();
	frame = img;
	edges = Mat();
}

Mat Capture::getEdges() {
#ifdef TEST
	cout << "Running Capture::getEdges..." << endl;
	cout << frame << endl;
#endif
	Mat out;
	if (edges.empty()) {
		out = edgesCanny(&frame, etol1, etol2, eSize);
		if (!out.empty()) {edges = out;}
	}

#ifdef TEST
	if (edges.empty()) {
		cout << edges << endl;
 		imshow("Canny",edges);}
#endif

	return edges;
}

Cnts Capture::getPolys() {
#ifdef TEST
	cout << "Running Capture::getPolys..." << endl;
#endif
	if (polys.empty() && getEdges().empty()) {
		polys = findPolys(&edges, polyTol);
	}
	return polys;
}

Fps Capture::getFps() {
#ifdef TEST
	cout << "Running Capture::getFps..." << endl;
#endif
	if (fps.empty() && !getPolys().empty()) {
		fps = findFocusPoints(getPolys(), angleTol, distTol);
	}

#ifdef TEST
	cout << "Fp's Found: " << (fps).size() << endl;
#endif

	return fps;
}

void Capture::set(cnt corners) {
#ifdef TEST
	cout << "Running Capture::set(cnt)..." << endl;
#endif
	Points cent = corners;
	ref = calcRef(corners);
	border = sortCorners(cent,getRef());
}

void Capture::set(Fps corners) {
#ifdef TEST
	cout << "Running Capture::set(Fps)..." << endl;
#endif
	Points cent = centroids(corners);
	ref = centroid(calcRef(corners));
	border = sortCorners(cent,getRef());
}

Point Capture::getRef() {
#ifdef TEST
	cout << "Running Capture::getRef..." << endl;
#endif
	if (ref == Point()) {
		getBorder();
	}
	return ref;
}

vector<cnt> Capture::getRects() {
#ifdef TEST
	cout << "Running Capture::getRects..." << endl;
#endif
	if (rects.empty()) {
		rects = hasRectangles(getPolys().contours, angleTol, distTol);
	}
	return rects;
}

cnt Capture::getBorder() {
#ifdef TEST
	cout << "Running Capture::getBorder..." << endl;
#endif
	switch(sel) {
	case fpcorners: {
#ifdef TEST
		cout << "Capture::getBorder: fpcorners..." << endl;
#endif
		if (border.empty() && !getFps().empty()) {
			vector<Fp> corners = calcCorners(getFps(), angleTol, distTol);
			if (corners.size() == 4) {
				set(corners);
			}
		}} break;

	case strongborder: {
#ifdef TEST
		cout << "Capture::getBorder: strongborder..." << endl;
#endif
		vector<cnt> check;
		for (cnt r : (getRects())) {
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
		for (cnt r : (getRects())) {
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
		if (getBorder().empty()) {sel = strongborder;
		if (getBorder().empty()) {sel = regular;
		getBorder();}}
		sel = automatic;
	} break;
	}
	return border;
}

// Uses polyTol, angleTol, distTol, wSize, C;
vector<Mat> Capture::process() {
#ifdef TEST
	cout << "Running Capture::process..." << endl;
#endif
	// Variable Declaration
	Mat warp, drawing;
	vector<Mat> out;
	cout << getBorder()  << endl;
	if (getBorder().empty() && getRef() != Point()) {
		// Get border from focus points and warp
		warp = fixPerspective(&frame, getBorder(), getRef());

		Color(&warp);

		Scalar color = Scalar(255, 0, 0);
		drawing = Mat(warp);

		drawContours(drawing, getBorder(), 0, color, 3, 8);
		out = vector<Mat>{drawing, warp};
		return out;
	} else {
		out = vector<Mat>{frame,Mat()};
		return out;
	}
}

bool Capture::validRect(cnt r) {
#ifdef TEST
	cout << "Capture::getBorder: validRect... " << endl;
#endif
	bool out = sizeRatio*((frame).cols)*((frame).rows)
			&& isAspectRatio(r, aspectRatio, ratioTol)
			&& ((fps).size()==0 || allInside(r, fps));
#ifdef TEST
	cout << "Capture::getBorder: validRect = " << out << endl;
#endif
	return out;
}
