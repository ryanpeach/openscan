#include "opencv2/core.hpp"
#include <vector>

using namespace std;
using namespace cv;

long createCapture() {
	Capture out = new Capture();
	return &out;
}

void destroyCapture(long ptr) {
	delete[] (Capture*)ptr;
}

void setFrame(long ptr, Mat frame) {
	Capture *c = ((Capture*)ptr);
	c->Frame(frame);
}

long runProcess(long ptr) {
	Capture *c = ((Capture*)ptr);
	vector<Mat> outV = new vector<Mat>(c->process());
        if (outV.empty()) {return -1;}
	return &(outV[0]);
}

Mat getMat(long ptr) {
	return *((Mat*)(ptr));
}

void setValue(long ptr, int param, double value) {
	Capture *c = ((Capture*)ptr);
	c->setValue((Capture::Param)param,value);
}
