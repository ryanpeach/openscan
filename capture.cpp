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
    outline.clear();
    corners.clear();
    polys = Cnts();
    ref = Point();
    frame = img;
    edges = Mat();
}

// ------------ Get Methods --------
Mat Capture::getEdges() {
#ifdef TEST
    cout << "Running Capture::getEdges..." << endl;
#endif
    if (edges.empty()) {
    	cout << "getEdges: empty..." << endl;
        edges = edgesCanny(&frame, etol1, etol2, eSize, false);
    }
    return edges;
}

Cnts Capture::getPolys() {
#ifdef TEST
    cout << "Running Capture::getPolys..." << endl;
#endif
    if(edges.empty()) {getEdges();}
    if (polys.empty() && !edges.empty()) {
        polys = findPolys(&edges, polyTol);
    }
    return polys;
}

//Source: http://docs.opencv.org/2.4/doc/tutorials/features2d/trackingmotion/harris_detector/harris_detector.html
Points Capture::getCorners() {
	if (corners.empty()) {
		Mat gray = toGray(frame);
		Mat dst, dst_norm, dst_norm_scaled;

		// Detecting corners
		cornerHarris(gray, dst, cBlock, cSize, k, BORDER_DEFAULT );

		// Normalizing
		normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
		convertScaleAbs( dst_norm, dst_norm_scaled );

		// Convert to Points
		for (int j = 0; j < dst_norm.rows; j++) {
			for (int i = 0; i < dst_norm.cols; i++) {
				if((int) dst_norm.at<float>(j,i) >= cThresh) {
					corners.push_back(Point(j,i));
				}
			}
		}
	}
    return corners;
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

cnt Capture::getOutline() {
	if (outline.empty()) {
		Point cent = Point(frame.cols/2,frame.rows/2);
		cnt boundary{Point(0,0), Point(frame.cols,0), Point(frame.cols, frame.rows), Point(0, frame.rows)};
		double totalArea = contourArea(boundary);
		double outlineArea = sizeRatio*totalArea;

		// W is the small length, L is the large length
		// Aspect Ratio assumed < 1
		double W = sqrt(outlineArea / aspectRatio);
		double L = W*aspectRatio;

		outline.push_back(Point(cent.x+W/2, cent.y-L/2));
		outline.push_back(Point(cent.x+W/2, cent.y+L/2));
		outline.push_back(Point(cent.x-W/2, cent.y+L/2));
		outline.push_back(Point(cent.x-W/2, cent.y-L/2));
		cout << "getOutline: Area = " << contourArea(outline) << " Expected = " << outlineArea << endl;
	}
	return outline;
}

cnt Capture::getBorder() {
#ifdef TEST
    cout << "Running Capture::getBorder..." << endl;
#endif
    switch (sel) {
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

// ----------- Draw Methods -------------
Mat Capture::drawInfo() {
    // Declare Variables
    Mat out = Mat::zeros(frame.rows, frame.cols, frame.type());

    drawPolys(out, white);  // Print all polys
    drawRects(out, red);    // Print Rectangles
    drawFps(out, green);    // Print Fps
    drawCorners(out, blue); // Print all corners

    return out;
}

Mat Capture::drawEdges() {
	return getEdges();
}

Mat Capture::drawPolys(Mat img, Scalar color) {
    Mat out = img;
    unsigned int N = getPolys().size();
    for (unsigned int i = 0; i < N; i++) {
        drawContours(out, polys.contours, i, color, 2, 8);
    }
    return out;
}

Mat Capture::drawRects(Mat img, Scalar color) {
    Mat out = img;
    unsigned int N = getRects().size();
    for (unsigned int i = 0; i < N; i++) {
        drawContours(out,rects, i, color, 2, 8);
    }
    return out;
}

Mat Capture::drawFps(Mat img, Scalar color) {
    Mat out = img;
    for (unsigned int i = 0; i < getFps().size(); i++) {
        vector<cnt> poly{fps[i].contour};
        drawContours(out, poly, 0, color, 2, 8);
    }
    return out;
}

//Not implemented
Mat Capture::drawCorners(Mat img, Scalar color) {
    Mat out = img;
    if (corners.empty()) {getCorners();}
    /// Drawing a circle around corners
    for(unsigned int j = 0; j < corners.size(); j++ ) {
    	circle(out, corners[j], 5, color, 2, 8, 0);
    }
    return out;
}

Mat Capture::drawBorder(Mat img, Scalar color) {
    Mat out = img;
    vector<cnt> conts{getBorder()};
    drawContours(out, conts, 0, color, 2, 8);
    return out;
}

//Not implemented
Mat Capture::drawOutline(Mat img, Scalar color) {
    Mat out = img;
    vector<cnt> conts{getOutline()};
    drawContours(out, conts, 0, color, 2, 8);
    return out;
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

// Uses polyTol, angleTol, distTol, wSize, C;
vector<Mat> Capture::process() {
#ifdef TEST
    cout << "Running Capture::process..." << endl;
#endif
    // Variable Declaration
    Mat warp; Mat drawing = frame.clone();
    vector<Mat> out;

    if (border.empty()) {getBorder();}
    drawFps(drawing,blue);
    drawRects(drawing,green);
    drawOutline(drawing,yellow);
    if (!border.empty() && ref != Point()) {
        // Get border from focus points and warp
        warp = fixPerspective(&frame, border, ref);

        Color(&warp);

        drawBorder(drawing, red);
        out = vector<Mat>{drawing, warp};
        return out;
    } else {
        out = vector<Mat>{drawing};
        return out;
    }
}

bool Capture::validRect(cnt r) {
#ifdef TEST
    cout << "Capture::getBorder: validRect... " << endl;
#endif
    double area1 = contourArea(getOutline());
    double area2 = contourArea(r);
    bool size = area1 < area2;
    bool ratio = isAspectRatio(r, aspectRatio, ratioTol);
    bool inside = ((fps).size()==0 || allInside(r, fps));
#ifdef TEST
    cout << "Capture::getBorder: validRect = " << size << ratio << inside << " " << area1 << "<" << area2 << endl;
#endif
    return size && ratio;
}
