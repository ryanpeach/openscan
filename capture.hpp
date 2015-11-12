/**
 * capture.hpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#ifndef CAPTURE
#define CAPTURE
#define DESKTOP

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
    double aspectRatio;    //
    int etol1;             //
    int etol2;             //
    int eSize;             //
    int R;                 //

 public:

    /**
     * The main process. Finds the border of the page, filters it, warps it, returns the scan.
     * @param The RGB image
     * @return Returns a vector {The original image with a found border drawn, The scan in RGB}
     * @return If no border found, returns {The original image, The original image}.
     * @complexity O(?)
     */
    vector<Mat> process(Mat img, bool filter = true);

    /**
     * The first alternative process. Finds the border of the page without using focus point corners.
     * @param The RGB image
     * @return Returns a vector {The original image with a found border drawn, The scan in RGB}
     * @return If no border found, returns {The original image, The original image}.
     * @complexity O(?)
     */
    vector<Mat> process2(Mat img, bool filter = true);
   
    /**
     * The second alternative process. Finds the border of the page within a still image using focus points.
     * @param The RGB image
     * @return Returns a vector {The original image with a found border drawn, The scan in RGB}
     * @return If no border found, returns {The original image, The original image}.
     * @complexity O(?)
     */
    vector<Mat> process3(Mat img, bool filter = true);

#ifdef DESKTOP
    void webCam(); 
#endif

    Capture (int angleTol = 10, int distTol = 5, int polyTol = 5, int wSize = 11,
            int C = 2, double aspectRatio = 8.5/11.0, int etol1 = 100, int etol2 = 200, int eSize = 3, int R = .04):
                angleTol(angleTol), distTol(distTol), polyTol(polyTol), wSize(wSize),
                C(C), aspectRatio(aspectRatio), etol1(etol1), etol2(etol2), eSize(eSize), R(R)
    {}

};

#endif
