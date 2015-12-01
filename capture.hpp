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

namespace COLORS {
const auto white = Scalar(255, 255, 255);
const auto red = Scalar(255, 0, 0);
const auto green = Scalar(0, 255, 0);
const auto blue = Scalar(0, 0, 255);
}

namespace OPT {
const double VSCALE = 1000.0;
enum Method {fpcorners, strongborder, regular, automatic};
enum PageType {detect, letter};
enum Par {ANGLETOL, DISTTOL, POLYTOL, ASPECTRATIO, SIZERATIO, RATIOTOL, ETOL1, ETOL2, ESIZE, METHOD};
}

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
    cnt border;           // The border of the scan
    vector<cnt> rects;    // Rectangles identified in the image

    int angleTol;          // The angle tolerance app-wide
    int distTol;           // The distance tolerance for small polygons (like focus pointss)
    int polyTol;           // The distance tolerance for universal polygons (like page borders)
    double aspectRatio;    // The aspect ratio of the page we are looking for.
    double sizeRatio;      // The min ratio of a detected page to the size of the image
    double ratioTol;       // The tolerance for two ratios to be considered equal

    //Import Parameters
    int etol1, etol2, eSize;

    // Preprocessing allows each process to share data,
    // so that nothing is calculated twice for the same image.
    Method sel;
    Mat getEdges();
    Cnts getPolys();
    Fps getFps();
    vector<cnt> getRects();
    vector<Point> getCorners();

    cnt getBorder();

    void set(cnt corners);
    void set(Fps corners);
    bool validRect(cnt r);

    void checkChanged();

 public:

    Mat drawInfo();
    Mat drawEdges(Mat img, Scalar color);
    Mat drawPolys(Mat img, Scalar color);
    Mat drawRects(Mat img, Scalar color);
    Mat drawFps(Mat img, Scalar color);
    Mat drawCorners(Mat img, Scalar color);
    Mat drawBorder(Mat img, Scalar color);
    Mat drawOutline(Mat img, Scalar color);
    Mat drawEdges() {return getEdges();}

    /**
     * The main process. Finds the border of the page, filters it, warps it, returns the scan.
     * @return Returns a vector {The original image with a found border drawn, The scan in RGB}
     * @return If no border found, returns {The original image, NULL}.
     * @complexity O(?)
     */
    vector<Mat> process();

    Capture (int angleTol = 20, int distTol = 20, int polyTol = 5,
             PageType aspectRatio = letter, double sizeRatio = .25, double ratioTol = .1,
             int etol1 = 100, int etol2 = 200, int eSize = 3, Method sel = fpcorners):
                angleTol(angleTol), distTol(distTol), polyTol(polyTol),
                sizeRatio(sizeRatio), ratioTol(ratioTol),
                etol1(etol1), etol2(etol2), eSize(eSize), sel(sel)
    {
        setValue(ASPECTRATIO,(int)aspectRatio);
    }

    void Frame(Mat img);

    void setValue(Par param, int value) {
        auto toDouble = [](int v) {return ((double)v)/VSCALE;};
    	bool changed = true;
            switch(param) {
            case ANGLETOL: angleTol = value; break;
            case DISTTOL: distTol = value; break;
            case POLYTOL: polyTol = value; break;
            case ASPECTRATIO: setAspectRatio((PageType)value); break;
            case SIZERATIO: sizeRatio = toDouble(value); break;
            case RATIOTOL: ratioTol = toDouble(value); break;
            case ETOL1: etol1 = value; break;
            case ETOL2: etol2 = value; break;
            case ESIZE: eSize = value; break;
            case METHOD: sel = (Method)value; break;
            default: changed = false; break;
    	}
        if (changed) {
            Frame(frame);
        }
    }

    void setAspectRatio(PageType type = letter) {
        switch (type) {
            case letter:
                aspectRatio = 8.5/11.0;
                break;
            case detect:
                aspectRatio = 0;
                break;
        }
    }

};
#endif
