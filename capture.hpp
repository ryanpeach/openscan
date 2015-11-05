/**
 * capture.hpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#ifndef CAPTURE
#define CAPTURE

#include "geometry.hpp"
#include "cvmethods.hpp"
#include "support.hpp"

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

 public:
    /**
     * The main process. Finds the border of the page, filters it, warps it, returns the scan.
     * @param The RGB image
     * @return Returns a vector {The original image with a found border drawn, The scan in RGB}
     * @return If no border found, returns {The original image, The original image}.
     * @complexity O(?)
     */
    vector<Mat> process(Mat img);

    //Sets Global variables
    void Capture (int angleTol, int distTol, int polyTol, int wSize, int C, double aspectRatio, int etol1, int etol2, int eSize, int R) {
        this.angleTol = angleTol; this.distTol = distTol; this.polyTol = polyTol; this.wSize = wSize; this.C = C;
        this.aspectRatio = aspectRatio; this.etol1 = etol1; this.etol2 = etol2; this.eSize = eSize; this.R = R;
    }

    //Sets default variables
    Mat Capture() {return IMGProcessor(img, 10, 5, 5, 11, 2, 8.5 / 11.0, 100, 200, 3, .04);}
}

#endif
