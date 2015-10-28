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

		        public Fp(vector<cnt> contours) throws Exception {
		            this.contours = contours;
		            this.center = centroid(contours);
		            this.depth = findInnerBorder();
		            this.shape = contours.length;
		        }

			private:
		        //Takes the contours and returns the index of
		        int findInnerBorder() throws Exception {

				}
	    }


	    // -------------- Feature Detection ----------------
	    //Filters the img, finds the contours, and returns the Cnts.
	    Cnts findPolys (Mat img,int polyTol) {
	        //Find contours and heirarchy
	        vector<cnt> contours, polys; vector<Vec4i> heirarchy;
	        findContours(img,contours,heirarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

	        //Return approximate polygons
	        for(int i=0;i<sizeOf(contours);i++){
	            approxPolyDP(contours[i],polys[i],polyTol,true);
	        }

	        //Return Cnts
	        return {polys, heirarchy};
	    }
	    Cnts findPolys(Mat img){return findPolys(img,5);}

	    //Find all the focus points within an image.
	    vector<Fp> findFocusPoints (Cnts polys) {
	        //Definitions
	        vector<FP> out; FP tempFp; vector<vector<cnt>> cntV; vector<int> done; vector<cnt> contours; int k;

	        for(int i=0;i<sizeOf(polys.contours);i++){
	            k=i; poly.empty();
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
	    vector<Fp> getCorners(vector<Fp> focusPoints, int angleTol){
			list<Fp> fours = remove_if(fp.begin(),fp.end(),[](Fp z){return len(z)!=4;});
			double angles (Point x) {
				vector<double> out;
				for (Fp y:fours) for (Fp z:fours) if (x != y && y != z && x != z) out.push_back(angle(x,y,z));
				return out;
			}

			//Classify corners as having 2 right angles
			vector<Fp> out;
			for (f : fours)
				if len(filter(lambda z: np.absolute(z-90.0)<ATOL, angles(c)))>=2 and c not in out:
					out.append(c)

			//Return their centroids
			if len(out)!=4: raise Exception("Corners not Detected!")
			else: print out
			return out
		}

	    vector<Fp> sortCorners(vector<Fp> corners){}

	    // ------------ Image Manipulation --------------
	    Mat importFilter(Mat img,int tol1,int tol2,int wSize){
	        //Declarations
	        Mat gray,edges;

	        //Convert to gray if not already
	        try{cvtColor(img,gray,COLOR_RGB2GRAY);}
	        catch(Exception e){
	            gray = img.copy();
	        }

	        //Return Canny Edge Detection
	        Canny(gray,edges,tol1,tol2,wSize);
	        return edges
	    }
	    Mat importFilter(Mat img){return importFilter(img,100,200,3);}

	    Mat outputFilter(Mat img,int wSize,int C){

		}
	    Mat outputFilter(Mat img){return outputFilter(img,11,2);}

	    Mat cropImage(Mat img){

		}

	    Mat fixPerspective(Mat img,int[][] border,int ref,double ratio){

		}
	    Mat fixPerspective(Mat img,int[][] border,int ref){return fixPerspective(img,border,ref,8.5/11.0);}

	    // -------------- Geometric ------------------
	    Point centroid(cnt contour){

		}
	    Point centroid(vector<cnt> contours){

		}
	    Point centroid(Cnts contours){

		}

	    double dist(Point a, Point b){

		}

	    bool allSameLength(cnt poly, int distTol){

		}
	    bool allSameLength(cnt poly){return allSameLength(poly,5);}

	public:
	    // -------------- Main Methods ----------------------
	    Mat process(Mat img,int angleTol,int distTol,int polyTol,int blockSize,int C,double aspectRatio,int etol1,int etol2,int eSize){

		}
	    Mat process(Mat img){return call(img,10,5,5,11,2,8.5/11.0,100,200,3);}

	    void IMGProcessor() {}
}
