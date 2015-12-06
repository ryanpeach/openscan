/**
 * capture.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "capture.hpp"
#define TEST

int PtoInt(double v) {return (int)(v*VSCALE);}
double PtoDouble(int v) {return ((double)v)/VSCALE;}

void Capture::Frame(Mat img) {
#ifdef TEST
    cout << "Running Capture::Frame..." << endl;
#endif
    // Set frame and stats
    frame = img;
    cent = Point(frame.cols/2,frame.rows/2);
    cnt boundary{Point(0,0), Point(frame.cols,0), Point(frame.cols, frame.rows), Point(0, frame.rows)};
    totalArea = contourArea(boundary);

    fps.clear();
    rects.clear();
    border.clear();
    outline.clear();
    polys = Cnts();
    ref = Point();
    edges = Mat();
}

void Capture::setValue(Par param, int value) {
#ifdef TEST
    cout << "Running Capture::setValue... param = " << param << ", value = " << value << endl;
#endif
    bool changed = true;
    switch(param) {
        case ANGLETOL: angleTol = value; break;
        case DISTRATIO: distRatio = PtoDouble(value); break;
        case POLYTOL: polyTol = value; break;
        case ASPECTRATIO: setAspectRatio((PageType)value); break;
        case SIZERATIO: sizeRatio = PtoDouble(value); break;
        case RATIOTOL: ratioTol = PtoDouble(value); break;
        case ETOL1: etol1 = value; break;
        case ETOL2: etol2 = value; break;
        case ESIZE: eSize = toOdd(value); break;
        case METHOD: sel = (Method)value; break;
        case BSIZE: bSize = toOdd(value); break;
        case BSIGMA: bSigma = PtoDouble(value); break;
        default: changed = false; break;
    }
    if (changed) {
        Frame(frame);
    }
}

void Capture::setAspectRatio(PageType type) {
    switch (type) {
        case letter:
            aspectRatio = 8.5/11.0;
            break;
        case detect:
            aspectRatio = 0;
            break;
    }
}

// ------------ Get Methods --------
int Capture::getValue(Par param) {
#ifdef TEST
    cout << "Running Capture::getValue... param = " << param << endl;
#endif
    switch(param) {
        case ANGLETOL: return angleTol;
        case DISTRATIO: return PtoInt(distRatio);
        case POLYTOL: return polyTol;
        case ASPECTRATIO: return aspectRatio;
        case SIZERATIO: return PtoInt(sizeRatio);
        case RATIOTOL: return PtoInt(ratioTol);
        case ETOL1: return etol1;
        case ETOL2: return etol2;
        case ESIZE: return eSize;
        case METHOD: return sel;
        case BSIZE: return bSize;
        case BSIGMA: return bSigma;
        default: return -1;
    }
}

Mat Capture::getEdges() {
#ifdef TEST
    cout << "Running Capture::getEdges..." << endl;
#endif
    if (edges.empty()) {
        cout << "getEdges: empty..." << endl;
        edges = edgesCanny(&frame, etol1, etol2, eSize, bSize, bSigma, false);
    }
    return edges;
}

Cnts Capture::getPolys() {
#ifdef TEST
    cout << "Running Capture::getPolys..." << endl;
#endif
    if(edges.empty()) {getEdges();}
    if (polys.empty() && !edges.empty()) {
        polys = findPolys(&edges, distRatio);
    }
    return polys;
}

Fps Capture::getFps() {
#ifdef TEST
    cout << "Running Capture::getFps..." << endl;
#endif
    if (polys.empty()) {getPolys();}
    if (fps.empty() && !polys.empty()) {
        fps = findFocusPoints(polys, angleTol, distRatio);
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
        rects = hasRectangles(polys.contours, angleTol, distRatio);
    }
    return rects;
}

Point Capture::getRef() {
#ifdef TEST
    cout << "Running Capture::getRef..." << endl;
#endif
    if (ref == Point()) {getBorder();}
    return ref;
}

cnt Capture::getOutline() {
    if (outline.empty()) {
        double outlineArea = sizeRatio*totalArea;

        // W is the small length, L is the large length
        // Aspect Ratio assumed < 1
        double W = sqrt(outlineArea / aspectRatio);
        double L = W*aspectRatio;

	if (frame.rows > frame.cols) {double temp = W; W = L; L = temp;}

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
    cnt corners;
    switch (sel) {
    case fpcorners: corners = fpsBorder(); break;
    case strongborder: corners = strongBorder(); break;
    case regular: corners = regBorder(); break;
    case automatic: corners = autoBorder(); break;
    }
    if (corners.size() == 4) {
        Points center = corners;
        if (ref==Point()) {ref = calcRef(corners);};
        border = sortCorners(center, ref, distRatio);
    }
    return border;
}

cnt Capture::fpsBorder() {
#ifdef TEST
    cout << "Capture::getBorder: fpcorners..." << endl;
#endif
    cnt corners;
    if (fps.empty()) {getFps();}
    if (border.empty() && !fps.empty()) {
        vector<Fp> t = calcCorners(fps, angleTol, distRatio);
        corners = centroids(t);
        ref = calcRef(t).center;
    }
    return corners;
}

cnt Capture::strongBorder() {
#ifdef TEST
    cout << "Capture::getBorder: strongborder..." << endl;
#endif
    cnt corners; // out

    // Get Fps
    if (fps.empty()) {getFps();}

    // Get valid Rects
    vector<cnt> check;
    for (cnt r : (getRects())) {
        bool inside = fps.empty() || allInside(r,fps);
        bool size = contourArea(r) >= sizeRatio*totalArea;
        if (inside && size) {check.push_back(r);}
    }

    // Get pairs
    vector<vector<cnt>> pairs = findSimilar(check, distRatio);
    if (!pairs.empty()) {
        // represents each pair by its largest member
        vector<cnt> rep;
        for (unsigned int i = 0; i < pairs.size(); i++) {
            rep.push_back(largest(pairs[i]));
        }

        // the largest pair is our border.
        corners = largest(rep);
        ref = calcRef(fps).center;
    }

    return corners;
}

cnt Capture::regBorder() {
#ifdef TEST
    cout << "Capture::getBorder: regular..." << endl;
#endif
    cnt corners;
    vector<cnt> valid;
    bool size, inside, ratio;
    if (fps.empty()) {getFps();}
    for (cnt r : getRects()) {
        size = contourArea(r) > totalArea*sizeRatio;
        ratio = aspectRatio != 0 && isAspectRatio(r, aspectRatio, ratioTol);
        inside = (fps.empty() || allInside(r, fps));
        if (size && ratio && inside) {valid.push_back(r);}
    }
    if (!valid.empty()) {
        corners = largest(valid);
    }
    return corners;
}

cnt Capture::autoBorder() {
#ifdef TEST
    cout << "Capture::getBorder: automatic..." << endl;
#endif
    cnt corners = fpsBorder();
    if (corners.empty()) {corners = strongBorder();}
    if (corners.empty()) {corners = regBorder();}
    return corners;
}

// ----------- Draw Methods -------------
Mat Capture::drawInfo() {
    // Declare Variables
    Mat out = Mat::zeros(frame.rows, frame.cols, frame.type());

    drawPolys(out, white);  // Print all polys
    drawRects(out, red);    // Print Rectangles
    drawFps(out, green);    // Print Fps

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

Mat Capture::drawBorder(Mat img, Scalar color) {
    Mat out = img;
    vector<cnt> conts{getBorder()};
    drawContours(out, conts, 0, color, 2, 8);
    return out;
}

Mat Capture::drawOutline(Mat img, Scalar color) {
    Mat out = img;
    vector<cnt> conts{getOutline()};
    drawContours(out, conts, 0, color, 2, 8);
    return out;
}


vector<Mat> Capture::process() {
#ifdef TEST
    cout << "Running Capture::process..." << endl;
#endif
    // Variable Declaration
    Mat warp; Mat drawing = frame.clone();
    vector<Mat> out;

    if (border.empty()) {getBorder();}
    drawFps(drawing,blue);
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

vector<Mat> Capture::getQr() {
    //Save and change aspect ratio
    double save = aspectRatio;
    aspectRatio = 1.0;

    Fps fs = getFps();
    vector<cnt> borders = getQRBorders(list<Fp>(fs.begin(),fs.end()));

    // Convert Triangular borders to Square and Warp
    vector<Mat> out;
    for (unsigned int i = 0; i < borders.size(); i++) {
        cnt b = borders[i];
        Point r = b[1];

        //Reflection about a line
        //Source: http://stackoverflow.com/questions/3306838/algorithm-for-reflecting-a-point-across-a-line
        double m = ((b[0].y)-(b[2].y))/((b[0].x)-(b[2].x)); // Slope
        double c = b[0].y-m*b[0].x;                         // Intercept

        double d = (b[1].x + (b[1].y - c)*m)/(1 + pow(m,2));
        double newX = 2*d - b[1].x;
        double newY = 2*d*m - b[1].y + 2*c;

        cnt square = cnt{b[0],b[1],b[2],Point((int)newX,(int)newY)};
        cnt outline = findSimilar(square,getPolys().contours,distRatio);

        //Get the matrix
        Mat temp = frame.clone();
        fixPerspective(&temp, outline, r);
        out.push_back(temp);
    }
    aspectRatio = save; //Set it back
    return out;
}

vector<cnt> Capture::getQRBorders(list<Fp> fs) {
    vector<cnt> out;
    bool end = false;

    //While there are 3 fps still availible for calculation
    while (fs.size() >= 3 or end) {
        bool reset = false;

        // Iterate through sets of three Fp's
        for (_List_iterator<Fp> i = fs.begin(); i != fs.end() && !reset; ++i) {
        for (_List_iterator<Fp> j = fs.begin(); j != fs.end() && !reset; ++j) {
        for (_List_iterator<Fp> k = fs.begin(); k != fs.end() && !reset; ++k) {
        if (i!=j && j!=k && k!=i) {
            Point x = (*i).center; Point y = (*j).center; Point z = (*k).center;
            cnt b = isQR(x,y,z);
            if (!b.empty()) {
                out.push_back(b);
                fs.erase(i);
                fs.erase(j);
                fs.erase(k);
                reset = true;
            }
        }}}}

        if (!reset){end = true;}
    }

    return out;
}

cnt Capture::isQR(Point a, Point b, Point c) {
    cnt a90 = anyAng(a,b,c,90.0,angleTol); //Get the corner that equals 90 degrees
    if (!a90.empty()) {
        //Check if the two edges from the angle are the same length.
        if(tolEq(dist(a90[1],a90[0]),dist(a90[1],a90[2]),distRatio)) {
            return a90;
        }
    }
    return cnt();
}

// bool Capture::validRect(cnt r) {
// #ifdef TEST
//     cout << "Capture::getBorder: validRect... " << endl;
// #endif
//     double area = contourArea(r);
//     bool size = area > totalArea*sizeRatio;
//     bool ratio = isAspectRatio(r, aspectRatio, ratioTol);
//     bool inside = ((fps).size()==0 || allInside(r, fps));
// #ifdef TEST
//     cout << "Capture::getBorder: validRect = " << size << ratio << inside << " " << area1 << "<" << area2 << endl;
// #endif
//     return size && ratio;
// }
