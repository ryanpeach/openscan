/**
 * main.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Ryan Peach
 */

#include<string>
#include "cvmethods.hpp"
#include "capture.hpp"

#define DESKTOP
#define TEST


#ifdef DESKTOP

// -------------- Window Manager Class -------------

class WindowManager {
 private:
    vector<Mat*> OUT;
    vector<string> NAMES;
 public:
    WindowManager(vector<Mat*> out, vector<string> names) : OUT(out), NAMES(names) {
        for (string n : names) {
            namedWindow(n, WINDOW_NORMAL);
        }
    }
    void update() {
        for (unsigned int i = 0; i < OUT.size(); i++) {
            if(!OUT[i]==nullptr && !(*OUT[i]).empty()) {imshow(NAMES[i], *(OUT[i]));}
            else if (OUT[i]==nullptr) {imshow(NAMES[i],Mat());}
        }
    }
    void close() {
        destroyAllWindows();
    }
};

Capture C;
double angleTol, distTol, polyTol;
double angleTol_max = 360;
double distTol_max = 100;
double polyTol_max = 100;
double sizeRatio, ratioTol;
double sizeRatio_max = 1.0;
double ratioTol_max = .1;

function<void (int, void*)> on_valueChange(Param sel, double param) {
    return [](int, void*) {
        C.setValue(param, angleTol);
    };
}


// ------ Video and Image Processing Methods ---------------

void imageProcess (Mat frame, Capture c) {
    // Variable Declaration
    Mat drawing, preview;
    string filename,filepath;

    // Create Windows
    vector<Mat*> images = vector<Mat*>{&drawing, &preview};
    vector<string> names = vector<string>{
        "Frame: Press 'q' to exit.",
        "Preview: Press 's' to save."};
    WindowManager win = WindowManager(images, names);

    vector<Mat> proc = c.process();

    // Save processed frame to appropriate outputs
    if (!proc.empty()) {
        drawing = proc[0];
        preview = proc[1];
    } else {
        cout << "No scan found." << endl;
        win.close();
        return;
    }

    // Show frame out and preview
    win.update();

    for (;;) {
        // Save File
        if (cvWaitKey(10) == 's') {
            filename = std::tmpnam(NULL);
            filepath = "scans/" + filename + ".jpg";
            imwrite(filepath, preview);
#ifdef TEST
            cout << "webCam: Saved as: " << filepath << endl;
#endif
        }

        // Quit
        if (cvWaitKey(10) == 'q') {break;}
    }
    win.close();
}

