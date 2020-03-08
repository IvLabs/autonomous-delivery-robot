'''
@author: Unmesh Patil
@project: planner for autonomous delivery bot
@description: Planner assumes 4 springs and 2 sliding beads 

input:: global path assumed to be sufficiently dense or arbitrated.
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
from nav_msgs.msg import Odometry

############################################ section 2 defaults and global vars

lookahead_distance = 1.5 
swath_check_radius = 0.5
r_sq = swath_check_radius**2
d = sliding_dist = 0.15
waypoint_list_cropped = []
############################################ section 3 helper functions

def collision_checker(xs, ys, waypoint_list_cropped):
	#circle-point based checking
	for i in range(len(waypoint_list_cropped)):
		if not lookup[i]:
			x_path = waypoint_list_cropped[i].x
			y_path = waypoint_list_cropped[i].y
			for j in range(len(contour_point_list)):
				x_contour = xs[j]
				y_contour = ys[j]
				dist_sq = (x_contour - x_path)**2 + (y_contour - y_path)**2
				if (r_sq >= dist_sq):
					#sliding
					print('in here collision detected')
					slider(waypoint_list_cropped, i, x_contour, y_contour )
			lookup[i] = 1
	all_set = False
	for i in range(len(lookup)):
		if not lookup[i]:
			all_set = False
			collision_checker(xs, ys, waypoint_list_cropped)
		else:
			all_set = True
	if all_set:
		print("congratulations!!!! path found party toh banti hai")
		waypoints_out = Path()
		for i in range(len(waypoint_list_cropped)-1):
			waypoints_out.poses[i].pose.position.x = waypoint_list_cropped[i].x
			waypoints_out.poses[i].pose.position.y = waypoint_list_cropped[i].y
		pub.publish(waypoints_out)

	#once a waypoint is slided then new path is generated between itself and next global wp so we need to check 
	#for collision on that path by arbitration and again checking new path if its feasible. 
	#so the checking process dhould terminate and then restart with new pathafter sliding.
	
def slider(waypoint_list_cropped, i, x_contour, y_contour ):
		if slider_list[i]:
			if i > 1:
				slider(waypoint_list_cropped, i-1, x_contour, y_contour )
				slider_list[i] = False
				slider(waypoint_list_cropped, i, x_contour, y_contour )
			else:
				print('path not possible better stop here')
				return False
		if not slider_list[i]:
			x_path = waypoint_list_cropped[i].x
			y_path = waypoint_list_cropped[i].y
			x_pre = waypoint_list_cropped[i-1].x
			y_pre = waypoint_list_cropped[i-1].y
			slope = math.atan2((y_path - y_pre)/(x_path - x_pre))
			x_pos = x_path + d*sin(slope)
			y_pos = y_path + d*cos(slope)
			x_neg = x_path - d*sin(slope)
			y_neg = y_path - d*cos(slope)
			a, b, c = line_eq(x_path, y_path, x_pre, y_pre)
			s1 = (a*x_pos + b*y_pos - c)>0			# basically if 2 points lie on same sid eof line then they give same sign when substituted in line eq
			s2 = (a*x_contour + b*y_contour - c)>0
			diection = (s1 & s2) + ((not s1) & (not s2)) #xor gate
			if not direction :                    # means 2 points lie on opp. dir of path
				waypoint_list_cropped[i].x , waypoint_list_cropped[i].y = x_pos , y_pos
			else:
				waypoint_list_cropped[i].x , waypoint_list_cropped[i].y = x_neg , y_neg
			slider_list[i] = True
			collision_checker(xs, ys, waypoint_list_cropped)


def line_eq(x1, y1, x2, y2):
	a = y2 - y1
	b = x1 - x2
	c = a*x1 + b*y1
	print('returning line eq')
	return a, b, c
def get_nearest_global_point(waypoint_list, xc, yc):
	min_dist_sq = 1000
	for i in range(len(waypoint_list)-1):
		x_path = waypoint_list[i].x
		y_path = waypoint_list[i].y
		dist_sq = (xc - x_path)**2 + (yc - y_path)**2
		if dist_sq < min_dist_sq:
			near_index = i
	print('returning near index ',near_index)
	return near_index

# def path_optimiser(new_waypoint):

# 	pub.publish(waypoints_out)



############################################ section 4 callbacks	
def callback_odom(data_pose):
	curr_pose = PoseStamped()
	curr_pose = data_pose
	waypoint_list = []
	data = rospy.wait_for_message("/global_path", Path)
	for i in range(len(data.poses)):
		waypoint_list.append(data.poses[i].pose.position) #assumes the path is arbitrated
	contour_points = Contour()
	print('recieved odom and global_path')
	xc = curr_pose.pose.pose.position.x
	yc = curr_pose.pose.pose.position.y
	near_index = get_nearest_global_point(waypoint_list, xc, yc)
	waypoint_list_cropped = []
	for i in range(int(lookahead_distance/0.15)):
		#assumes points are at distance of 15 cm each considers 10 points in sequence so plan for 1.5 m ahead of curr_pose
		if len(waypoint_list) <= (near_index + i):
			waypoint_list_cropped.append(waypoint_list[near_index + i])
		else:
			print('not enough points but still continuing')
			break
	contour_points = rospy.wait_for_message("/contour_surround", Contour)
	xs = []
	ys = []
	for i in range(len(contour_points.points)-1):
		xs.append(contour_points.points[i].x)
		ys.append(contour_points.points[i].y)
	new_waypoints = []
	collision_checker(xs, ys, waypoint_list_cropped)
	del waypoint_list[:]
	#path_optimiser(new_waypoints)
	
############################################ section 5 main body

if __name__ == '__main__':
    try:
    	rospy.init_node('planner_node', anonymous=True)
    	slider_list = [False for i in range(len(waypoint_list_cropped))]
    	lookup = [0 for i in range(len(waypoint_list_cropped))]
    	rospy.Subscriber("/odom", Odometry, callback_odom)
    	pub = rospy.Publisher('/waypoints_out', Path, queue_size=10)
    	rospy.spin()
    except rospy.ROSInterruptException:
        pass
