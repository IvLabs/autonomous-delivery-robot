'''
@author: Unmesh Patil
@project: planner for autonomous delivery bot
@description: Planner assumes 4 springs and 2 sliding beads 

input:: global path assumed to start from current pose everytime. Update global path evrytime by cropping the alradey traversed path.
		asssumption:: global path points and contour points are transformed to suitable frame.
		separate node to publish 360 contour point list from segmap and lidar inputs
'''
########################################### section 1 imports

import math
import rospy
import sys
from nav_msgs.msg import Path
from geometry_msgs.msg import PoseStamped
import std_msgs.msg
from geometry_msgs.msg import Point32
from sensor_msgs.msg import Image
from opencv_apps.msg import Contour
from opencv_apps.msg import Point2D

############################################ section 2 defaults and global vars

lookahead_distance = 1 
swath_check_radius = 0.5

waypoint_list = []
contour_point_list = []
curr_pose = PoseStamped()

############################################ section 3 helper functions

def collision_checker_slider(waypoint_list, contour_point_list):
	#circle-point based checking
	r_sq = swath_check_radius**2
	collision_flag = False
	for i in range(len(waypoint_list)):
		x_path = waypoint_list[i].x
		y_path = waypoint_list[i].y
		for j in range(len(contour_point_list)):
			x_contour = contour_point_list[j].x
			y_contour = contour_point_list[j].y
			dist_sq = (x_contour - x_path)**2 + (y_contour - y_path)**2
			if (r_sq => dist_sq):
				#sliding
				collision_flag = True
				waypoint_list[i].x = waypoint_list[i].x + 
				waypoint_list[i].y = waypoint_list[i].y +
	#once a waypoint is slided then new path is generated between itself and next global wp so we need to check 
	#for collision on that path by arbitration and again checking new path if its feasible. 
	#so the checking process dhould terminate and then restart with new pathafter sliding.
	
	pub.publish(waypoints_out)

############################################ section 4 callbacks	
def callback_waypoint_processor(data):
	for i in range(len(data.poses)):
		waypoint_list.append(data.poses[i].pose)
	curr_pose = waypoint_list[0]

	
def callback_contour_processor(data):
	for i in range(len(data.points)):
		contour_point_list.append(data.points[i])
	contour_point_list = crop_contour(contour_point_list)

############################################ section 5 main body

if __name__ == '__main__':
    try:
    	rospy.init_node('planner_node', anonymous=True)
    	rospy.Subscriber("/global_path", Path, callback_waypoint_processor)
    	#rospy.Subscriber("/robot_pose_ekf", PoseStamped) #no need as the global path always starts from current pose
    	rospy.subscriber("/contour_surround", Contour, callback_contour_processor)
    	pub = rospy.Publisher('/waypoints_out', Path, queue_size=10)
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
