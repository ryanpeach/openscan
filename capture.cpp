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
	fps.clear();
	rects.clear();
	border.clear();
	polys = Cnts();
	ref = Point();
	frame = img;
	edges = Mat();
}

Mat Capture::getEdges() {
#ifdef TEST
	cout << "Running Capture::getEdges..." << endl;
#endif
	if (edges.empty()) {
		edges = edgesCanny(&frame, etol1, etol2, eSize);
	}


	return edges;
}

Cnts Capture::getPolys() {
#ifdef TEST
	cout << "Running Capture::getPolys..." << endl;
#endif
	if (edges.empty()) {getEdges();}
	if (polys.empty() && !edges.empty()) {
		polys = findPolys(&edges, polyTol);
	}
#ifdef TEST
	Mat draw = Mat::zeros(edges.rows, edges.cols, edges.type());
	const auto white = Scalar(255,255,255);
	for (int i = 0; i<polys.contours.size();i++) {drawContours(draw,polys.contours,i,white,2,8);}
	imshow("Edges", draw);
#endif
	return polys;
}

Fps Capture::getFps() {
#ifdef TEST
	cout << "Running Capture::getFps..." << endl;
#endif
	if (polys.empty()) {getPolys();}
	if (fps.empty() && !polys.empty()) {
		fps = findFocusPoints(polys, angleTol, distTol);
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
	border = sortCorners(cent,ref);
}

void Capture::set(Fps corners) {
#ifdef TEST
	cout << "Running Capture::set(Fps)..." << endl;
#endif
	Points cent = centroids(corners);
	ref = centroid(calcRef(corners));
	border = sortCorners(cent,ref);
}

vector<cnt> Capture::getRects() {
#ifdef TEST
	cout << "Running Capture::getRects..." << endl;
#endif
	if (polys.empty()) {getPolys();}
	if (rects.empty() && !polys.empty()) {
		rects = hasRectangles(polys.contours, angleTol, distTol);
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
		if (fps.empty()) {getFps();}
		if (border.empty() && !fps.empty()) {
			vector<Fp> corners = calcCorners(fps, angleTol, distTol);
			if (corners.size() == 4) {
				set(corners);
			}
		}} cout << border << endl; break;

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
	if (border.empty()) {getBorder();}
	if (!border.empty() && ref != Point()) {
		// Get border from focus points and warp
		warp = fixPerspective(&frame, border, ref);

		Color(&warp);

		Scalar color = Scalar(255, 0, 0);
		drawing = frame.clone();

		vector<cnt> conts{border};
		drawContours(drawing, conts, 0, color, 3, 8);
		out = vector<Mat>{drawing, warp};
		return out;
	} else {
		out = vector<Mat>{};
		return out;
	}
}

bool Capture::validRect(cnt r) {
#ifdef TEST
	cout << "Capture::getBorder: validRect... " << endl;
#endif
	double area1 = sizeRatio*((frame).cols)*((frame).rows);
	double area2 = contourArea(r);
	bool size = area1 < area2;
	bool ratio = isAspectRatio(r, aspectRatio, ratioTol);
	bool inside = ((fps).size()==0 || allInside(r, fps));
#ifdef TEST
	cout << "Capture::getBorder: validRect = " << size << ratio << inside << " " << area1 << "<" << area2 << endl;
#endif
	return size && ratio;
}
