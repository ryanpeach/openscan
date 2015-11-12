/**
 * capture.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "capture.hpp"

vector<Mat> Capture::process(Mat img) {
    // Variable Declaration
    Mat warp, crop;
    Mat out;
    
    // Intial Processing
    polys = findPolys(img, Capture::polyTol);
    fps = findFocusPoints(polys);

    // Get border from focus points
    vector<Fp> corners = getCorners(fps, Capture::angleTol, Capture::distTol);
    if (corners.size() == 4) {
        Fp ref = getRef(fps);
        warp = fixPerspective(img, corners, ref);
        out = outputFilter(crop, wSize, C);
    } else {return {img, img};}

    Mat drawing = drawContour(img, sort, {255,0,0}, 1);
    cvtColor(out, out, COLOR_GRAY2RGB);
    return {out, drawing};
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

void Capture::webCam() {
    VideoCapture cap;
    Mat frame;
    vector<Mat> preview;
    if(!cap.open(0))
        return;
    for(;;)
    {
        cap >> frame;
        if( frame.empty() ) break; // end of video stream
        preview = process(frame);
        if (!(bool)(preview == vector<Mat> {frame, frame}))
            imshow("Preview", preview);
        imshow("Video", frame);
        if( waitKey(1) == 27 ) break; // stop capturing by pressing ESC 
    }
}
