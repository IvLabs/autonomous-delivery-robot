import cv2
import numpy as np
import os

kernel = np.ones((3,3),np.uint8)
alpha = 0.7
for f in os.listdir('/home/himanshu/dl/ADB/vnit_test'):

	img = cv2.imread('/home/himanshu/dl/ADB/vnit_test/' + str(f))
	img = cv2.resize(img, (640,480))
	mask = cv2.imread('/home/himanshu/dl/ADB/vnit_mask/' + str(f).strip('.jpg')+ '_predict.png',0)
	# ret2,mask = cv2.threshold(mask,0,255,cv2.THRESH_BINARY)
	# mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel, iterations=5)
	temp = np.zeros((512,512, 3), np.uint8)
	temp[:,:,1] = mask
	temp = cv2.resize(temp, (img.shape[1],img.shape[0]))
	
	segmented = cv2.addWeighted(img, alpha, temp, (1-alpha), 0.0)
	#masked = cv2.bitwise_and(img,mask)
	cv2.imshow('img', segmented)
	# cv2.imwrite('/home/himanshu/dl/ADB/vnit_mask/' + str(f).strip('.jpg')+ '.png', segmented)
	cv2.waitKey(0)
	cv2.destroyAllWindows()