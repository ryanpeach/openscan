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

#include "filters.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <ctime>
#include <array>
#include <list>

namespace COLORS {
const auto white = Scalar(255, 255, 255);
const auto red = Scalar(255, 0, 0);
const auto green = Scalar(0, 255, 0);
const auto blue = Scalar(0, 0, 255);
const auto yellow = Scalar(255, 255, 0);
const auto purple = Scalar(255, 0, 255);
const auto cyan = Scalar(0, 255, 255);
}

namespace OPT {
const double VSCALE = 1000.0;
enum Method {fpcorners, strongborder, regular, automatic};
enum PageType {detect, letter};
enum Par {ANGLETOL, DISTTOL, POLYTOL, ASPECTRATIO, SIZERATIO, RATIOTOL, ETOL1, ETOL2, ESIZE, METHOD,
			CBLOCK, CSIZE, K, CTHRESH, BSIZE, BSIGMA};
};

int PtoInt(double v);
double PtoDouble(int v);

using namespace OPT;
using namespace COLORS;

class Capture {
 private:
    // Variable Declaration
    // These variables allow you to set global params for all process methods.
    Mat frame, edges;    // A variable to hold the most current processed frame
    Cnts polys;           // Just a temp variable to hold the polys from the most frame
    Fps fps;              // Just a temp variable to hold the Fp's from the most recent frame
    Point ref;            // The reference corner's location
    cnt border, outline;  // The border of the scan
    vector<cnt> rects;    // Rectangles identified in the image
    Points corners;

    int angleTol;          // The angle tolerance app-wide
    int distTol;           // The distance tolerance for small polygons (like focus pointss)
    int polyTol;           // The distance tolerance for universal polygons (like page borders)
    double aspectRatio;    // The aspect ratio of the page we are looking for. Always < 1.0
    double sizeRatio;      // The min ratio of a detected page to the size of the image
    double ratioTol;       // The tolerance for two ratios to be considered equal

    //Import Parameters
    int etol1, etol2, eSize, bSize, bSigma;         // Used in edge detection
    int cBlock, cSize, k, cThresh;   				// Used in corner detection
    Method sel;

    // Preprocessing allows each process to share data,
    // so that nothing is calculated twice for the same image.
    Mat getEdges();
    Cnts getPolys();
    Fps getFps();
    vector<cnt> getRects();
    Points getCorners();

    cnt getBorder();
    cnt getOutline();

    void set(cnt corners);
    void set(Fps corners);
    bool validRect(cnt r);
    cnt isQR(Point a, Point b, Point c);

    void checkChanged();
    void setAspectRatio(PageType type = letter);

    vector<cnt> getQRBorders(list<Fp> fs);

 public:

    Mat drawInfo();
    Mat drawEdges();
    Mat drawPolys(Mat img, Scalar color = white);
    Mat drawRects(Mat img, Scalar color = red);
    Mat drawFps(Mat img, Scalar color = green);
    Mat drawCorners(Mat img, Scalar color = blue);
    Mat drawBorder(Mat img, Scalar color = blue);
    Mat drawOutline(Mat img, Scalar color = yellow);

    /**
     * The main process. Finds the border of the page, filters it, warps it, returns the scan.
     * @return Returns a vector {The original image with a found border drawn, The scan in RGB}
     * @return If no border found, returns {The original image, NULL}.
     * @complexity O(?)
     */
    vector<Mat> process();

    /**
     * @complexity WARNING: HUGE
     */
    vector<Mat> getQr();

    Capture (int angleTol = 20, int distTol = 20, int polyTol = 5,
             PageType aspectRatio = letter, double sizeRatio = .25, double ratioTol = .1,
             int etol1 = 100, int etol2 = 200, int eSize = 3, int bSize = 5, double bSigma = 5,
             int cBlock = 2, int cSize = 3, int k = .04, int cThresh = 200, Method sel = fpcorners):
                angleTol(angleTol), distTol(distTol), polyTol(polyTol),
                sizeRatio(sizeRatio), ratioTol(ratioTol),
                etol1(etol1), etol2(etol2), eSize(eSize), bSize(bSize), bSigma(bSigma),
                cBlock(cBlock), cSize(cSize), k(k), cThresh(cThresh), sel(sel)
    {
        setValue(ASPECTRATIO,(int)aspectRatio);
    }

    void Frame(Mat img);

    void setValue(Par param, int value);
    int getValue(Par param);

};
#endif
