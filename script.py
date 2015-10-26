## This script contains the main logic of the code implementing OpenCV
## It is simple and to the point, outlining all the essential features of the application
import sys
sys.path.append("C:/Users/Ryan/Documents/Workspace/openscan/.lib/OpenCV/x86")
import cv2
import numpy as np
import random

ATOL = 10 #Degrees
DTOL = 10 #Pixels

def plot(imgs,titles = []):
	num = len(imgs)
	for i in range(num):
		plt.figure(i)
		plt.imshow(imgs[i])
		if len(titles)>0:
			plt.title(titles[i])
	plt.show()

def importImage(filepath):
	"""Imports the img at the filepath to an RGB ndarray."""
	return cv2.imread(filepath,0)

def saveImage(img, filepath, name):
	"""Saves the img at the filepath as the given name + filetype."""
	raise IOError

def findPolys(img):
	"""Filters the img, finds the contours, and returns the polys."""
	img = img.copy()
	try:
		gray = cv2.cvtColor(img,cv2.COLOR_RGB2GRAY);		#Convert Image captured from Image Input to GrayScale
	except:
		gray = img
		print "Already Gray."
	edges = cv2.Canny(gray, 100 , 200, 3);			#Apply Canny edge detection on the gray image

	find = cv2.findContours(edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) # Find contours with hierarchy
	heirarchy = find[2][0] 	#the last index is the heirarchy, it is contained within a one value array
	contours = find[1]		#The second index is the contour list

	#Return approximate polygons
	polys = []
	global DTOL
	for c in contours:
		temp = cv2.approxPolyDP(np.array(c),DTOL,True)
		polys.append(temp[:,0])
	return polys, heirarchy

#Code copied and modified from https://github.com/bharathp666/opencv_qr
def findFocusPoints(polys, heirarchy):
	"""Returns Border Poly. Checks that the QR info matches the border found."""
	focus, done = [], []
	for i in range(len(polys)):
		k, obj = i, []
		if i not in done:		#Check that through navigation you haven't been here before
			done.append(i)

			while heirarchy[k][2] != -1: #Navigate the heirarchy
				k = heirarchy[k][2]
				done.append(k)
				obj.append(k)
			if heirarchy[k][2] != -1: obj.append(k) #Add the last one

			#Check if there are enough polys to count as a potential focus point
			obj = [polys[x] for x in obj]
			if len(obj) >= 5:
				focus.append(obj[len(obj)-5:len(obj)]) #Get the last 5 elements and add them as a focus point
				continue

	#Filter the focus points for their innermost border
	out = []
	for f in focus:
		i = findInnerBorder(f) #Checks the shapes and looks for a square, returns innermost border
		if i>0:					#If found, append to out
			out.append(f[i])

	#Return the borders
	return out

def dist(a,b):
	"""Simple 2D Distance Formula"""
	return np.sqrt((a[0]-b[0])**2+(a[1]-b[1])**2)

def allSameLength(poly):
	"""Checks that all lines are the same length"""
        global DTOL
	pairs = [(poly[i-1],poly[i]) for i in range(len(poly))] 	#Get a list of all lines in poly
	lengths = map(lambda (a,b): dist(a,b), pairs)				#Get a list of the length of all pairs in poly
	mean = np.mean(lengths)										#Get the mean of all lengths in poly
	lengths = map(lambda z: np.absolute(z-mean), lengths)		#Get the error from the mean of each length
	return all(iter(map(lambda z: z<DTOL,lengths)))						#Return if all are within the error tolerance

def findInnerBorder(cnts):
	"""Checks shape of each contour from last to -5 and finds the first 'square.' Returns 0 if none exists."""
	for x in range(len(cnts)):
		cnt = cnts[-x]
		if cv2.isContourConvex(cnt) and len(cnt)==4 and allSameLength(cnt):
			return -x+3
	else: return 0

