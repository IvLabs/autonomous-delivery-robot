import cv2
import numpy as np
import matplotlib.pyplot as plt


def sample_points(img, size = 10):
	blank = np.zeros(img.shape, np.uint8)
	h,w = img.shape
	indicesx = np.array(list(range(int(w / size)))) * size
	indicesy = np.array(list(range(int(h / size)))) * size
	blank[:, indicesx] = 255
	blank[indicesy, :] = 255
	cv2.imshow("", blank)
	cv2.waitKey(0)
	cv2.imwrite("sample.png", blank)
	return blank

def get_contours_list(img):
	bw = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
	_, cnts, h = cv2.findContours(bw, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
	h = np.array(h).T.reshape(4,-1)
	max_area = -1
	index = -1
	for i in range(len(cnts)):
		area = cv2.contourArea(cnts[i])
		if area > max_area:
			max_area = area
			index = i
	indices = np.where(h[3] == index)
	h,w = bw.shape	
	blank = np.zeros((h+5, w), np.uint8)				
	cv2.polylines(blank, pts=[cnts[index]], isClosed = True, color=255)				
	for indice in indices:
		if indice:
			cv2.polylines(blank, pts=[cnts[indice]], isClosed = True,  color=255)					
	mask = cv2.imread("sample.png", 0)		
	ys, xs = np.where(mask & blank == 255)
	return xs, ys, mask & blank

cap = cv2.VideoCapture("/home/aman/Downloads/output4_binaryout.avi")
ret, frame = cap.read()
xs, ys, img = get_contours_list(frame)
cv2.imshow("", img)
cv2.waitKey(0)
print(xs, ys)	
"""while True:
	ret, frame = cap.read()
	if ret:
		bw = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		_, cnts, h = cv2.findContours(bw, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
		h = np.array(h).T.reshape(4,-1)
		max_area = -1
		index = -1
		for i in range(len(cnts)):
			area = cv2.contourArea(cnts[i])
			if area > max_area:
				max_area = area
				index = i
		indices = np.where(h[3] == index)
		h,w = bw.shape	
		blank = np.zeros((h+5, w), np.uint8)				
		cv2.polylines(blank, pts=[cnts[index]], isClosed = True, color=255)				
		for indice in indices:
			if indice:
				print("internal_contour_present")
				cv2.polylines(blank, pts=[cnts[indice]], isClosed = True,  color=255)				
					
		mask = cv2.imread("sample.png", 0)#sample_points(blank, size = 10)		
		cv2.imshow("2", blank & mask)
		print(cv2.countNonZero(blank & mask))		
		k = cv2.waitKey(0)
		if k == 27:
			break

"""
