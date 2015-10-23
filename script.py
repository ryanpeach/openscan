## This script contains the main logic of the code implementing OpenCV
## It is simple and to the point, outlining all the essential features of the application

import cv2
import numpy as np

def importImage(filepath):
    """Imports the img at the filepath to an RGB ndarray."""
    raise IOError

def saveImage(img, filepath, name):
    """Saves the img at the filepath as the given name + filetype."""
    raise IOError

def filterImage(img):
    """Filters the img that the border is easily and uniquely visible."""
    pass

def findBorder(img, qr):
    """Returns Border Poly. Checks that the QR info matches the border found."""
    raise ValueError

def skewImage(img, border):
    """Returns img skewed inverse of the skew of the border."""
    pass

def cropImage(img, border):
    """Returns img cropped to the border. Border must be minimally skewed."""
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
Filtered = filterImage(IMG)
Border = findBorder(Filtered)
Skew, skewBorder = skewImage(Filtered,Border)
Crop = cropImage(Skew,skewBorder)

#Output Information
saveImage(Crop,savedir,name)
saveImage(drawBorder(IMG,Border),savedir,name+"(Border)")