# --------------- Image Modification ----------------------
def fixPerspective(img, border,ratio=8.5/11.0):
	"""Returns img skewed to the border."""
	out = img.copy()
	sizeX, sizeY = img.shape()
	rY, rX = sizeY, ratio*sizeY
	#Set dst order to match the length ratios of previous (assume 8.5x11 page)
	src = np.array(border)
	dst = np.array([[0,0],[rX,0],[rX,rY],[0,rY]])

	#Return Affine Transform
	M = cv2.getPerspectiveTransform(src, dst)
	out = cv2.warpPespective(out, M)
	return out, dst

def drawBorder(img, border, c=(255,0,0)):
	"""Returns img with the border drawn overlay."""
	temp = img.copy()
	out = cv2.drawContours(temp, [border], 0, color = c,thickness = 4)
	return out

def filterOut(img):
	return img

def findCorners(fp):
        """Classifies squares and selects the four most likely to be corners"""
	fours = filter(lambda z: len(z)==4, fp)
	sets = (frozenset((x,y,z)) for x in fours for y in fours for z in fours if x!=y and y!=z)

        #Removes Duplicates
        found = []
	for p in sets:
                if p not in found:
                        found.append(p)
                        
        #Map angles onto found          
	angles = map(lambda p: angle(*tuple(p), found)

	#Corners have 2 right angles
	rightangles = [x for x in fours for i in range(len(pairs)) if x in pairs[i] and np.absolute(angles[i]-90)<ATOL]
        corners,found = [],set()
        for c in rightangles:
                if c not in found:
                        found.add(c)
                else:
                        corners.append(c)
        return np.array([centroid(x) for x in corners])
        
def sortCorners(corners):
        """Sort edges by distance. Not done"""
        centroid = centroid(corners) #Get the centroid of the four corners
        polar = map(lambda z: angle(z,centroid),corners) #Get the polar angle from centroid
	sort = sorted(zip(corners,polar), key=1) #Sort by the polar coords
	keys = [x for x,y in sort] #Return just the keys

        #Sort the keys with the longest edge pair first
        pairs = [(keys[i-1],keys[i]) for i in range(len(keys))]
        dists = map(lambda z: dist(*z),pairs)
        if dists[0]>dists[1]: return keys
        else: return keys[-1:len(keys)-1]

def centroid(a):
        pass

def angles(origin,l1,l2):
        pass

# -------------------- Working Methods -----------------------
def webcamTest():
	"""Runs a detection test on an image and displays the result"""
	cap = cv2.VideoCapture(0) #Setup webcam

	while(True):
		ret, frame = cap.read() #Capture frame by frame
		try:
			imageTest(frame,video=True)
		except:
			pass
		if cv2.waitKey(1) & 0xFF == ord('q'): #Exit
			break

	#When everything done, release the capture
	cap.release()
	cv2.destroyAllWindows()

def imageTest(img, video = False):
	"""Runs a detection test on an image and displays the result"""
	fp, shapes = detect(img)
	out = drawOutlines(img,fp,shapes)
	corners = findCorners(fp)
	sort = sortCorners(corners)
	warp = fixPerspective(sort)
	cv2.imshow('img',warp)
	if not video: cv2.waitKey(0)

def detect(img):
	"""Detects the polygons and returns their focus points along with their classifying shapes"""
	polys, heir = findPolys(img)
	fp = findFocusPoints(polys, heir)
	shapes = map(lambda z: len(z),fp) #Classify by length
	return fp, shapes

def drawOutlines(img,fp,shapes):
	"""Draws focus point outlines on the img along with their shapes classified by colors"""
	out = img.copy()

	#Converts to color if grayscale
	if len(out[0,0])!=3:
		out = cv2.cvtColor(out,cv2.COLOR_GRAY2RGB)

	for i in range(len(fp)):
		f,s = fp[i],shapes[i]
		random.seed(s)
		color = [random.randrange(255) for x in range(3)]
		out = drawBorder(out,f,color)
	return out

if __name__ == "__main__":
	#img = importImage("C:/Users/Ryan/Documents/Workspace/openscan/assets/ScanTemplate(small).jpg")
	#imageTest(img)
	webcamTest()
