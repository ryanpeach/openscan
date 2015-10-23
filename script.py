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

#Should be passed contour list
#Calibrated
def poly(cnts,val = 0.02):
    out = []
    for c in cnts:
        temp = cv2.approxPolyDP(c,val,True)
        out.append(temp[:,0])
    return out

def importImage(filepath):
	"""Imports the img at the filepath to an RGB ndarray."""
	return cv2.imread(fileName,0)

def saveImage(img, filepath, name):
    """Saves the img at the filepath as the given name + filetype."""
    raise IOError

def filterImage(img):
    """Filters the img that the border is easily and uniquely visible."""
    pass

def findBorder(img, qr):
	"""Returns Border Poly. Checks that the QR info matches the border found."""
	temp = np.copy(img)
	ret,thresh = cv2.threshold(temp,0.001*temp.max(),255,cv2.THRESH_BINARY_INV)
	contours = cv2.findContours(thresh,cv2.RETR_CCOMP,cv2.CHAIN_APPROX_SIMPLE)
	#Find Polygons
	polys = []
	for c in cnts:
		temp = cv2.approxPolyDP(c,val,True)
		polys.append(temp[:,0])
	#Find Rectangles
	rect = []
	for p in polys:
		if len(p)==4:
			rect.append(p)

def fixPerspective(img, border):
	"""Returns img skewed to the border."""
	pass

def drawBorder(img, border):
	"""Returns img with the border drawn overlay."""
	pass

def getQR(img):
	"""Returns QR info, sharpness, skew, orientation, etc..."""
	raise ValueError

#User Input
filename = raw_input("Where is the file? ")
savedir = raw_input("Save Directory? ")
name = raw_input("Name (no type)? ")

#Import Image
IMG = importImage(filename)

#Print QR Informatoin
print getQR(IMG)

#Crop and Skew
filtered = filterImage(IMG)
skew, skewBorder = fixPerspective(Filtered,Border)
crop = cropImage(Skew,skewBorder)

#Output Information
saveImage(Crop,savedir,name)
saveImage(drawBorder(IMG,Border),savedir,name+"(Border)")