void videoProcess(VideoCapture cap, Capture c) {
#ifdef TEST
    cout << "Running Capture::webCam..." << endl;
#endif
    // Variable Declaration
    Mat drawing, preview;
    vector<Mat> proc;
    string filename, filepath;
    bool found = false; bool saved = false;

    // Create Windows
    vector<Mat*> images = vector<Mat*>{&drawing, &preview, nullptr};
    vector<string> names = vector<string>{
        "Frame: Press 'q' to exit.",
        "Preview: Press 's' to save."
        "Settings"};
    WindowManager win = WindowManager(images, names);

    if(!cap.isOpened()){  // check if we succeeded
        cout << "Camera failed to open!" << endl;
	return;
    }

    //Create Trackbars
    int N = 5;
    String[N] trackbarNames = {"angleTol", "distTol", "polyTol", "sizeRatio", "ratioTol"};
    double*[N] trackbarValues = {&angleTol, &distTol, &polyTol, &sizeRatio, &ratioTol};
    double[N] trackbarMax = {angleTol_max, distTol_max, polyTol_max, sizeRatio_max, ratioTol_max};
    Params[N] params = {ANGLETOL, DISTTOL, POLYTOL, SIZERATIO, RATIOTOL};
    for (int i = 0; i < N; i++) {
        createTrackbar(trackbarNames[i], "Settings", trackbarValues[i], trackbarMax[i], 
                onValueChange(params[i],trackbarValues[i]);
    }

#ifdef TEST
    cout << "Video: Beginning Main Loop..." << endl;
#endif

    for (;;) {
        // Process Frame
    	Mat frame;
        cap >> frame;
        if ( frame.empty() ) {break;}  // end of video stream
        c.Frame(frame);
        proc = c.process();

#ifdef TEST
        cout << "webCam: Process Complete!" << endl;
#endif

        // Save processed frame to appropriate outputs
        if (!proc.empty()) {
            drawing = proc[0];
            preview = proc[1];
            found = true;
            saved = false;
        } else {
            drawing = frame;
        }

        // Show frame out and preview
        win.update();

        // Save File
        if (cvWaitKey(10) == 's' && found && !saved) {
            filename = std::tmpnam(NULL);
            filepath = "scans/" + filename + ".jpg";
            imwrite(filepath, preview);
            saved = true;
#ifdef TEST
            cout << "webCam: Saved as: " << filepath << endl;
#endif
        }

        // Quit
        if (cvWaitKey(10) == 'q') {break;}
    }
    cap.release();
    win.close();
}

// ---------- Derivative Video Methods -------------
void webCam (Capture c) {
    VideoCapture cap(0);
    if(!cap.open(0)) {
        cout << "Camera failed to open..." << endl;
        return;
    }
    videoProcess(cap, c);
}

void videoFile (char *filepath, Capture c) {
    VideoCapture cap(filepath);
    if (!cap.open(filepath)) {
        std::cout << "!!! Failed to open file: " << filepath << std::endl;
        return;
    }
    videoProcess(cap, c);
}

void imageFile (char *filepath, Capture c) {
    Mat cap = imread(filepath);
    if (!cap.empty()) {
        std::cout << "!!! Failed to open file: " << filepath << std::endl;
        return;
    }
    imageProcess(cap, c);
}

#endif

// ---------------- Test Methods -----------------
#ifdef TEST
void testGeometry() {
    cout << "running testGeometry" << endl;
    cout << "Creating Variables" << endl;

    // Variable Declarations
    // Points
    const Point a = Point(0, 0), b = Point(100, 100), c = Point(0, 100);
    const Point e = Point(200, 0), d = Point(100, 0), f = Point(100, 200);
    // Poly's
    const cnt tri {a, e, b};  // non-equal sides
    const cnt square {a, c, b, d};
    const cnt rect {a, c*2, f, d};
    const cnt bigSquare {a*2-b, c*2-b, b*2-b, d*2-b};
    // Fp's
    const Fp testFp1 = Fp({bigSquare, square});
    const Fp testFp2 = Fp({bigSquare});  // not a valid Fp
    const Fp testFp3 = Fp({square});  // not a valid Fp

    cout << "Creating Vectors" << endl;

    // Vectors
    // Poly's
    const vector<cnt> vpoly1 {tri, square, bigSquare};
    const vector<cnt> vpoly2 {tri, square};
    const vector<cnt> vpoly3 {square, bigSquare};
    const vector<cnt> vpoly4 {tri, bigSquare};
    const vector<cnt> vpoly5 {tri};
    const vector<cnt> vpoly6 {square};
    const vector<cnt> vpoly7 {bigSquare};
    // Fp's
    const vector<Fp> vfp1 {testFp1, testFp2, testFp3};
    const vector<Fp> vfp2 {testFp1, testFp2};
    const vector<Fp> vfp3 {testFp2, testFp3};
    const vector<Fp> vfp4 {testFp1, testFp3};
    const vector<Fp> vfp5 {testFp1};
    const vector<Fp> vfp6 {testFp2};
    const vector<Fp> vfp7 {testFp3};

    cout << "Test Methods..." << endl;

    // Test Methods
    // Centroid
    Point tric = centroid(tri);
    Point squarec = centroid(square);
    Point bigsc = centroid(bigSquare);
    cout << "Triangle" << tostr(tri) << tostr(tric) << endl;
    cout << "Square" << tostr(square) << tostr(squarec) << endl;
    cout << "Big Square" << tostr(bigSquare) << tostr(bigsc) << endl;
    cout << "Square & BigSquare Centroid: " << tostr(centroid(vpoly3)) << endl;

    // Dist & Angs
    vector<double> dts = dists(tri);
    vector<double> ans = angles(tri);
    cout << "Triangle lengths: " << vtostr(dts) << endl;
    cout << "Triangle angles: " << vtostr(ans) << endl;
    cout << "Unit Angle: " << "Origin - " << tostr(a) << "; Point - " << tostr(b) << "; Ang - " << angle(a,b) << endl;

    // All same length
    cout << "Triangle all same length? " << allSameLength(tri, (double)0.0) << endl;
    cout << "Square all same length? " << allSameLength(square, (double)0.0) << endl;

    // isPoly
    bool test1 = isPoly(tri, 3, false, false, 0, 0);  // True
    bool test2 = isPoly(tri, 3, true, true, 0, 0);  // False
    bool test3 = isPoly(tri, 4, false, false, 0, 0);  // False
    bool test4 = isRectangle(rect, false, 0, 0);  // True
    bool test5 = isRectangle(rect, true, 0, 0);  // False
    bool test6 = isSquare(square, 0, 0);  // True
    cout << test1 << test2 << test3 << test4 << test5 << test6 << endl;
}
#endif

// ------------------ Main Method --------------


int main(int argc,char *argv[]) {
#ifdef TEST
    cout << "Running main..." << endl;
    testGeometry();
#endif
    if (argc == 1) {
        webCam(C);
    } else if (argc == 2 && *argv[1] == '-' && *argv[2] == 'h') {
        cout << " Usage : " << argv[0] << " " << "filename[optional]" <<endl;
        cout << "Use an avi file as an argument to take input from avi file." << endl;
        cout << "If no argument is specified the input is taken from the webcam"<<endl;
    } else if (argc == 2 && *argv[1] == '-' && *argv[2] == 'v') {
        videoFile(argv[3], C);
    } else if (argc == 2 && *argv[1] == '-' && *argv[2] == 'i') {
        imageFile(argv[3], C);
    } else {
        cout << " Usage : " << argv[0] << " " << "filename[optional]" <<endl;
        cout << "Use an avi file as an argument to take input from avi file." << endl;
        cout << "If no argument is specified the input is taken from the webcam"<<endl;
    }
}
