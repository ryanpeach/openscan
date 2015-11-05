/**
 * capture.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "capture.hpp"

vector<Mat> Capture::process(Mat img) {
    // Define Variables
    vector<Fp> corners, sort; Fp ref; Mat warp, crop, out;

    // Intial Processing
    polys = findPolys(img);
    fps = findFocusPoints(polys);

    // Get border from focus points
    vector<Fp> corners = getCorners(fps, angleTol);
    if (corners.size() == 4) {
        Fp ref = getRef(corners);
        vector<Fp> sort = sortCorners(corners, ref);
        warp = fixPerspective(img, sort, ref);
        crop = cropImage(warp, R*warp.cols);
        out = outputFilter(crop, wSize, C);
    } else {return {img, img};} //future revision: return process2(img);

    Mat drawing = drawContour(img, sort, color = {255,0,0}, thickness = 1);
    return {cvtColor(out, out, COLOR_GRAY2RGB), drawing};
}

// Processes the image without the help of corner focus points
// Null-Condition: Returns {null,img}
vector<Mat> Capture::process2(Mat img) {
    // Get the largest rectangular border from polys which contains all focus points.
    // If no focus points exist, then simply return the largest border.
    cnt largest; int size = 0;
    for (int i = 0; i < polys.size(); i++) {
        if (isRectangle(polys[i]) && contourArea(polys[i]) > size) {
            if ((fps.size() != 0 && allInside(polys[i],fps)) || fps.size() == 0) {
                largest = polys[i]; size = contourArea(polys[i]);
            }
        }
    }

    // If one is found, return the processed image. If exception or none found, return null.
    if (size > 0) {
        Point ref = getRef(largest);
        vector<Fp> sort = sortCorners(largest, ref);
        Mat warp = fixPerspective(img, sort, ref, aspectRatio);
        Mat crop = cropImage(warp, R*warp.cols);
        Mat out = outputFilter(crop, wSize, C);

        // Draw and return
        Mat drawing = drawContour(img, largest, color = {255,0,0}, thickness = 1);
        return {cvtColor(out, out, COLOR_GRAY2RGB), drawing};
    }
    else {return {img, img};}
}
