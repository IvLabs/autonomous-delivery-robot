import cv2
import matplotlib.pyplot as plt
from pupil_apriltags import Detector
import numpy as np

cap = cv2.VideoCapture('apriltag_ground/output.avi')
fourcc = cv2.VideoWriter_fourcc(*'XVID')
writer = cv2.VideoWriter('out.avi', fourcc, 15, (2000, 2000))

detector = Detector()

while (cap.isOpened()) :
	success, img = cap.read()
	if success : 
		# using the homography matrix calculated using AprilTag on the ground
		H = np.array([[-6.13630839e+02, -2.81610381e+01,  2.51096776e+05],
					[-1.43915902e+02, 1.58572394e+02, 2.71513537e+05],
					[-5.75208061e-01, 8.16663809e-01, 6.86058534e+02]])
		# translation to get proper view
		t = np.array([[1, 0, 1500], [0, 1, 350], [0, 0, 1]])

		out = cv2.warpPerspective(img, t@np.linalg.inv(H), (2000, 2000))
		writer.write(out)
	else :
		break

writer.release()
cap.release()
cv2.destroyAllWindows()
