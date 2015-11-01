#include <std.h>
#include <vector.h>
#include <cv2.h>


class IMGProcessor {
    private:
        //All contours from findPoly along with the heirarchy

        //A Focus Point
        //Null-Condition: this.depth == -1



        // -------------- Geometric ------------------


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
