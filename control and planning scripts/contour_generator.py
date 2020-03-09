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
from opencv_apps.msg import Point2D
from sensor_msgs.msg import Image
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Odometry


final_list = []

######################################################### callacks

def callback_pose(data):
	curr_pose = Odometry()
	curr_pose = data
	xc = curr_pose.pose.pose.position.x
	yc = curr_pose.pose.pose.position.y
	try:
		img_data = rospy.wait_for_message("/segmap", Image)
		image_proc(img_data, xc, yc)
	except Exception as e:
		raise e

def image_proc(data, xc, yc):
	global final_list
	img = Image()
	img = data
	bridge = CvBridge()
	camera_img = bridge.imgmsg_to_cv2(img, 'mono8')
	xs, ys, mask_blank = get_contours_list(camera_img)
	i = 0
	for i in range(len(xs)-1):
		point = xs[i], ys[i]
		final_list.append(point) #list of touples
	try:
		scan_data = rospy.wait_for_message("/scan", LaserScan)
		scan_processor(scan_data, xc, yc)
	except Exception as e:
		del final_list[:]
		raise e	

def scan_processor(data, xc, yc):
	global final_list
	if not len(data.ranges):
		try:
			data = rospy.wait_for_message("/scan", LaserScan, timeout = 20)
		except Exception as e:
			del final_list[:]
			raise e	
	else:
		old_list = []
		for i in range(len(data.ranges)):
			old_list.append(data.ranges[i])
		j = -math.pi - 0.00872664619237
		for i in range(0, 719):
			r = old_list[i]
			j = j +  0.00872664619237
			if j > math.pi:
				print("out of thetas")
				return
			else:
				if r:
					x = xc + r*math.cos(j)
					y = yc + r*math.sin(j)
					point = x, y
					final_list.append(point)
		del old_list[:]
		pub_contour()

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
	return xs, ys, mask_blank

def pub_contour():
	global final_list
	merged_cont = Contour()
	merged_cont.points = []
	for i in range(len(final_list)):
		point= Point2D()
		point.x = final_list[i][0]
		point.y = final_list[i][1]
		merged_cont.points.append(point)
	print('publishing')
	pub.publish(merged_cont)

######################################################### Main body
if __name__ == '__main__':
    try:
    	rospy.init_node('contour_generator_node', anonymous=True)
    	rospy.Subscriber("/odom", Odometry, callback_pose)
    	pub = rospy.Publisher('/contour_surround', Contour, queue_size=10)
        rospy.spin()
    except rospy.ROSInterruptException:
        pass