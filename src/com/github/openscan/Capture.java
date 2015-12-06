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
    	ANGLETOL(0), DISTTOL(1), POLYTOL(2), ASPECTRATIO(3),
    	SIZERATIO(4), RATIOTOL(5), ETOL1(6), ETOL2(7), ESIZE(8), METHOD(9);
    
    	private final int value;
    	private Param(int value){
    		this.value = value;
    	}
    	
    	public int getValue() {
    		return value;
    	}
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
        if (matptr == -1) {return new Mat[2];}
    	Mat[] out = {getMat(matptr),getMat(matptr+1)};
    	return out;
    }
    
    public void setValue(Param param, double value) {
    	setValue(ptr_, param.getValue(), value);
    }
}
