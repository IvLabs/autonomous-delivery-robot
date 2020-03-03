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
import rospy, cv2
import sys
from cv_bridge import CvBridge, CvBridgeError

from sensor_msgs.msg import Image
from sensor_msgs.msg import LaserScan

range_list = []
segmap_contour = []
merged_contour = []

######################################################### callacks

def callback_scan_processor(data):
	# the scan input is from -180 to 180 degrees. So total 360 degrees are covered in scan by 0.5 degree increment.
	# total 720 readings for 360 degrees.
	for i in range(len(data.ranges)):
		range_list.append(data.ranges[i])
	print(len(range_list))

def callback_image_processor(data):
	try:
        bridge = CvBridge()
        camera_img = bridge.imgmsg_to_cv2(data, "bgr8")
        #image_sub.unregister()
    except CvBridgeError, e:
        print e
    xs, ys, mask_blank = get_contours_list(camera_img)
    #further processing
    for i in range(xs):
    	point = xs[i], ys[i]
    	segmap_contour.append(point) #list of touples

######################################################### helping functions

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
	mask_blank = mask & blank
	return xs, ys, mask_blank


def crop_contour():
	#lidar scan gives 720 readings starting from -pi to pi and the camera scan gives input for 58 degrees.
	# it is 29 degrees left and 29 degree right of current orientation of bot.
	curr_theta = curr_pose.position.z
	start = abs(int(curr_theta + 29)*2 - 1)
	end = abs(int(curr_theta - 29)*2 - 1)
	if start > end:
		start, end = end, start
	if not 0 < start, end < 720:
		print('error cropping')
		return 0
	for i in range(0, start):
		merged_contour[i] = range_list[i]
	for i in range(0, len(segmap_contour)):
		merged_contour[i + start] = segmap_contour[i]
	for i in range(end, len(range_list)-1):
		merged_contour[i + len(segmap_contour)] = range_list[i]
	return merged_contour


######################################################### Main body
if __name__ == '__main__':
    try:
    	rospy.init_node('contour_generator_node', anonymous=True)
    	rospy.Subscriber("/scan", LaserScan, callback_scan_processor)
    	rospy.Subscriber("/segmap", Image, callback_image_processor)
    	pub = rospy.Publisher('contour_surround', Contour, queue_size=10)
        rospy.spin()
    except rospy.ROSInterruptException:
        pass