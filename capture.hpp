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
#include <ctime>

class Capture {
 private:
    // Variable Declaration
    // These variables allow you to set global params for all process methods.
    Mat frame, edges;      // A variable to hold the most current processed frame
    Cnts polys;            // Just a temp variable to hold the polys from the most frame
    vector<Fp> fps;        // Just a temp variable to hold the Fp's from the most recent frame

    int angleTol;          // The angle tolerance app-wide
    int distTol;           // The distance tolerance for small polygons (like focus pointss)
    int polyTol;           // The distance tolerance for unversal polygons (like page borders)
    int wSize;             //
    int C;                 //
    double aspectRatio;    // The aspect ratio of the page we are looking for.
    double sizeRatio;      // The min ratio of a detected page to the size of the image
    int etol1;             //
    int etol2;             //
    int eSize;             //
    int R;                 //

    // Preprocessing allows each process to share data,
    // so that nothing is calculated twice for the same image.
    void preProcess(Mat img);

 public:

    /**
     * The main process. Finds the border of the page, filters it, warps it, returns the scan.
     * @param The RGB image
     * @return Returns a vector {The original image with a found border drawn, The scan in RGB}
     * @return If no border found, returns {The original image, The original image}.
     * @complexity O(?)
     */
    vector<Mat> focusPointCorners(Mat img);

    /**
     * The first alternative process. Finds the border of the page without using focus point corners.
     * @param The RGB image
     * @return Returns a vector {The original image with a found border drawn, The scan in RGB}
     * @return If no border found, returns {The original image, The original image}.
     * @complexity O(?)
     */
    vector<Mat> strongBorder(Mat img);

    Capture (int angleTol = 10, int distTol = 5, int polyTol = 5, int wSize = 11,
             int C = 2, double aspectRatio = 8.5/11.0, double sizeRatio = .5, int etol1 = 100, int etol2 = 200, int eSize = 3, int R = .04):
                angleTol(angleTol), distTol(distTol), polyTol(polyTol), wSize(wSize),
                C(C), aspectRatio(aspectRatio), sizeRatio(sizeRatio), etol1(etol1), etol2(etol2), eSize(eSize), R(R)
    {}

};

#endif
