#include <std.h>
#include <vector.h>
#include <cv2.h>

typedef vector<Point> cnt;
template <typename T>

class IMGProcessor {
    private:
        //All contours from findPoly along with the heirarchy
        struct Cnts {
            vector<cnt> contours;
            vector<Vec4i> heirarchy;
        }

        //A Focus Point
        //Null-Condition: this.depth == -1
        class Fp {
            public:
                vector<cnt> contours;
                Point center;
                int depth, shape;

                Fp (vector<cnt> contours) throws Exception {
                    this.contours = contours;
                    this.center = centroid(contours);
                    this.depth = findInnerBorder(contours);
                    this.shape = contours[this.depth].size();
                }

            private:
                //Checks shape of each contour from last to -5 and finds the first 'square.' Returns 0 if none exists.
                int findInnerBorder(vector<cnt> cnts) {
                    cnt contour;
                    for (int x = sizeOf(cnts); x > 0; x++) {
                        contour = cnts[x]
                        if (isPoly(contour,4,true)) {return x+1;}
                        else {return -1;}
                    }
                }
                int findInnerBorder(Cnts cnts) {return findInnerBorder(cnts.contours);}
        }


        // -------------- Feature Detection ----------------
        //Filters the img, finds the contours, and returns the Cnts.
        //Uses: polyTol
        Cnts findPolys (Mat img) {
            //Find contours and heirarchy
            vector<cnt> contours, polys; vector<Vec4i> heirarchy; cnt temp;
            findContours(img, contours, heirarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

            //Return approximate polygons
            for (int i = 0; i < contours.size(); i++) {
                approxPolyDP(contours[i], temp, polyTol, true);
                polys.push_back(temp);
            }

            //Return Cnts
            return {polys, heirarchy};
        }

        //Find all the focus points within an image.
        vector<Fp> findFocusPoints (Cnts polys) {
            //Definitions
            vector<FP> out; FP tempFp; vector<vector<cnt>> cntV;
            vector<int> done; vector<cnt> contours; int k;

            for(int i = 0; i < polys.contours.size(); i++) {
                k=i; poly.clear();
                if(!done.contains(i)){        //Check that through navigation you haven't been here before
                    done.push_back(i);

                    //Navigate the heirarchy
                    while (heirarchy[k][2] != -1) {
                        k=heirarchy[k][2];
                        done.push_back(k);
                        contours.push_back(polys.contours[k]);
                    }
                    if (heirarchy[k][2] != -1) {contours.push_back(polys.contours[k]);} //Add the last element

                    //Check if there are enough polys to count as a potential focus point, append them to fp
                    if (poly.size() >= 5) {cntV.push_back(contours);}
                }
            }

            //Filter the focus points for their innermost border
            for (int x = 0; x < cntV.size()); x++) {
                tempFp = Fp(cntV[x]);
                if (tempFp.depth >= 0) {out.push_back(tempFp);}
            }

            //Return the focus points
            return out;
        }

        //Classifies squares and selects the four most likely to be corners
        //Null-Condition: Returns null
        //Uses: angleTol
        vector<Fp> getCorners(vector<Fp> focusPoints) {
            list<Fp> fpList = list<Fp>(focusPoints);
            list<Fp> fours = fpList.remove_if([](Fp z){return z.shape == 4;});
            double angles (Point x) {
                vector<double> out;
                for (Fp y : fours) {for (Fp z : fours) {if (x != y && y != z && x != z) {out.push_back(angle(x,y,z));}}}
                return out;
            }

            //Classify corners as having 2 right angles
            list<Fp> out;
            for (f : fours) {
                if (angles(f.contours).remove_if([](double z){return math.abs(z-90.0)<angleTol;}).size()>=2 && !find(out.begin(),out.end(),f)) {
                    out.push_back(f);
                }
            }

            //Return their centroids
            if (!hasRectangle(out)) {return null;}
            return vector<Fp>(out);
        }

