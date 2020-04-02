'''
@author: Unmesh Patil
@project: planner for autonomous delivery bot
@description: Planner assumes 4 springs and 2 sliding beads 

input:: global path assumed to be sufficiently dense or arbitrated.
		separate node to publish 360 contour point list from segmap and lidar inputs
bot dimensions 34 cm x 45 cm

TODO::::  Code optimisation
'''
########################################### section 1 imports

import math, rospy, sys
from nav_msgs.msg import Path, Odometry
from geometry_msgs.msg import PoseStamped, Pose
from std_msgs.msg import Bool
from sensor_msgs.msg import Image
from opencv_apps.msg import Contour, Point2D
import matplotlib.pyplot as plt

############################################ section 2 defaults and global vars

lookahead_distance = 1.5 
swath_check_radius = 0.14
r_sq = swath_check_radius**2
d = sliding_dist = 0.15
Emergency_stop = Bool()
Emergency_stop = False
direction = False
waypoint_list_cropped = [] 
slider_list = [0]*(int(lookahead_distance / 0.15))
lookup = [0]*(int(lookahead_distance / 0.15))

############################################ section 3 helper functions

def collision_checker(xs, ys):
	global lookup
	global slider_list
	global waypoint_list_cropped
	global Emergency_stop
	#circle-point based checking
	for i in range(len(waypoint_list_cropped)):
		if not lookup[i]:
			x_path = waypoint_list_cropped[i].x
			y_path = waypoint_list_cropped[i].y
			for j in range(len(xs)):
				x_contour = xs[j]
				y_contour = ys[j]
				dist_sq = (x_contour - x_path)**2 + (y_contour - y_path)**2
				if (r_sq >= dist_sq):
					#sliding
					print('in here collision detected so sliding',i)
					all_set = slider(i, x_contour, y_contour, 1, xs, ys)
					if not all_set:
						Emergency_stop = True
						emergency.publish(Emergency_stop)
						return

			lookup[i] = 1
	for i in range(len(waypoint_list_cropped)-1):
		if not lookup[i]:  #debug
			all_set = False
			print("oops! no path found")
			return
		else:
			all_set = True
	print('inside', all_set)
	if all_set:
		x_plot = []
		y_plot = []
		print("congratulations!!!! path found")
		waypoints_out = Path()
		waypoints_out.poses = []
		for i in range(len(waypoint_list_cropped)-1):
			pose = Pose()
			pose.position.x = waypoint_list_cropped[i].x
			x_plot.append(waypoint_list_cropped[i].x)
			pose.position.y = waypoint_list_cropped[i].y
			y_plot.append(waypoint_list_cropped[i].y)
			waypoints_out.poses.append(pose)
		plt.scatter(x_plot,y_plot)
		plt.xlim(0.8, 2.4)
		plt.ylim(0.05, -2)
		plt.show()
		pub.publish(waypoints_out)
	return
	#once a waypoint is slided then new path is generated between itself and next global wp so we need to check 
	#for collision on that path by arbitration and again checking new path if its feasible. 
	#so the checking process dhould terminate and then restart with new pathafter sliding.
	
