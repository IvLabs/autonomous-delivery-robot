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
######################################################### imports

import math
import rospy, cv2
import sys
from cv_bridge import CvBridge, CvBridgeError

from sensor_msgs.msg import Image
from sensor_msgs.msg import LaserScan

range_list = []
segmap_contour = []
merged_contour = []

######################################################### vision functions

def callback_scan_processor(data):
	# the scan input is from -180 to 180 degrees. So total 360 degrees are covered in scan by 0.5 degree increment.
	# total 720 readings for 360 degrees.
	for i in range(len(data.ranges)):
		range_list.append(data.ranges[i])

def callback_image_processor(data):
	try:
        bridge = CvBridge()
        camera_img = bridge.imgmsg_to_cv2(data, "bgr8")
        #image_sub.unregister()
    except CvBridgeError, e:
        print e

    #further processing


def crop_contour(list_old):
	#lidar scan gives 720 readings starting from -pi to pi and the camera scan gives input for 58 degrees.
	# it is 29 degrees left and 29 degree right of current orientation of bot.
	curr_theta = curr_pose.position.z
	start_index = (curr_theta + 29)*2 - 1 
	end_index = (curr_theta - 29)*2 - 1

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