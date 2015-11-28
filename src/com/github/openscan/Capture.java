package com.github.openscan;

import org.opencv.core.Mat;

public class Capture {
	private long ptr_;
	
    private native long createCapture();
    private native void destroyCapture(long ptr);
    private native void setFrame(long ptr, Mat frame);
    private native long runProcess(long ptr);
    private native Mat getMat(long ptr);
    
    private native void setValue(long ptr, int param, double value);
    
    static {
    	System.loadLibrary("CaptureJNI");
    }
    
    public enum Param {
    	ANGLETOL, DISTTOL, POLYTOL, ASPECTRATIO, SIZERATIO, RATIOTOL, ETOL1, ETOL2, ESIZE, METHOD;
    }
    
    public Capture() {
    	ptr_ = createCapture();
    }
    
    public void destroy() {
    	destroyCapture(ptr_);
    }
    
    public void Frame(Mat frame) {
    	setFrame(ptr_, frame);
    }
    
    public Mat[] process() {
    	long matptr = runProcess(ptr_);

    	Mat[] out = [getMat(matptr),getMat(matptr+1)];
    	return out;
    }
    
    public void setValue(Param param, double value) {
    	setValue(ptr_, (int)param, value);
    }
}
