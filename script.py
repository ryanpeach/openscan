## This script contains the main logic of the code implementing OpenCV
## It is simple and to the point, outlining all the essential features of the application
import cv2
import numpy as np
import random

ATOL = 10 #Degrees
DTOL = 5 #Pixels

# ------------- I/O ----------------
def importImage(filepath):
	"""Imports the img at the filepath to an RGB ndarray."""
	return cv2.imread(filepath,0)

def saveImage(img, filepath, name):
	"""Saves the img at the filepath as the given name + filetype."""
	raise IOError

# ------------- Computer Vision --------------
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
				focus.append(obj[0:len(obj)]) #Get the last 5 elements and add them as a focus point
				continue

	#Filter the focus points for their innermost border
	out,depth = [],[]
	for f in focus:
		i = findInnerBorder(f) #Checks the shapes and looks for a square, returns innermost border
		if i>0:					#If found, append to out
			out.append(f[i-2])
			depth.append(len(f)-i)

	#Return the borders
	return out, depth

# --------------- Classification Tests ------------------
def findCorners(fp):
	"""Classifies squares and selects the four most likely to be corners"""
	#Find all focus points of length four and calculate all of their angles to other fours
	fours = list(filter(lambda z: len(z)==4, fp))
	cents = map(lambda z: tuple(centroid(z)), fours)
	angles = lambda x: [angle(x,y,z) for y in cents for z in cents if x != y and y != z and x != z]

	#Classify corners as having 2 right angles
	out = []
	for c in cents:
		if len(filter(lambda z: np.absolute(z-90.0)<ATOL, angles(c)))>=2 and c not in out:
			out.append(c)

	#Return their centroids
	if len(out)!=4: raise Exception("Corners not Detected!")
	else: print out
	return out

def sortCorners(corners):
	"""Sort edges by distance."""
	cent = centroid(corners) #Get the centroid of the four corners
	polar = map(lambda z: np.angle(complex(*tuple(np.array(z)-np.array(cent))),deg=True),corners) #Get the polar angle from centroid
	sort = sorted(zip(corners,polar), key=lambda x: x[1]) #Sort by the polar coords
	keys = [x for x,y in sort] #Return just the keys

	return keys

def findInnerBorder(cnts):
	"""Checks shape of each contour from last to -5 and finds the first 'square.' Returns 0 if none exists."""
	for x in range(len(cnts)):
		cnt = cnts[-x]
		if cv2.isContourConvex(cnt) and len(cnt)==4 and allSameLength(cnt):
			return -x+3
	else: return 0

def getRef(fp,depth):
	i = depth.index(np.max(depth))
	return centroid(fp[i])

# --------------- Image Modification ----------------------
def fixPerspective(img, border,ref,ratio=8.5/11.0):
	"""Returns img skewed to the border."""
	out = img.copy()
	#Rotate the array until the reference is first
	while tuple(border[0])!=tuple(ref):
		border = rotateList(border,1)

	#Copied from http://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
	(tl, tr, br, bl) = border
	# compute the width of the new image, which will be the
	# maximum distance between bottom-right and bottom-left
	# x-coordiates or the top-right and top-left x-coordinates
	widthA = np.sqrt(((br[0] - bl[0]) ** 2) + ((br[1] - bl[1]) ** 2))
	widthB = np.sqrt(((tr[0] - tl[0]) ** 2) + ((tr[1] - tl[1]) ** 2))
	maxWidth = max(int(widthA), int(widthB))

	# compute the height of the new image, which will be the
	# maximum distance between the top-right and bottom-right
	# y-coordinates or the top-left and bottom-left y-coordinates
	heightA = np.sqrt(((tr[0] - br[0]) ** 2) + ((tr[1] - br[1]) ** 2))
	heightB = np.sqrt(((tl[0] - bl[0]) ** 2) + ((tl[1] - bl[1]) ** 2))
	maxHeight = max(int(heightA), int(heightB))

	# now that we have the dimensions of the new image, construct
	# the set of destination points to obtain a "birds eye view",
	# (i.e. top-down view) of the image, again specifying points
	# in the top-left, top-right, bottom-right, and bottom-left
	# order
	dst = np.array([
		[0, 0],
		[maxWidth - 1, 0],
		[maxWidth - 1, maxHeight - 1],
		[0, maxHeight - 1]], dtype = "float32")

	#Return Perspective Transform
	M = cv2.getPerspectiveTransform(np.array(border, dtype = "float32"), dst)
	out = cv2.warpPerspective(img.copy(), M, (maxWidth, maxHeight))
	return out

def drawBorder(img, border, c=(255,0,0)):
	"""Returns img with the border drawn overlay."""
	temp = img.copy()
	border = np.array(np.round(border), dtype=np.int32)
	out = cv2.drawContours(temp, [border], 0, color = c,thickness = 4)
	return out

def filterOut(img):
	out = cv2.cvtColor(img.copy(),cv2.COLOR_RGB2GRAY)
	out = cv2.adaptiveThreshold(out,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,11,2)
	return out

def cropImage(img,r):
	sizeX,sizeY,p = img.shape
	return img[r:sizeX-r,r:sizeY-r]

def rotateList(l,n):
    return l[n:] + l[:n]

# ----------------- Geometric Methods ---------------
def centroid(a):
	return np.array(np.round(np.mean(a, axis=0)),dtype=np.int32)

def angle(origin, c2, c3):
	a1 = np.array(list(origin))
	a2 = np.array(list(c2))
	a3 = np.array(list(origin))
	a4 = np.array(list(c3))
	return np.rad2deg(np.arccos(np.dot((a2-a1),(a4-a3))/(np.linalg.norm(a2-a1)*np.linalg.norm(a4-a3))))

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

# -------------------- Working Methods -----------------------
def webcamTest():
	"""Runs a detection test on an image and displays the result"""
	cap = cv2.VideoCapture(0) #Setup webcam

	while(True):
		ret, frame = cap.read() #Capture frame by frame
		imageTest(frame,video=True)
		if cv2.waitKey(1) & 0xFF == ord('q'): #Exit
			break

	#When everything done, release the capture
	cap.release()
	cv2.destroyAllWindows()

def imageTest(img, video = False):
	"""Runs a detection test on an image and displays the result"""
	fp, shapes, depth = detect(img)
	try:
		corners = findCorners(fp)
		sort = sortCorners(corners)
		ref = getRef(fp,depth)
		out = fixPerspective(img,sort,ref)
		out = cropImage(out,.04*out.shape[0])
		out = filterOut(out)
		cv2.imshow('img',out)
		cv2.waitKey(0)
	except:
		out = img
	cv2.imshow('img',out)
	if not video: cv2.waitKey(0)

def detect(img):
	"""Detects the polygons and returns their focus points along with their classifying shapes"""
	polys, heir = findPolys(img)
	fp,depth = findFocusPoints(polys, heir)
	shapes = map(lambda z: len(z),fp) #Classify by length
	return fp, shapes, depth

def drawOutlines(img,fp,shapes):
	"""Draws focus point outlines on the img along with their shapes classified by colors"""
	out = img.copy()

	#Converts to color if grayscale
	if isinstance(out[0,0],list) and len(out[0,0])!=3:
		out = cv2.cvtColor(out,cv2.COLOR_GRAY2RGB)

	for i in range(len(fp)):
		f,s = fp[i],shapes[i]
		random.seed(s)
		color = [random.randrange(255) for x in range(3)]
		out = drawBorder(out,f,color)
	return out

if __name__ == "__main__":
	webcamTest()
