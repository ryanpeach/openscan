/**
 * capture.hpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#ifndef CAPTURE
#define CAPTURE

#include "cvmethods.hpp"
#include "opencv2/highgui/highgui.hpp"

class Capture {
 private:
    // Variable Declaration
    Cnts polys;            //
    vector<Fp> fps;        //
    int angleTol;          //
    int distTol;           //
    int polyTol;           //
    int wSize;             //
    int C;                 //
    int etol1;             //
    int etol2;             //
    int eSize;             //
    int R;                 //
    double aspectRatio;    //

 public:
    /**
     * The alternative process. Finds the border of the page without using focus point corners.
     * If any focus points exist, makes sure they are all within the found border.
     * Filters the output, warps it, returns the scan.
     * @param The RGB image
     * @return Returns a vector {The original image with a found border drawn, The scan in RGB}
     * @return If no border found, returns {The original image, The original image}.
     * @complexity O(?)
     */
    vector<Mat> process2(Mat img);

    /**
     * The main process. Finds the border of the page, filters it, warps it, returns the scan.
     * @param The RGB image
     * @return Returns a vector {The original image with a found border drawn, The scan in RGB}
     * @return If no border found, returns {The original image, The original image}.
     * @complexity O(?)
     */
    vector<Mat> process(Mat img);

    void webCam(); 
    
    Capture (int angleTol, int distTol, int polyTol, int wSize,
            int C, double aspectRatio, int etol1, int etol2, int eSize, int R):
                angleTol(angleTol), distTol(distTol), polyTol(polyTol), wSize(wSize),
                C(C), aspectRatio(aspectRatio), etol1(etol1), etol2(etol2), eSize(eSize), R(R)
    {}

    //Sets default variables
    Capture() {Capture(10, 5, 5, 11, 2, 8.5 / 11.0, 100, 200, 3, .04);}
};

#endif
