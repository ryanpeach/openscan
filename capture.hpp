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

class Capture {
 private:
    // Variable Declaration
    // These variables allow you to set global params for all process methods.
    Mat *frame, *edges;    // A variable to hold the most current processed frame
    Cnts *polys;           // Just a temp variable to hold the polys from the most frame
    Fps *fps;              // Just a temp variable to hold the Fp's from the most recent frame
    Point *ref;            // The reference corner's location
    cnt *border;           // The border of the scan

    int angleTol;          // The angle tolerance app-wide
    int distTol;           // The distance tolerance for small polygons (like focus pointss)
    int polyTol;           // The distance tolerance for universal polygons (like page borders)
    double aspectRatio;    // The aspect ratio of the page we are looking for.
    double sizeRatio;      // The min ratio of a detected page to the size of the image
    double ratioTol;       // The tolerance for two ratios to be considered equal

    //Import Parameters
    int etol1, etol2, eSize;

    enum Method {fpcorners, strongborder} sel;

    // Preprocessing allows each process to share data,
    // so that nothing is calculated twice for the same image.
    Mat* getEdges();
    Cnts* getPolys();
    Fps* getFps();

    Point* getRef();
    cnt* getBorder();

    void set(cnt corners);
    void set(Fps corners);
    bool validRect(cnt r);

 public:

    /**
     * The main process. Finds the border of the page, filters it, warps it, returns the scan.
     * @return Returns a vector {The original image with a found border drawn, The scan in RGB}
     * @return If no border found, returns {The original image, NULL}.
     * @complexity O(?)
     */
    vector<Mat*> process();

    Capture (int angleTol = 10, int distTol = 5, int polyTol = 5,
             double aspectRatio = 8.5/11.0, double sizeRatio = .5, double ratioTol = .1,
             int etol1 = 100, int etol2 = 200, int eSize = 3, Method sel = fpcorners):
                angleTol(angleTol), distTol(distTol), polyTol(polyTol),
                aspectRatio(aspectRatio), sizeRatio(sizeRatio), ratioTol(ratioTol),
                etol1(etol1), etol2(etol2), eSize(eSize), sel(sel)
    {}

    void Frame(Mat img);

};

#endif
