'''
This script detects an AprilTag on the ground, detects it pose (rotation and translation) w.r.t. camera.
Then, homography matrix is constructed using this R and T, which is used to get the ortho view using 
warpPerspective.
'''

import cv2
import matplotlib.pyplot as plt
from pupil_apriltags import Detector
import numpy as np

img = cv2.imread('left0002.jpg', cv2.IMREAD_GRAYSCALE)

detector = Detector()
# Astra's intrinsic parameters from ROS
# result = detector.detect(img, estimate_tag_pose = True, camera_params = [570.34222, 570.34222, 319.50000, 239.50000], tag_size = 0.08)

# Astra's intrinsic parameters given by Aman
result = detector.detect(img, estimate_tag_pose = True, camera_params = [518.56666108, 518.80466479, 329.45801792, 237.05589955], tag_size = 0.08)

R = result[0].pose_R
# scaling is required in the translation
T = result[0].pose_t * 1000

# creating intermediate homography matrix H' by replacing 3rd column of R with T
R[:, 2] = np.transpose(T)

# intrinsic matrix of camera
K = np.array([[518.56666108, 0., 329.45801792],
    [0., 518.80466479, 237.05589955],
    [  0., 0., 1.]])
    
# homography matrix
H = K @ R

t = np.array([[1, 0, 1500], [0, 1, 350], [0, 0, 1]])

img = cv2.imread('apriltag_ground/left0002.jpg')
out = cv2.warpPerspective(img, t@np.linalg.inv(H), (2000, 2000))

plt.imshow(out)
plt.savefig('out.png')
