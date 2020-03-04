'''
@author: Unmesh Patil, Aman Jain
@project: Contour generator by merging lidar scan and seg map contour
@description: segmentation map gives binary image for road and non-road region. the contour is extracted and arbitrated.
			  but seg map is only available for limited FOV so for the rest of the surrounding liadr scan is used ,
			  overall the node publishes a contour of surrounding, in which the front contour surrounds the navigable 
			  road area and rest of the contour surrounds all area unless there's any obstacle.
input:: lidar scan
		segmentation map image

'''
######################################################### imports and globals

import math
import numpy as np
import rospy, cv2
import sys
from cv_bridge import CvBridge, CvBridgeError
from opencv_apps.msg import Contour
from sensor_msgs.msg import Image
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Odometry

range_list = []
segmap_contour = []
merged_contour = []
old_list = []
img = Image()
curr_pose = Odometry()

######################################################### callacks

def callback_image(data):
	img = data
	print('img done')
	bridge = CvBridge()
	camera_img = bridge.imgmsg_to_cv2(img, 'mono8')
	xs, ys, mask_blank = get_contours_list(camera_img)
	i = 0
	for i in range(len(xs)-1):
		point = xs[i], ys[i]
		segmap_contour.append(point) #list of touples
	print(len(segmap_contour))
	rospy.Subscriber("/odom", Odometry, callback_pose)
    
def callback_pose(data):
	curr_pose = data
	print('pose done')
	rospy.Subscriber("/scan", LaserScan, callback_scan_processor)

def callback_scan_processor(data):
	# the scan input is from -180 to 180 degrees. So total 360 degrees are covered in scan by 0.5 degree increment.
	# total 720 readings for 360 degrees.
	for i in range(len(data.ranges)):
		old_list.append(data.ranges[i])
	print(len(old_list))
	j = -math.pi - 0.00872664619237
	x = curr_pose.pose.pose.position.x
	y = curr_pose.pose.pose.position.y
	for i in range(0, 719):
		r = old_list[i]
		j = j +  0.00872664619237
		if j > math.pi:
			print("out of thetas")
			return
		else:
			x += r*math.cos(j)
			y += r*math.sin(j)
			point = x, y
			range_list.append(point)
	del old_list[:]
	if len(range_list) == 719:
		crop_contour()
	else:
		print('error in scan Subscriber', len(range_list))
		del range_list[:]
		return
	print('scan done')

######################################################### helping functions

def get_contours_list(bw):
	#bw = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
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
	mask = cv2.imread("/home/unmesh/sample.png", 0)		
	ys, xs = np.where(mask & blank == 255)
	mask_blank = mask & blank
	print('returning xs ys')
	return xs, ys, mask_blank


def crop_contour():
	#lidar scan gives 720 readings starting from -pi to pi and the camera scan gives input for 58 degrees.
	# it is 29 degrees left and 29 degree right of current orientation of bot.
	curr_theta = curr_pose.pose.pose.orientation.x
	start = abs(int(curr_theta + 29)*2 - 1)
	end = abs(int(curr_theta - 29)*2 - 1)
	if start > end:
		start, end = end, start
	if not (0 < start, end < 720) :
		print('error cropping')
		return 0
	for i in range(0, start):
		merged_contour.append(range_list[i])
	for i in range(0, len(segmap_contour)):
		merged_contour.append(segmap_contour[i])
	for i in range(end, len(range_list)-1):
		merged_contour.append(range_list[i])
	pub.publish(merged_contour)
	print(len(merged_contour))
	del range_list[:]
	del segmap_contour[:]
	del merged_contour[:]


######################################################### Main body
if __name__ == '__main__':
    try:
    	rospy.init_node('contour_generator_node', anonymous=True)
    	rospy.Subscriber("/segmap", Image, callback_image )
    	# rospy.Subscriber("/odom", Odometry, callback_pose)
    	# rospy.Subscriber("/scan", LaserScan, callback_scan_processor)
    	pub = rospy.Publisher('/contour_surround', Contour, queue_size=10)
        rospy.spin()
    except rospy.ROSInterruptException:
        pass