        //Sort edges by distance.
        //Corners must be a rectangle
        //Null-Condition: Returns corners
        vector<Fp> sortCorners(vector<Fp> corners) {
            Point cent = centroid(corners); list<double> polar; int n; vector<Fp> out;
            for (Fp f : corners) {polar.push_back(angle(f.center,cent));} //Calculate all the angles from the centroid, maintaining index
            //Sort "corners" by the order of sorted "polar"
            vector<Point> sorted = polar.sort();
            for (int i = 0; i<polar.size()); i++) {
                n = find(polar.begin(), polar.end(), sorted[i]) - polar.begin(); //Gets the index of sorted[i]
                out.push_back(corners[n]); //Return sorted corners
            }

            return out;
        }

        //Null-Condition: Returns fps[0]
        Fp getRef(vector<Fp> fps) {
            Fp maxFp = fps[0]; int max = maxFp.depth; //sets default values
            for (Fp fp : fps) {
                if (fp.depth > max) {
                    maxFp = fp;
                    max = fp.depth;
                }
            }
            return maxFp;
        }

        //Null-Condition: Returns contour[0]
        Point getRef(cnt contour) {
            auto D = dists(contour); int a = 0; int b = 1;
            while (D[a]<=D[b]){
                a++; b++;
                if (b>=contour.size()){b=0;}
                if (a==contour.size()){return contour[0];}
            }
            return contour[a];
        }

        // ------------ Image Manipulation --------------
        //Uses: etol1, etol2, eSize
        Mat importFilter(Mat img){
            //Declarations
            Mat gray, edges;

            //Convert to gray if not already
            if (isColor(img)) {cvtColor(img,gray,COLOR_RGB2GRAY);}
            else {gray = img.copy();}

            //Return Canny Edge Detection
            Canny(gray,edges,etol1,etol2,eSize);
            return edges
        }

        //Uses: wSize, C
        Mat outputFilter(Mat img){
            Mat gray, out;
            cvtColor(img, gray, COLOR_RGB2GRAY);
            adaptiveThreshold(gray, out, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, wSize, C);
            return out;
        }

        //Uses: R
        Mat cropImage(Mat img){
            int sizeX = img.cols; int sizeY = img.rows;
            return img[R:sizeY-R][R:sizeX-R];
        }

        //Uses: aspectRatio
        //Reference: Modified from http://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
        Mat fixPerspective (Mat img, vector<cnt> border, Point ref) {
            //Declare variables
            cnt t1, tr, br, b1;
            Mat M, out;

            //Rotate the array until the reference is first
            while (centroid(border[0]) != ref)
            border = rotateList(border,1);

            tl = border[0]; tr = border[1]; br = border[2]; bl = border[3];

            // compute the width of the new image, which will be the
            // maximum distance between bottom-right and bottom-left
            // x-coordiates or the top-right and top-left x-coordinates
            auto widthA = math.sqrt(((br[0] - bl[0]) ** 2) + ((br[1] - bl[1]) ** 2));
            auto widthB = math.sqrt(((tr[0] - tl[0]) ** 2) + ((tr[1] - tl[1]) ** 2));
            auto maxWidth = max(int(widthA), int(widthB));

            // compute the height of the new image, which will be the
            // maximum distance between the top-right and bottom-right
            // y-coordinates or the top-left and bottom-left y-coordinates
            auto heightA = math.sqrt(((tr[0] - br[0]) ** 2) + ((tr[1] - br[1]) ** 2));
            auto heightB = math.sqrt(((tl[0] - bl[0]) ** 2) + ((tl[1] - bl[1]) ** 2));
            auto maxHeight = max(int(heightA), int(heightB));

            // now that we have the dimensions of the new image, construct
            // the set of destination points to obtain a "birds eye view",
            // (i.e. top-down view) of the image, again specifying points
            // in the top-left, top-right, bottom-right, and bottom-left
            // order
            auto dst = Rect(Point(0, 0),
            Point(maxWidth - 1, 0),
            Point(maxWidth - 1, maxHeight - 1),
            Point(0, maxHeight - 1));

            //Return Perspective Transform
            M = getPerspectiveTransform(border, dst);
            warpPerspective(img, out, M, (maxWidth, maxHeight));
            return out;
        }

        bool isColor(Mat img){
            if (sizeOf(img[0][0])==3) {return true;}
            else {return false}
        }

        // -------------- Geometric ------------------
        bool allSameLength(cnt poly){
            vector<vector<Point>> pairs; vector<double> lengths, error; vector<bool> test; int i;

            //Get a list of all lines in poly
            pairs.push_back({poly[poly.size()-1],poly[0]}) 											//Add the first pair to the list
            for (i = 1; i < poly.size(); i++) {pairs.push_back({poly[i-1],poly[i]});} 				//Add the rest
            for (i = 0; i < poly.size(); i++) {lengths.push_back(dist(pairs[i][0],pairs[i][1]));} 	//Get a list of the length of all pairs in poly
            double mean = accumulate(lengths.begin(), lengths.end(), 0.0) / lengths.size();
            for (i = 0; i < poly.size(); i++) {error.push_back(abs(lengths[i]-mean));} 			//Get the error from the mean of each length
            for (i = 0; i < poly.size(); i++) {test.push_back(error[i] < distTol);}				//Check if the error is within tolerance
            return find(test.begin(), test.end(), false)!=v.end();									//Test and return to see if there is a false within the test vector
        }

        //Test that all focus points are inside the poly
        bool allInside(cnt poly, vector<FP> fps) {
            for (Fp f : fps) {
                if (!pointPolygonTest(polys[i], f.center, false) {
                    return false;
                }
            }
            return true;
        }

        cnt rotateCnt(cnt contour, int n) {
            cnt AB = cnt().reserve(contour.size());
            AB.insert( AB.end(), contour[n:].begin(), contour[n:].end() );
            AB.insert( AB.end(), contour[:n].begin(), contour[:n].end() );
            return AB;
        }
        cnt rotateCnt(cnt contour) {return rotateCnt(contour,1);}

        //Returns the center of a contour, or of a bunch of contours, or of a bunch of points
        Point centroid(cnt contour) {
            Point sum = Point(0,0);
            for (Point p : contour) {sum += p;}
            return sum / contour.size();
        }
        Point centroid(vector<cnt> contours) {
            Point sum = Point(0,0);
            for (cnt c : contours) {sum += centroid(c);}
            return sum / contours.size();
        }
        Point centroid(Cnts contours) {return centroid(contours.contours);}
        Point centroid(vector<Fp> fps) {
            vector<cnt> contours;
            for (Fp f : fps) {
                contours.push_back(f.contour);
            }
            return centroid(contours);
        }

        double dist(Point a, Point b){
            return sqrt((a[0]-b[0])**2+(a[1]-b[1])**2);
        }

        double angle(Point origin, Point a){
            Point v = a - origin;
            return atan2(v[0],v[1]) * 180 / PI;
        }

        double angle(Point origin, Point c2, Point c3) {
            Point *a1 = origin; Point *a2 = c2; Point *a3 = origin; Point *a4 = c3;
            return acos(dot((*a2-*a1),(*a4-*a3))/(norm(*a2-*a1)*norm(*a4-*a3)))) * 180 / PI;
        }

        //Runs the polygon rules of this application, that all valid shapes are convex, all size 4 shapes have all right angles within tolerance, and optionally all sides are the same length
        bool isPoly(cnt poly, int size, int regular) {
            if (poly.size()==size && isContourConvex(poly)) {
                if (size == 4) {
                    auto angles = angles(r);
                    for (a : angles) {if (abs(a-90.0)>angleTol) {return false;}}    //Test that all angles are within tolerance of 90
                }
                if (regular) {return allSameLength(poly);}
                else {return true;}
            }
            else {return false;}
        }
        bool isRectangle(cnt poly, bool square) {return isPoly(poly,4,square);}
        bool isSquare(cnt poly) {return isPoly(poly,4,true);}

        //Finds if there is a rectangle within poly
        bool hasRectangle(vector<Fp> poly) {
            if (sizeOf(poly)!=4) {return false;}
            //check all combinations of poly
            for (a1 : poly) {
                for (a2 : poly) {
                    for (a3 : poly) {
                        for (a4 : poly) {
                            if (isRectangle({a1.center,a2.center,a3.center,a4.center},false)) {
                                return true;
            }}}}}
            return false;
        }

        //Returns a vector of angles for the polygon
        vector<double> angles(cnt poly) {
            int a = 0; int b = poly.size()-1; int c = 1;
            vector<double> out;
            while (a < poly.size()) {
                out.push_back(angle(poly[a],poly[b],poly[c]));
                a++;b++;c++;
                if (c==poly.size()) {c=0;}
                if (b==poly.size()) {b=0;}
            }
            return out;
        }

        //Returns a vector of distances for the polygon
        vector<double> dists(cnt poly) {
            int a = 0; int b = 1;
            vector<double> out;
            while (a < poly.size()) {
                out.push_back(dist(poly[a],poly[b]));
                a++; b++;
                if (b==poly.size()) {b=0;}
            }
        }


        //Variable Declaration
        Cnts polys; vector<Fp> fps;
        int angleTol, distTol, polyTol, wSize, C, etol1, etol2, eSize, R;
        double aspectRatio;

        //Processes the image without the help of corner focus points
        //Null-Condition: Returns {null,img}
        vector<Mat> process2(Mat img) {
            //Get the largest rectangular border from polys which contains all focus points.
            //If no focus points exist, then simply return the largest border.
            cnt largest; int size=0;
            for (int i = 0; i < polys.size(); i++) {
                if (isRectangle(polys[i]) && contourArea(polys[i])>size) {
                    if ((fps.size()!=0 && allInside(polys[i],fps)) || fps.size()==0) {
                        largest = polys[i]; size = contourArea(polys[i]);
                    }
                }
            }

            //If one is found, return the processed image. If exception or none found, return null.
            if (size>0) {
                Point ref = getRef(largest);
                vector<Fp> sort = sortCorners(largest,ref);
                Mat warp = fixPerspective(img,sort,ref,aspectRatio);
                Mat crop = cropImage(warp,R*warp.cols);
                Mat out = outputFilter(crop,wSize,C);

                //Draw and return
                Mat drawing = drawContour(img, largest, color = {255,0,0}, thickness = 1);
                return {cvtColor(out,out,COLOR_GRAY2RGB), drawing};
            }
            else {return {null,img};}
        }

    public:
        // -------------- Main Methods ----------------------
        //Null-Condition: Returns process2(img)
        vector<Mat> process(Mat img) {
            //Define Variables
            vector<Fp> corners, sort; Fp ref; Mat warp, crop, out;

            //Intial Processing
            polys = findPolys(img);
            fps = findFocusPoints(polys);

            //Get border from focus points
            vector<Fp> corners = getCorners(fps,angleTol);
            if (corners.size()==4) {
                Fp ref = getRef(corners);
                vector<Fp> sort = sortCorners(corners,ref);
                warp = fixPerspective(img,sort,ref);
                crop = cropImage(warp,R*warp.cols);
                out = outputFilter(crop,wSize,C);
            } else {return process2(img);}

            Mat drawing = drawContour(img, sort, color = {255,0,0}, thickness = 1);
            return {cvtColor(out,out,COLOR_GRAY2RGB), drawing};
        }

        //Reset Global variables
        void setVars(int angleTol, int distTol, int polyTol, int wSize, int C, double aspectRatio, int etol1, int etol2, int eSize, int R) {
            tshis.angleTol = angleTol; this.distTol = distTol; this.polyTol = polyTol; this.wSize = wSize; this.C = C;
            this.aspectRatio = aspectRatio; this.etol1 = etol1; this.etol2 = etol2; this.eSize = eSize; this.R = R;
        }

        //Sets Global variables
        void IMGProcessor (int angleTol, int distTol, int polyTol, int wSize, int C, double aspectRatio, int etol1, int etol2, int eSize, int R) {
            this.angleTol = angleTol; this.distTol = distTol; this.polyTol = polyTol; this.wSize = wSize; this.C = C;
            this.aspectRatio = aspectRatio; this.etol1 = etol1; this.etol2 = etol2; this.eSize = eSize; this.R = R;
        }

        //Sets default variables
        Mat IMGProcessor() {return IMGProcessor(img,10,5,5,11,2,8.5/11.0,100,200,3,.04);}
}
