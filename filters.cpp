#include "filters.hpp"

Mat toGray(Mat img) {
#ifdef TEST
    cout << "Running toGray..." << endl;
#endif
	Mat gray;
	if (isColor(&img)) {cvtColor(img, gray, COLOR_RGB2GRAY);}
	else {gray = (img).clone();}
	return gray;
}

Mat toColor(Mat img) {
#ifdef TEST
    cout << "Running toColor..." << endl;
#endif
	Mat out;
	if (!isColor(&img)) {cvtColor(img, out, COLOR_GRAY2RGB);}
	else {out = (img).clone();}
	return out;
}

void Gray(Mat* img) {
#ifdef TEST
    cout << "Running Gray..." << endl;
#endif
	if (isColor(img)) {cvtColor(*img, *img, COLOR_RGB2GRAY);}
}

void Color(Mat* img) {
#ifdef TEST
    cout << "Running Color..." << endl;
#endif
	if (!isColor(img)) {cvtColor(*img, *img, COLOR_GRAY2RGB);}
}

Mat edgesCanny(Mat* img, int tol1, int tol2, int wSize, int bSize, double bSigma, bool l2grad) {
#ifdef TEST
    cout << "Running edgesCanny..." << endl;
#endif
	Mat edges, blur;
	Mat gray = toGray(*img);
	Odd(&bSize);
	GaussianBlur(gray, blur, Size(bSize,bSize), bSigma, 0);
	Canny(blur, edges, tol1, tol2, wSize, l2grad);
	return edges;
}

Mat adaptiveGaussian(Mat* img, int wSize, int C) {
#ifdef TEST
    cout << "Running outputFilter..." << endl;
#endif
    // Testing & Declarations
    Mat out;
    Odd(&wSize);

    // Convert to gray if not already
    Mat gray = toGray(*img);

    cout << "here" << endl;
    adaptiveThreshold(gray, out, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, wSize, C);
    return out;
}
