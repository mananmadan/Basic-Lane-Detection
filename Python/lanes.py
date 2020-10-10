import cv2
import numpy as np
import matplotlib.pyplot as plt

def canny(image):
 ## Converting into gray scale image for edge detection
 lane_image = np.copy(image)
 gray = cv2.cvtColor(lane_image,cv2.COLOR_RGB2GRAY)
 blur = cv2.GaussianBlur(gray,(5,5),0)
 ## If the pixel derivative is less than 50 reject else >150 accept
 canny = cv2.Canny(blur,50,150) 
 return canny

def region_of_interest(image):
 height = image.shape[0]
 polygons = np.array([[(200,height),(1100,height),(550,250)]])
 mask = np.zeros_like(image)
 ## mask is a black image , we will fill it with wite triangle mask
 cv2.fillPoly(mask,polygons,255)
 return mask


image = cv2.imread('test_image.jpg')
canny_o = canny(image)
mask = region_of_interest(canny_o)

## Show current output
cv2.imshow("result",mask)

## Press a key to exit
cv2.waitKey(0)
