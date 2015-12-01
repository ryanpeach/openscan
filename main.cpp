/**
 * main.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Ryan Peach
 */

#include "tests.hpp"

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
            if(OUT[i]!=nullptr && !(*OUT[i]).empty()) {imshow(NAMES[i], *(OUT[i]));}
            else if (OUT[i]==nullptr) {imshow(NAMES[i],Mat());}
        }
    }
    void close() {
        destroyAllWindows();
    }
};

int toInt(double v) {return (int)(v*VSCALE);}

// ------ Video and Image Processing Methods ---------------

// Global Variables
// Note: Main should use pointers to communicate data references,
// These variables are global only that we can ensure the reference is persistent.
struct udata {
    Capture *capt;
    Par param;
} token;

Capture C;
int etol1, etol2, eSize, polyTol;
int cBlock, cSize, k, cThresh;

// Sets the value of a variable on a change of trackbar value
void setTrackBar(int v, void* userdata){
#ifdef TEST
    cout << "Running setTrackBar..." << endl;
#endif
    udata t = *((udata*)(userdata));
#ifdef TEST
    cout << t.param << " " << v << endl;
#endif
    (t.capt)->setValue((t.param), v);
}

//This is complicated, these pointers are passed via the last pointer in createTracker as void* in setTrackBar
//Then they are unpacked to show the capture method and the selected parameter
void createTrackbar(String vname, String wname, int* v, int maxv, Capture* c, Par param) {
    token = {c, param}; 
    createTrackbar(vname, wname, v, maxv, &setTrackBar, &token);
}

void videoProcess(VideoCapture cap, Capture* c) {
#ifdef TEST
    cout << "Running Capture::webCam..." << endl;
#endif
    // Variable Declaration
    Mat drawing, preview, canny, info;
    vector<Mat> proc;
    string filename, filepath;
    bool found = false; bool saved = false;

    // Create Windows
    vector<Mat*> images = vector<Mat*>{&drawing, &preview, &canny, &info};
    vector<string> names = vector<string>{
        "Frame: Press 'q' to exit.",
        "Preview: Press 's' to save.",
        "Canny Edge Detection",
        "Polys, Rects, & Fps"};
    WindowManager win = WindowManager(images, names);

    if(!cap.isOpened()){  // check if we succeeded
        cout << "Camera failed to open!" << endl;
        return;
    }

    //Create Trackbars
    //Canny
    createTrackbar("eTol1", "Canny Edge Detection", &etol1, 255, c, ETOL1);
    createTrackbar("eTol2", "Canny Edge Detection", &etol2, 255, c, ETOL2); 
    createTrackbar("eSize", "Canny Edge Detection", &eSize, 21, c, ESIZE);

    //Poly Approx
    createTrackbar("polyTol", "Polys, Rects, & Fps", &polyTol, 200, c, POLYTOL);

    //Corners
    createTrackbar("cBlock", "Polys, Rects, & Fps", &cBlock, 21, c, CBLOCK);
    createTrackbar("cSize", "Polys, Rects, & Fps", &cSize, 21, c, CSIZE);
    createTrackbar("k", "Polys, Rects, & Fps", &k, 21, c, K);
    createTrackbar("cThresh", "Polys, Rects, & Fps", &cThresh, 255, c, CTHRESH);


#ifdef TEST
    cout << "Video: Beginning Main Loop..." << endl;
#endif

    for (;;) {
        // Process Frame
    	Mat frame;
        cap >> frame;
        if ( frame.empty() ) {break;}  // end of video stream
        c->Frame(frame);
        proc = c->process();

#ifdef TEST
        cout << "webCam: Process Complete!" << endl;
#endif

        // Save processed frame to appropriate outputs
        if (proc.size()!=1) {
            drawing = proc[0];
            preview = proc[1];
            found = true;
            saved = false;
        } else {
            drawing = proc[0];
        }
        canny = c->drawEdges();
        info = c->drawInfo();

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
void webCam (Capture* c) {
    VideoCapture cap(0);
    if(!cap.open(0)) {
        cout << "Camera failed to open..." << endl;
        return;
    }
    videoProcess(cap, c);
}

void videoFile (char* filepath, Capture* c) {
    VideoCapture cap(filepath);
    if (!cap.open(filepath)) {
        std::cout << "!!! Failed to open file: " << filepath << std::endl;
        return;
    }
    videoProcess(cap, c);
}

// ------------------ Main Method --------------

int main(int argc,char *argv[]) {
#ifdef TEST
    cout << "Running main..." << endl;
    testGeometry();
#endif
    if (argc == 1) {
        webCam(&C);
    } else if (argc == 2 && *argv[1] == '-' && *argv[2] == 'h') {
        cout << " Usage : " << argv[0] << " " << "filename[optional]" <<endl;
        cout << "Use an avi file as an argument to take input from avi file." << endl;
        cout << "If no argument is specified the input is taken from the webcam"<<endl;
    } else if (argc == 2 && *argv[1] == '-' && *argv[2] == 'v') {
        videoFile(argv[3], &C);
    } else if (argc == 2 && *argv[1] == '-' && *argv[2] == 'i') {
        //imageFile(argv[3], &C);
    } else {
        cout << " Usage : " << argv[0] << " " << "filename[optional]" <<endl;
        cout << "Use an avi file as an argument to take input from avi file." << endl;
        cout << "If no argument is specified the input is taken from the webcam"<<endl;
    }
}

#endif
