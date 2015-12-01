#import "tests.hpp"

void imageProcess (Mat frame, Capture* c) {
    // Variable Declaration
    Mat drawing, preview;
    string filename,filepath;

    // Create Windows
    vector<Mat*> images = vector<Mat*>{&drawing, &preview};
    vector<string> names = vector<string>{
        "Frame: Press 'q' to exit.",
        "Preview: Press 's' to save."};
    WindowManager win = WindowManager(images, names);

    vector<Mat> proc = c->process();

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

void imageFile (char *filepath, Capture* c) {
    Mat cap = imread(filepath);
    if (!cap.empty()) {
        std::cout << "!!! Failed to open file: " << filepath << std::endl;
        return;
    }
    imageProcess(cap, c);
}
