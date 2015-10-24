## This script contains the main logic of the code implementing OpenCV
## It is simple and to the point, outlining all the essential features of the application

import cv2
import numpy as np

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
	return cv2.imread(fileName,0)

def saveImage(img, filepath, name):
    """Saves the img at the filepath as the given name + filetype."""
    raise IOError

def filterImage(img):
    """Filters the img that the border is easily and uniquely visible."""
	img = img.copy()
	gray = cv2.cvtColor(img,cv2.CV_RGB2GRAY);		#Convert Image captured from Image Input to GrayScale
	edges = cv2.Canny(gray, 100 , 200, 3);			#Apply Canny edge detection on the gray image

	contours, heirarchy = cv2.findContours(edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) # Find contours with hierarchy

	#Return approximate polygons
	polys = []
	for c in contours:
		temp = cv2.approxPolyDP(c,0.02,True):
		polys.append(temp[:,0])
	return polys, heirarchy

#Code copied from https://github.com/bharathp666/opencv_qr
def findCorners(img):
	"""Returns Border Poly. Checks that the QR info matches the border found."""
	img = img.copy()
	mark = 0;	#Reset all detected marker count for this frame

	# Get Moments for all Contours and the mass centers
	mu = np.zeros(len(contours))
  	mc = np.zeros(len(contours))

	for i in range(len(contours)):
		mu[i] = cv2.moments( contours[i], false );
		mc[i] = cv2.Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 )

	# Start processing the contour data

	# Find Three repeatedly enclosed contours A,B,C
	# NOTE: 1. Contour enclosing other contours is assumed to be the three Alignment markings of the QR code.
	# 2. Alternately, the Ratio of areas of the "concentric" squares can also be used for identifying base Alignment markers.
	# The below demonstrates the first method

	A, B, C = 0, 0, 0
	for i in range(len(contours)):
		k, c = i, 0;

		while hierarchy[k][2] != -1:
			k = hierarchy[k][2] ;
			c = c+1;

		if hierarchy[k][2] != -1: c = c+1;

		if c >= 5:
			if (mark == 0):			A = i;
			else if  (mark == 1)	B = i;		# i.e., A is already found, assign current contour to B
			else if  (mark == 2)	C = i;		# i.e., A and B are already found, assign current contour to C
			mark = mark + 1

	ax = 1
	if mark == 3: return [np.mean(contours[A], axis = ax),np.mean(contours[B], axis = ax),np.mean(contours[C], axis = ax)]
	elif mark > 3: raise Exception("Too many found!")
	else: raise Exception("Not enough markers found!")

def dist(a,b):
	"""Simple 2D Distance Formula"""
	return np.sqrt((a[0]-b[0])**2+(a[1]-b[1])**2)

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

def drawBorder(img, border):
	"""Returns img with the border drawn overlay."""
	out = img.copy()
	cv2.drawContours(out, border, color=, thickness=, lineType=)
	return out

def cropImage(img, border):
	pass
	
#User Input
filename = raw_input("Where is the file? ")
savedir = raw_input("Save Directory? ")
name = raw_input("Name (no type)? ")

#Import Image
IMG = importImage(filename)

#Crop and Skew
filtered = filterImage(IMG)
skew, skewBorder = fixPerspective(Filtered,Border)
crop = cropImage(skew,skewBorder)

#Output Information
plot(drawBorder(IMG,Border))
plot(crop)
#saveImage(Crop,savedir,name)
#saveImage(drawBorder(IMG,Border),savedir,name+"(Border)")
