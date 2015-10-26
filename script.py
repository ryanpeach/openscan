## This script contains the main logic of the code implementing OpenCV
## It is simple and to the point, outlining all the essential features of the application
import sys
sys.path.append("C:/Users/Ryan/Documents/Workspace/openscan/.lib/OpenCV/x86")
import cv2
import numpy as np
import random

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
	for c in contours:
		temp = cv2.approxPolyDP(np.array(c),5,True)
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

def allSameLength(poly, tol=5):
	"""Checks that all lines are the same length"""
	pairs = [(poly[i-1],poly[i]) for i in range(len(poly))] 	#Get a list of all lines in poly
	lengths = map(lambda (a,b): dist(a,b), pairs)				#Get a list of the length of all pairs in poly
	mean = np.mean(lengths)										#Get the mean of all lengths in poly
	lengths = map(lambda z: np.absolute(z-mean), lengths)		#Get the error from the mean of each length
	return all(iter(map(lambda z: z<tol,lengths)))						#Return if all are within the error tolerance

def findInnerBorder(cnts):
	"""Checks shape of each contour from last to -5 and finds the first 'square.' Returns 0 if none exists."""
	for x in range(len(cnts)):
		cnt = cnts[-x]
		if cv2.isContourConvex(cnt) and len(cnt)==4 and allSameLength(cnt):
			return -x+3
	else: return 0

# --------------- Image Modification ----------------------
def fixPerspective(img, border):
	"""Returns img skewed to the border."""
	out = img.copy()
	#Get order of corners
	B = border[1]
	if dst(border[1],border[0])>dst(border[1],border[2]): A,C = border[0],border[1]
	else: A,C = border[2],border[0]

	#Set dst order to match the length ratios of previous (assume 8.5x11 page)
	src = np.array([A,B,C])
	dst = np.array([[0,1100],[0,0],[850,0]])

	#Return Affine Transform
	M = cv2.getAffineTransform(src, dst)
	out = cv2.warpAffine(out, M)
	return out, dst

def drawBorder(img, border, c=(255,0,0)):
	"""Returns img with the border drawn overlay."""
	temp = img.copy()
	out = cv2.drawContours(temp, [border], 0, color = c,thickness = 4)
	return out

def cropImage(img, border):
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
	cv2.imshow('img',out)
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
