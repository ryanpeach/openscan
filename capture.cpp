/**
 * capture.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#define DESKTOP

#include "capture.hpp"

//Uses polyTol, angleTol, distTol, wSize, C;
vector<Mat> Capture::process(Mat img, bool filter) {
    // Variable Declaration
    Mat warp; frame = img;
    
    // Intial Processing
    polys = findPolys(img, polyTol);
    fps = findFocusPoints(polys, angleTol, distTol);

    // Get border from focus points
    vector<Fp> corners = getCorners(fps, angleTol, distTol);
    if (corners.size() == 4) {
        Fp ref = getRef(fps);
        warp = fixPerspective(img, corners, ref);
        if (filter) {warp = outputFilter(warp, wSize, C);}
    } else {return {img, img};}

    cvtColor(warp, warp, COLOR_GRAY2RGB);
    Scalar color = Scalar(255,0,0);
    Mat drawing = warp;
    drawContours(drawing, centroids(corners), 0, color, 3, 8);
    return {warp, drawing};
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

#ifdef DESKTOP
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
        if (!(matEq(preview[0],frame) && matEq(preview[1],frame)))
            imshow("Preview", preview);
        imshow("Video", frame);
        if( waitKey(1) == 27 ) break; // stop capturing by pressing ESC 
    }
}
#endif
