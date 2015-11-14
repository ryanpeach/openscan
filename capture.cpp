/**
 * capture.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "capture.hpp"
#define DESKTOP
#define TEST

// Uses polyTol, angleTol, distTol, wSize, C;
vector<Mat> Capture::process(Mat img, bool filter) {
#ifdef TEST
    cout << "Running Capture::process..." << endl;
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

    // Get border from focus points
    vector<Fp> corners = getCorners(fps, angleTol, distTol);
    if (corners.size() == 4) {
        Fp ref = getRef(fps);
        warp = fixPerspective(img, corners, ref);
        if (filter) {filtered = outputFilter(warp, wSize, C);}
    } else {return vector<Mat>();}

    cvtColor(warp, warp, COLOR_GRAY2RGB);
    Scalar color = Scalar(255, 0, 0);
    Mat drawing = warp;
    drawContours(drawing, centroids(corners), 0, color, 3, 8);
    return {drawing, warp, filtered};
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
void Capture::webCam(char *avifile) {
#ifdef TEST
    cout << "Running Capture::webCam..." << endl;
#endif
    VideoCapture cap;
    Mat frame, preview, drawing, cropped;
    vector<Mat> proc;
    string filename,filepath;

    cout << "here1" <<endl;

    bool found = false;

    if(avifile == NULL )
	{
	    	if(!cap.open(0))
		{
		cout << "Camera failed to open..." << endl;
		return;
		}
	}
	else
	{
		if (!cap.open(avifile))
    		{
        	std::cout << "!!! Failed to open file: " << avifile << std::endl;
        	return ;
    		}
	}
    

    namedWindow("Capture:Press and Hold 'q' to exit", WINDOW_NORMAL);
    namedWindow("Preview: Press 's' to save.", WINDOW_NORMAL);

#ifdef TEST
    cout << "webCam: Beginning Main Loop..." << endl;
#endif

    for (;;) {
        cap >> frame;
        if ( frame.empty() ) {break;}  // end of video stream
        vector<Mat> proc = process(frame);

#ifdef TEST
        cout << "webCam: Process Complete!" << endl;
#endif

        if (!proc.empty()) {
            drawing = proc[0];
            cropped = proc[1];
            preview = proc[2];
            found = true;
        } else {
            drawing = frame;
            found = false;
        }

        // Show and save webcam out and preview
        imshow("Capture:Press and Hold 'q' to exit", drawing);
        if (found) {
            imshow("Capture:Press and Hold 's' to save", preview);
            if (cvWaitKey(10) == 's') {                 
	        // save
                filename = std::tmpnam(nullptr); 
                filepath = "scans/" + filename + ".jpg";
                imwrite(filepath, preview);
#ifdef TEST
                cout << "webCam: Saved as: " << filepath << endl;
#endif
            }
        }

        // Quit
        if (cvWaitKey(10) == 'q') {break;}
    }
    cap.release();
    destroyAllWindows();
}
#endif
