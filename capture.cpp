/**
 * capture.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "capture.hpp"
#define TEST

// Uses polyTol, angleTol, distTol, wSize, C;
vector<Mat> Capture::focusPointCorners(Mat img) {
#ifdef TEST
    cout << "Running Capture::focusPointCorners..." << endl;
#endif
    // Variable Declaration
    Mat warp, edges, filtered;

    // Intial Processing
    edges = importFilter(img, etol1, etol2, eSize);
#ifdef TEST
    imshow("Canny",edges);
#endif
    polys = findPolys(edges, polyTol);
    fps = findFocusPoints(polys, angleTol, distTol);
    cout << "Fp's Found: " << fps.size() << endl;
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
    Mat drawing = warp;
    drawContours(drawing, vector<cnt>{centroids(corners)}, 0, color, 3, 8);
    vector<Mat> end = vector<Mat>{drawing, warp, filtered};
    cout << "end" << endl;
    return end;
}



// Processes the image without the help of corner focus points
// Null-Condition: Returns {null,img}
// vector<Mat> Capture::process2(Mat img) {
//    // Get the largest rectangular border from polys which contains all focus points.
//    // If no focus points exist, then simply return the largest border.
//    cnt largest; int size = 0;
//    for (int i = 0; i < polys.size(); i++) {
//        if (isRectangle(polys[i]) && contourArea(polys[i]) > size) {
//            if ((fps.size() != 0 && allInside(polys[i],fps)) || fps.size() == 0) {
//                largest = polys[i]; size = contourArea(polys[i]);
//            }
//        }
//    }
//
//    // If one is found, return the processed image. If exception or none found, return null.
//    if (size > 0) {
//        Point ref = getRef(largest);
//        vector<Fp> sort = sortCorners(largest, ref);
//        Mat warp = fixPerspective(img, sort, ref, aspectRatio);
//        Mat crop = cropImage(warp, R*warp.cols);
//        Mat out = outputFilter(crop, wSize, C);
//
//        // Draw and return
//        Mat drawing = drawContour(img, largest, color = {255,0,0}, thickness = 1);
//        return {cvtColor(out, out, COLOR_GRAY2RGB), drawing};
//    }
//    else {return {img, img};}
// }