def slider( i, x_contour, y_contour, check_bool, xs, ys ):
	global lookup
	global slider_list
	global waypoint_list_cropped
	global direction
	global slope
	if slider_list[i]:
		if i > 1 and i < len(waypoint_list_cropped)-2:
			b1 = slider(i-1, x_contour, y_contour, 0, xs, ys )
			if b1: 
				b2 = slider(i+1, x_contour, y_contour, 0,xs, ys)
				if b2:
					slider_list[i] = False
					b3 = slider(i, x_contour, y_contour, 1, xs, ys )
			if not (b1 and b2 and b3):
				return False
		else:
			print('path not possible better stop here')
			return False
	if not slider_list[i]:
		if i > 0 and i < len(waypoint_list_cropped)-1:
			x_path = waypoint_list_cropped[i].x
			y_path = waypoint_list_cropped[i].y
			x_pre = waypoint_list_cropped[i-1].x
			y_pre = waypoint_list_cropped[i-1].y
			if check_bool:
				slope = math.atan2((y_path - y_pre),(x_path - x_pre))
			x_pos = x_path + d* math.sin(slope)
			y_pos = y_path + d* math.cos(slope)
			x_neg = x_path - d* math.sin(slope)
			y_neg = y_path - d* math.cos(slope)
			if check_bool:
				a, b, c = line_eq(x_path, y_path, x_pre, y_pre)
				s1 = (a*x_pos + b*y_pos - c) >= 0			# basically if 2 points lie on same sid eof line then they give same sign when substituted in line eq
				s2 = (a*x_contour + b*y_contour - c) >= 0
				direction = (s1 == s2)
			if not direction :                    # means 2 points lie on opp. dir of path
				waypoint_list_cropped[i].x , waypoint_list_cropped[i].y = x_pos , y_pos
			if direction:
				waypoint_list_cropped[i].x , waypoint_list_cropped[i].y = x_neg , y_neg
			print('slided as ',waypoint_list_cropped[i].x , waypoint_list_cropped[i].y)
			slider_list[i] = True
			if check_bool:
				collision_checker(xs, ys)
		else:
			print('too less points and that too in collision u better stop')
			return False
	return True

def line_eq(x1, y1, x2, y2):
	a = y2 - y1
	b = x1 - x2
	c = a*x1 + b*y1
	print('returning line eq')
	return a, b, c

def get_nearest_global_point(waypoint_list, xc, yc):
	min_dist_sq = 1000
	near_index = -1
	#print(xc, yc, len(waypoint_list))
	for i in range(len(waypoint_list)-1):
		x_path = waypoint_list[i].x
		y_path = waypoint_list[i].y
		dist_sq = (xc - x_path)**2 + (yc - y_path)**2
		if dist_sq < min_dist_sq:
			near_index = i
			min_dist_sq = dist_sq
	#print('returning nearest index ',near_index)
	return near_index

# def path_optimiser(new_waypoint):

# 	pub.publish(waypoints_out)



############################################ section 4 callbacks	
def callback_odom(data_pose):
	global lookup
	global slider_list
	global waypoint_list_cropped
	curr_pose = PoseStamped()
	#print(len(lookup))
	curr_pose = data_pose
	waypoint_list = []
	data = rospy.wait_for_message("/global_path", Path)
	waypoint_list.extend(data.poses[i].pose.position for i in range(len(data.poses)))
	contour_points = Contour()
	print('recieved odom and global_path')
	xc = curr_pose.pose.pose.position.x
	yc = curr_pose.pose.pose.position.y
	near_index = get_nearest_global_point(waypoint_list, xc, yc)
	waypoint_list_cropped = []
	for i in range(int(lookahead_distance / 0.15)):
		#assumes points are at distance of 15 cm each considers 10 points in sequence so plan for 1.5 m ahead of curr_pose
		if (len(waypoint_list) >= (near_index + i)) and (near_index != -1):
			waypoint_list_cropped.append(waypoint_list[near_index + i])
	if len(waypoint_list_cropped) <= 2:
		print('not enough points but still continuing')
			
	print(waypoint_list_cropped)
	
	contour_points = rospy.wait_for_message("/contour_surround", Contour)
	xs = []
	ys = []
	print(contour_points)
	for i in range(len(contour_points.points)-1):
		xs.append(contour_points.points[i].x)
		ys.append(contour_points.points[i].y)
	#plt.scatter(xs, ys)
	#plt.show()
	print('going to checker ##################')
	collision_checker(xs, ys)
	print("back home #########################")
	slider_list = [0]*(int(lookahead_distance / 0.15))
	lookup = [0]*(int(lookahead_distance / 0.15))
	del waypoint_list[:]
	#path_optimiser(new_waypoints)
	
############################################ section 5 main body

if __name__ == '__main__':
    try:
    	rospy.init_node('planner_node', anonymous=True)
    	rospy.Subscriber("/odom", Odometry, callback_odom)
    	pub = rospy.Publisher('/waypoints_out', Path, queue_size=10)
    	emergency = rospy.Publisher('/emergency', Bool, queue_size=10) 
    	rospy.spin()
    except rospy.ROSInterruptException:
        pass
