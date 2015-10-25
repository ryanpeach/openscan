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
		temp = cv2.approxPolyDP(c,0.02,True)
		polys.append(temp[:,0])
	return polys, heirarchy

#Code copied and modified from https://github.com/bharathp666/opencv_qr
def findFocusPoints(polys, heirarchy, poly1 = 4, poly2 = 4, num = 4):
	"""Returns Border Poly. Checks that the QR info matches the border found."""
	img = img.copy()

	# Start processing the contour data

	# Find Three repeatedly enclosed contours A,B,C
	# NOTE: 1. Contour enclosing other contours is assumed to be the three Alignment markings of the QR code.
	# 2. Alternately, the Ratio of areas of the "concentric" squares can also be used for identifying base Alignment markers.
	# The below demonstrates the first method

	focus = []
	for i in range(len(contours)):
		k, obj = i, []

		#Add all k indexes to obj
		while hierarchy[k][2] != -1:
			k = hierarchy[k][2]
			obj.append(k)
		if hierarchy[k][2] != -1: obj.append(k)

		#Check if the polys are of the right shape if there are enough of them
		if c >= 5 and checkConvex(obj[-1]) and checkConvex(obj[-3]) and len(obj[-1])==poly2 and len(obj[-3])==poly1:
			focus.append(obj)

	#if mark == num: return [np.mean(i, axis = 1) for i in icnts]
	if len(focus) == num: return focus
	elif len(focus) > num: raise Exception("Too many found!")
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
	cv2.drawContours(out, border)
	return out

def cropImage(img, border):
	pass

# -------------------- Working Methods -----------------------
def webcamTest():
	cap = cv2.VideoCapture(0) #Setup webcam

	while(True):
		ret, frame = cap.read() #Capture frame by frame
		out = detect(frame) #Detection method
		cv2.imshow('frame',out) #Display
		if cv2.waitKey(1) & 0xFF == ord('q'): #Exit
			break

	#When everything done, release the capture
	cap.release()
	cv2.destroyAllWindows()

def detect(img):
	polys, heir = findContours(img)
	fp = findFocusPoints(polys, heir)
	for f in fp:
		out = drawBorder(out,f[-3])
	return out

if __name__ == "__main__":
	webcamTest()
