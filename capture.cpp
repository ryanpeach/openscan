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
        edges = importFilter(img, etol1, etol2, eSize);
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
    vector<cnt> rects = hasRectangles(polys);
    vector<cnt> check;
    for (cnt r : rects) {
        if (contourArea(r) > sizeRatio*M.cols*M.rows
                && isAspectRatio(r, aspectRatio)) {
            if (fps.size()==0 || allInside(r, fps)) {
                check.push_back(sortCorners(r));
            }
        }
    }

    // Find any two contours who share similar corners
    vector<cnt> pair; vector<cnt> out;
    for (int r1 = 0; r1 < check.size(); r1++) {
        for (int r2 = 0; r2 < check.size(); r2++) {
            bool found = true;
            for (int i = 0; i < 4 && r1 != r2
                && !contains(pairs, vector<vector<int>> pairs); i++) {
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
    warp = fixPerspective(img, largest(out));
    drawContours(drawing, vector<cnt>{centroids(corners)}, 0, color, 3, 8);
    vector<Mat> end = vector<Mat>{drawing, warp};
    return end;

    return vector<Mat>{};
}
