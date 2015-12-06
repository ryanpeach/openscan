/**
 * cvmethods.cpp
 *
 * @date Nov 4, 2015
 * @author Ryan Peach
 * @version v0.1
 */

#include "cvmethods.hpp"
//#define TEST

// -------------- Feature Detection ----------------

Cnts findPolys(Mat* img, double distRatio) {
#ifdef TEST
        cout << "Running findPolys..." << endl;
#endif
    // Find contours and heirarchy
    vector<cnt> contours, polys; vector<Vec4i> heirarchy; cnt temp;
    findContours(*img, contours, heirarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // Return approximate polygons
    for (unsigned int i = 0; i < contours.size(); i++) {
    	//double epsilon = distTol*arcLength(contours[i],true);
        approxPolyDP(contours[i], temp, distTol(contours[i],distRatio), true);
        polys.push_back(temp);
    }

    // Return Cnts
    return Cnts(polys, heirarchy);
}

Fps findFocusPoints(Cnts polys, double angleTol, double distRatio) {
#ifdef TEST
    cout << "Running findFocusPoints..." << endl;
#endif
    // Definitions
    vector<Fp> out; vector< vector<cnt> > cntV;
    vector<int> done; vector<cnt> contours; int k;
    cnt poly;

    for(unsigned int i = 0; i < polys.contours.size(); i++) {
        k = i; contours.clear();
        // Check that through navigation you haven't been here before
        if(!contains<vector<int>, int>(done, (int)i)){
            done.push_back(i);
#ifdef TEST
            cout << "findFocusPoints: Navigating Heirarchy " << i << endl;
#endif
            // Navigate the heirarchy
            while (polys.heirarchy[k][2] != -1) {
                k = polys.heirarchy[k][2];
                done.push_back(k);
                contours.push_back(polys.contours[k]);
            }
            if (polys.heirarchy[k][2] != -1) {contours.push_back(polys.contours[k]);} //Add the last element

            // Check if there are enough polys to count as a potential focus point, append them to fp
            if (contours.size() >= 2) {cntV.push_back(contours);}
        }
    }

#ifdef TEST
    cout << "findFocusPoints: Filtering " << cntV.size() << endl;
#endif
    // Filter the focus points for their innermost border
    for (unsigned int x = 0; x < cntV.size(); x++) {
        Fp tempFp = Fp(cntV[x], angleTol, distRatio);
        if (tempFp.depth >= 0) {  // Check that cntV[x] is a valid Fp
            out.push_back(tempFp);
        }
    }

    // Return the focus points
    return out;
}

Fps calcCorners(Fps focusPoints, double angleTol, double distRatio) {
#ifdef TEST
    cout << "Running getCorners..." << endl;
    cout << "getCorners: Getting list of fours..." << endl;
#endif
    // Make fours a list of only size four Fp's
    // vector<Fp> fours = filter(focusPoints,[](Fp z){return z.shape == 4;});
    vector<Fp> fours;
    for (Fp z : focusPoints) {
            if (z.shape == 4) {
                    fours.push_back(z);
            }
    }

#ifdef TEST
    cout << "getCorners: Classifying corners as having 2 right angles..." << endl;
#endif
    // Classify corners as having 2 right angles
    vector<Fp> out;
    vector<double> a, temp;
    for (Fp f : fours) {
        // Gets the angles between f.center and all other items in "fours"
        a = angs(f.center, fours);

        // Add to temp all items which fall within tolerance from 90 degrees.
        vector<double> found;
        for (double d : a) {
            if (abs(d-90.0) < angleTol) {
                found.push_back(d);
            }
        }

        // If f has at least two right angle relations with
        // other items in "fours", add it to out.
        if (found.size() >= 2 && !contains<vector<Fp>, Fp>(out, f)) {
            out.push_back(f);
        }
    }

    // Return their centroids
    vector<Fp> rect = hasRectangle(out, angleTol, distRatio);
    if (rect.size() != 4) {return vector<Fp>();}
    else {return out;}
}

//This might have a large complexity due to toFps
cnt sortCorners(cnt corners, Point ref, double distRatio) {
#ifdef TEST
    cout << "Running sortCorners(cnt, Point, double)..." << endl;
#endif
    cnt sort = sortCorners(corners);

    for (int i = 0; i < 4 && dist(sort[0],ref) > distTol(sort,distRatio); i++) {
        sort = rotateVec(sort);
    }
    return sort;
}

cnt sortCorners(cnt corners) {
#ifdef TEST
    cout << "Running sortCorners(cnt)..." << endl;
#endif
    // Variable Declaration
    Point cent = centroid(corners); vector<double> polar; int n; cnt out;

    // Calculate all the angles from the centroid, maintaining index
    for (Point p : corners) {polar.push_back(angle(p, cent));}
    vector<double> sorted = polar;
    sort(sorted.begin(), sorted.end());

    // Sort "corners" by the order of sorted "polar"
    for (double d : sorted) {
        n = index(polar, d);
        out.push_back(corners[n]);  // Return sorted corners
    }
    return out;
}


Fp calcRef(vector<Fp> fps) {
#ifdef TEST
    cout << "Running getRef(vector<Fp>)..." << endl;
#endif
    bool found = false;
    Fp maxFp = fps[0]; int max = maxFp.depth;  // sets default values
    for (Fp fp : fps) {
        if (fp.depth > max && fp.depth <= max+1) {
            maxFp = fp;
            max = fp.depth;
            found  = true;
        }
    }
    if (found) {return maxFp;}
    else {return Fp();}
}

Point calcRef(cnt contour) {
#ifdef TEST
    cout << "Running getRef(cnt)..." << endl;
#endif
    auto D = dists(contour); unsigned int a = 0; unsigned int b = 1;
    while (D[a] <= D[b]) {
        a++; b++;
        if (b >= contour.size()) {b = 0;}
        if (a == contour.size()) {return contour[0];}
    }
    return contour[a];
}


// ------------ Image Manipulation --------------
Mat cropImage(Mat * img, int R) {
#ifdef TEST
    cout << "Running cropImage..." << endl;
#endif
    int sizeX = (*img).cols; int sizeY = (*img).rows;
    Mat out = (*img)(Rect(R, R, sizeX, sizeY));
    return out;
}

Mat fixPerspective(Mat * img, cnt border, Point ref) {
#ifdef TEST
    cout << "Running fixPerspective..." << endl;
#endif
    // Declare variables
    Point tl, tr, bl, br;
    Mat out;

    // Print Data
    cout << vtostr(border) << endl;

    tr = border[0]; tl = border[1]; br = border[2]; bl = border[3];

    // compute the width of the new image, which will be the
    // maximum distance between bottom-right and bottom-left
    // x-coordiates or the top-right and top-left x-coordinates
    auto widthA = sqrt(pow((double)(br.x - bl.x), 2.0) + pow((double)(br.y - bl.y), 2.0));
    auto widthB = sqrt(pow((double)(tr.x - tl.x), 2.0) + pow((double)(tr.y - tl.y), 2.0));
    int maxWidth = max(int(widthA), int(widthB));

    // compute the height of the new image, which will be the
    // maximum distance between the top-right and bottom-right
    // y-coordinates or the top-left and bottom-left y-coordinates
    auto heightA = sqrt(pow((double)(tr.x - br.x), 2.0) + pow((double)(tr.y - br.y), 2.0));
    auto heightB = sqrt(pow((double)(tl.x - bl.x), 2.0) + pow((double)(tl.y - bl.y), 2.0));
    int maxHeight = max((int)heightA, (int)heightB);

    // now that we have the dimensions of the new image, construct
    // the set of destination points to obtain a "birds eye view",
    // (i.e. top-down view) of the image, again specifying points
    // in the top-left, top-right, bottom-right, and bottom-left
    // order
    auto dst = vector<Point2f>({Point(0, 0),
    Point(maxWidth - 1, 0),
    Point(maxWidth - 1, maxHeight - 1),
    Point(0, maxHeight - 1)});
    auto src = vector<Point2f>({tl, tr, bl, br});

    // Return Perspective Transform
    auto M = getPerspectiveTransform(src, dst);
    warpPerspective(*img, out, M, Size(maxWidth, maxHeight));
    return out;
}
Mat fixPerspective(Mat * img, vector<Fp> border, Fp ref) {
	return fixPerspective(img, centroids(border), ref.center);
}

bool isColor(Mat * img) {
#ifdef TEST
    cout << "Running isColor..." << endl;
#endif
    if ((*img).channels() == 3) {return true;}
    else {return false;}
}
