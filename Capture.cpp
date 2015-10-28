#include <std.h>
#include <vector.h>
#include <cv2.h>

typedef vector<Point> cnt;

class IMGProcessor {
	private:
		//All contours from findPoly along with the heirarchy
		struct Cnts {
			vector<cnt> contours;
			vector<Vec4i> heirarchy;
		}

		//A Focus Point
		class Fp {
			public:
				vector<cnt> contours;
				Point center;
				int depth, shape;

				Fp (vector<cnt> contours) throws Exception {
					this.contours = contours;
					this.center = centroid(contours);
					this.depth = findInnerBorder(contours);
					this.shape = contours.length;
				}

			private:
				//Checks shape of each contour from last to -5 and finds the first 'square.' Returns 0 if none exists.
				int findInnerBorder(vector<cnt> cnts) throws Exception {
					cnt contour;
					for (int x = sizeOf(cnts); x > 0; x++) {
						contour = cnts[x]
						if (isContourConvex(contour) && sizeOf(contour)==4 && allSameLength(contour)) {return x+1;}
						else {throw Exception e;}
					}
				}
				int findInnerBorder(Cnts cnts) throws Exception {return findInnerBorder(cnts.contours);}
		}


		// -------------- Feature Detection ----------------
		//Filters the img, finds the contours, and returns the Cnts.
		Cnts findPolys (Mat img) {
			//Find contours and heirarchy
			vector<cnt> contours, polys; vector<Vec4i> heirarchy;
			findContours(img, contours, heirarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

			//Return approximate polygons
			for(int i=0; i<sizeOf(contours); i++) {
				approxPolyDP(contours[i],polys[i],polyTol,true);
			}

			//Return Cnts
			return {polys, heirarchy};
		}

		//Find all the focus points within an image.
		vector<Fp> findFocusPoints (Cnts polys) {
			//Definitions
			vector<FP> out; FP tempFp; vector<vector<cnt>> cntV;
			vector<int> done; vector<cnt> contours; int k;

			for(int i=0;i<sizeOf(polys.contours);i++){
				k=i; poly.clear();
				if(!done.contains(i)){        //Check that through navigation you haven't been here before
					done.push_back(i);

					//Navigate the heirarchy
					while(heirarchy[k][2]!=-1){
						k=heirarchy[k][2];
						done.push_back(k);
						contours.push_back(polys.contours[k]);
					}
					if(heirarchy[k][2]!=-1){contours.push_back(polys.contours[k]);} //Add the last element

					//Check if there are enough polys to count as a potential focus point, append them to fp
					if(sizeOf(poly)>=5){cntV.push_back(contours);}
				}
			}

			//Filter the focus points for their innermost border
			for(int x=0;x<sizeOf(cntV);x++){
				try {
					tempFp = new Fp(cntV[x]);
					out.push_back(tempFp);
	 			} catch (Exception e) {}
			}
			delete tempFp

			//Return the focus points
			return out;
		}

		//Classifies squares and selects the four most likely to be corners
		vector<Fp> getCorners(vector<Fp> focusPoints){
			list<Fp> fpList = list<Fp>(focusPoints);
			list<Fp> fours = fpList.remove_if([](Fp z){return z.shape == 4;});
			double angles (Point x) {
				vector<double> out;
				for (Fp y : fours) for (Fp z : fours) if (x != y && y != z && x != z) {out.push_back(angle(x,y,z));}
				return out;
			}

			//Classify corners as having 2 right angles
			list<Fp> out;
			for (f : fours)
				if (sizeOf(angles(f.contours).remove_if([](double z){return math.abs(z-90.0)<angleTol;}))>=2 && !out.contains(f))
					out.append(f);

			//Return their centroids
			if (sizeOf(out)!=4) {throw Exception e;}
			return vector<Fp>(out);
		}

		//Sort edges by distance.
	    vector<Fp> sortCorners(vector<Fp> corners){
			Point cent = centroid(corners); list<double> polar; int n; vector<Fp> out;
			for (Fp z : corners) {polar.push_back(angle(z.center,cent));} //Calculate all the angles from the centroid, maintaining index
			//Sort "corners" by the order of sorted "polar"
			vector<Point> sorted = polar.sort();
			for (int i = 0; i<sizeOf(polar); i++) {
				n = polar.index(sorted[i]);
				out.push_back(corners[n]); //Return sorted corners
			}

			return out;
		}

	    // ------------ Image Manipulation --------------
	    Mat importFilter(Mat img){
	        //Declarations
	        Mat gray,edges;

	        //Convert to gray if not already
	        try{cvtColor(img,gray,COLOR_RGB2GRAY);}
	        catch(Exception e){
	            gray = img.copy();
	        }

	        //Return Canny Edge Detection
	        Canny(gray,edges,etol1,etol2,eSize);
	        return edges
	    }

	    Mat outputFilter(Mat img){
			Mat gray, out;
			cvtColor(img, gray, COLOR_RGB2GRAY);
			adaptiveThreshold(gray, out, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, wSize, C);
			return out;
		}

	    Mat cropImage(Mat img, int R){
			int sizeX = img.cols; int sizeY = img.rows;
			return img[R:sizeY-R][R:sizeX-R];
		}

	    Mat fixPerspective(Mat img, vector<cnt> border, Fp ref) throws Exception {
			//Check initial factors
			if (sizeOf(border)!=4) {throw Exception e;}

			//Declare variables
			cnt t1, tr, br, b1;
			Mat M, out;

			//Rotate the array until the reference is first
			while (centroid(border[0]) != ref.center)
				border = rotateList(border,1);

			//Copied from http://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
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

	    // -------------- Geometric ------------------
	    bool allSameLength(cnt poly){
			vector<vector<Point>> pairs; vector<double> lengths, error; vector<bool> test; int i;

			//Get a list of all lines in poly
			pairs.push_back({poly[poly.size()-1],poly[0]}) 											//Add the first pair to the list
			for (i = 1; i < sizeOf(poly); i++) {pairs.push_back({poly[i-1],poly[i]});} 				//Add the rest
			for (i = 0; i < sizeOf(poly); i++) {lengths.push_back(dist(pairs[i][0],pairs[i][1]));} 	//Get a list of the length of all pairs in poly
			double = mean(lengths);
			for (i = 0; i < sizeOf(poly); i++) {error.push_back(abs(lengths[i]-mean));} 			//Get the error from the mean of each length
			for (i = 0; i < sizeOf(poly); i++) {test.push_back(error[i] < distTol);}				//Check if the error is within tolerance
			return find(test.begin(), test.end(), false)!=v.end();									//Test and return to see if there is a false within the test vector
		}

		cnt rotateCnt(cnt contour, int n) {
			cnt AB = cnt().reserve(contour.size());
			AB.insert( AB.end(), contour[n:].begin(), contour[n:].end() );
			AB.insert( AB.end(), contour[:n].begin(), contour[:n].end() );
		    return AB;
		}
		cnt rotateCnt(cnt contour) {return rotateCnt(contour,1);}

		Point centroid(cnt contour) {
			Point sum = Point(0,0);
			for (Point p : contour)
				sum += p;
			return sum / sizeOf(contour);
		}
	    Point centroid(vector<cnt> contours) {
			Point sum = Point(0,0);
			for (cnt c : contours)
				sum += centroid(c);
			return sum / sizeOf(contours);
		}
	    Point centroid(Cnts contours) {return centroid(contours.contours);}

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

		Fp getRef(vector<Fp> fps) {
			Fp maxFp; int max = 0;
			for (fp : fps) {
				if (fp.depth > max) {
					maxFp = fp;
					max = fp.depth;
				}
			}
			return maxFp;
		}

		// -------------- Main Methods ----------------------
	    Mat process(Mat img){
			//Intial Processing
			Cnts polys = findPolys(img,polyTol)
			vector<Fp> fps = findFocusPoints(polys)

			try {
				vector<Fp> corners = getCorners(fps,angleTol);
				vector<Fp> sort = sortCorners(corners);
				Fp ref = getRef(corners);
				Mat warp = fixPerspective(img,sort,ref,aspectRatio);
				Mat crop = cropImage(warp,R*warp.cols);
				Mat out = outputFilter(crop,wSize,C);
			}
			catch(...) {out = img;}

			return out;
		}

		const Mat img;
		const int angleTol, distTol, polyTol, wSize, C, etol1, etol2, eSize, R;
		const double aspectRatio;

	public:
	    Mat IMGProcessor(Mat img, int angleTol, int distTol, int polyTol, int wSize, int C, double aspectRatio, int etol1, int etol2, int eSize, int R) throws Exception {
			//Global variables
			this.img = img; this.angleTol = angleTol; this.distTol = distTol; this.polyTol = polyTol; this.wSize = wSize; this.C = C;
			this.aspectRatio = aspectRatio; this.etol1 = etol1; this.etol2 = etol2; this.eSize = eSize; this.R = R;

			return process(img);
		}

		Mat IMGProcessor(Mat img) throws Exception {return IMGProcessor(img,10,5,5,11,2,8.5/11.0,100,200,3,.04);}
}
