/**
 * capture.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "capture.hpp"
#define TEST

void Capture::preProcess(Mat img) {
    if (!matEq(img, frame)) {
        edges = edgesCanny(img,etol1,etol2,eSize);
        polys = findPolys(edges, polyTol);
        fps = findFocusPoints(polys, angleTol, distTol);
        frame = img;
    }
}

// Uses polyTol, angleTol, distTol, wSize, C;
vector<Mat> Capture::focusPointCorners(Mat img) {
#ifdef TEST
    cout << "Running Capture::focusPointCorners..." << endl;
#endif

    Mat warp, drawing;  // Variable Declaration

    preProcess(img);  // Intial Processing

#ifdef TEST
    imshow("Canny",edges);
    cout << "Fp's Found: " << fps.size() << endl;
#endif

    // Get border from focus points
    vector<Fp> corners = getCorners(fps, angleTol, distTol);
    if (corners.size() == 4) {
        Fp ref = getRef(fps);
        if (ref.contours.size()>0){
            warp = fixPerspective(img, corners, ref);
        }
        else {return vector<Mat>();}
    } else {return vector<Mat>();}

    if(!isColor(warp)) {cvtColor(warp, warp, COLOR_GRAY2RGB);}
    Scalar color = Scalar(255, 0, 0);
    drawing = warp;

    drawContours(drawing, vector<cnt>{centroids(corners)}, 0, color, 3, 8);
    vector<Mat> end = vector<Mat>{drawing, warp};
    cout << "end" << endl;
    return end;
}



vector<Mat> Capture::strongPageBorder(Mat img) {
#ifdef TEST
    cout << "Running Capture::focusPointCorners..." << endl;
#endif

    Mat warp, drawing;  // Variable Declaration

    preProcess(img);  // Intial Processing

#ifdef TEST
    imshow("Canny",edges);
#endif

    // Find only big rectangles
    // Of the proper aspect ratio
    // And which contain all focus points
    vector<cnt> rects = hasRectangles(polys.contours, angleTol, distTol);
    vector<cnt> check;
    for (cnt r : rects) {
        if (contourArea(r) >= sizeRatio*img.cols*img.rows
                && isAspectRatio(r, aspectRatio, ratioTol)) {
            if (fps.size()==0 || allInside(r, fps)) {
                check.push_back(sortCorners(r));
            }
        }
    }

    // Find any two contours who share similar corners
    vector<cnt> pair; vector<cnt> out;
    for (unsigned int r1 = 0; r1 < check.size(); r1++) {
        for (unsigned int r2 = 0; r2 < check.size(); r2++) {
            bool found = true;
            for (unsigned int i = 0; i < 4 && r2 > r1; i++) {  //No duplicates
                if (!(dist(check[r1][i], check[r2][i]) <= distTol)) {
                    found = false;
                }
                if (found) {
                    out.push_back(check[r1]);
                }
            }
        }
    }

    // Main processing and return
    auto color = Scalar{0,0,255};
    auto border = largest(out);
    auto ref = getRef(border);
    warp = fixPerspective(img, border, ref);
    drawContours(drawing, border, 0, color, 3, 8);
    vector<Mat> end = vector<Mat>{drawing, warp};
    return end;
}
