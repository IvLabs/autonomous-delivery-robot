'''
@author: Unmesh Patil
@project: controlling an autonomous vehicle outdoors using pure pursuit

Control api for autonomous delivery robot for vnit
input /odom and /goal_pose
output Stop factor(0/1), PWM factor (0 or 1), Servo factor (theta) as a list of three numbers
the structure is then serially communicated to arduino 

structures::::
1. curr_pose
header
	time stamp
pose (stamped vector whose frameId represents velocity)
	x
	y
	theta
	velocity v (1 or 0)

2. Waypoint (stamped pose)
	x position.x
	y position.y
	t position.z
	v orientation.x

3. Arduino struct
	bool Fwd. or reverse (1 fwd  0 rev)
	int servo angle (0 to 180 in degrees)
	bool PWM factor (0 or 1)
	bool autonomous/ manual (0 autonomous 1 manual)

rectngular velocity profile
assumes on/off control here
motor is either at full speed or zero speed no velocity profiling 
target speed and control speed assumes value of 0 or 1 only off or on

to do::::
make custom_ds, waypoints message for serial com
'''
Lookahead_distance = 2  #in meters
Max_turning_radius = 0.5 #in meters
Theta_max_abs = 60 # indicates range from -60 to 60 with zero as fwd
# car dimensions
# Base_length = 
# Wheel_radius = 
# Car_length = 

#Offsets
Turning_offset = 0
Forward_offset = 0.1
Steer_to_servo_offset = 0

steer_scaling_factor = 1  # steering angle to curvature conversion cnstant

#Defaults
max_angle_threshold = 40 # max abs possible angle between two consecutive waypoints
Servo_factor = 0
PWM_factor = 0
Stop_factor = 0

import math
import rospy
import sys
from nav_msgs.msg import Path
from geometry_msgs.msg import PoseStamped
import std_msgs.msg
from geometry_msgs.msg import Point32


#::::::::::::::::::::::::::::::: helper funcions

def Angular_deviation(xg, yg, curr_pose):
	#tells the quadrant of immediate next waypoint assuming origin at curr pose and cuu theta as pos y axis
	curr_theta = curr_pose.position.z
	curr_x = curr_pose.position.x
	curr_y = curr_pose.position.y
	slope = (math.atan2((yg - curr_y),(xg - curr_x)))*180/math.pi
	angle = slope - curr_theta
	print(angle)
	q = 0
	if angle < 90 and angle >= 0:
		q =1 # fwd left
	if angle < 180 and angle > 90:
		q =2 # rev left	
	if angle < -90 and angle >= -180:
		q =3 # rev right
	if angle < 0 and angle > -90:
		q =4 # fwd right
	return angle, q

def get_curvature_from_goal(xc, yc, xg , yg):
	#pure pursuit
	l_sq = (xg - xc)**2 + (yg - yc)**2
	if l_sq == 0:
		curvature = 1/ Max_turning_radius
	else:
		curvature = (2*abs(yg-yc)/l_sq)
	Servo_factor = curvature * steer_scaling_factor
	return Servo_factor

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::callbacks

def callback_waypoint_validator(data):
	validated_waypoints = [] #waypoint_list contains curr pose as zeroth waypoint but validated_waypoints does not
	waypoint_list = []
	for i in range(len(data.poses)):
		waypoint_list.append(data.poses[i].pose)
	#waypoint_list.append(data.poses.pose for pose in data.poses)
	waypoint = [0, 0, 0, 0, 0]
	goal_flag = 0
	if(len(waypoint_list) < 2):
		print('returning very low points only ', len(waypoint_list))
		return 0
	curr_pose = waypoint_list[0]
	i = 0
	for i in range(len(waypoint_list)-1):
		xgi = waypoint_list[i].position.x
		ygi = waypoint_list[i].position.y
		tgi = waypoint_list[i].position.z
		xgf = waypoint_list[i+1].position.x
		ygf = waypoint_list[i+1].position.y
		tgf = waypoint_list[i+1].position.z
		vgf  = waypoint_list[i+1].orientation.x
		d = math.sqrt((xgf - xgi)**2 + (ygf - ygi)**2)
		print(d, xgi, xgf, ygi, ygf)
		if(d > Lookahead_distance):
			print('bad waypoint detected', i, 'long distance')
			return 0
		theta =  (math.atan2((ygf - ygi),(xgf - xgi)))*180/math.pi
		angle = abs(tgi - theta)
		print('heee', angle, tgi, theta)
		if(abs(angle) > max_angle_threshold):
			print('bad waypoint detected', i+1, 'out of direct scope')
			return 0 # out of direct goal scope
		if(abs(tgi - tgf)> Theta_max_abs and abs(tgi - tgf)< (180 - Theta_max_abs)):
			print('bad waypoint detected', i+1, 'out of turning range')
			return 0 # out of max turnable angle
		if(i+2 == len(waypoint_list)):
			goal_flag = 1  #setting last waypoint as goal
			vgf = 0
		print('all tests passed')
		waypoint = [xgf, ygf, tgf, vgf, goal_flag]
		validated_waypoints.append(waypoint)
	waypoint_follower(validated_waypoints, curr_pose)

def waypoint_follower(validated_waypoints, curr_pose):
	
	if(len(validated_waypoints) < 1):
		print('validation killed your waypoints only', len(validated_waypoints), 'are alive')
		return False
	curr_theta = curr_pose.position.z
	curr_x = curr_pose.position.x
	curr_y = curr_pose.position.y
	angle_0, q = Angular_deviation(validated_waypoints[0][0], validated_waypoints[0][1], curr_pose )
	initial = [curr_x, curr_y, curr_theta, curr_pose.orientation.x]
	final = validated_waypoints[0]
	go_to_local_goal(initial, final, angle_0, q)

def go_to_local_goal(initial, final, angle_0, q):
	if q == 0 :
		return False
	if q == 1 or q == 4:
		fwd = 1
	if q == 2 or q == 3:
		fwd = 0 #rev
	Servo_factor_raw = get_curvature_from_goal(initial[0], initial[1], final[0], final[1])
	Servo_factor = Servo_factor_raw * steer_scaling_factor + Steer_to_servo_offset
	target_speed = final[3]
	control_speed = initial[3]
	point = Point32()
	if target_speed == 0 :
		#goal pose
		if (final[0] > (initial[0] - Forward_offset)) and (final[0] < (initial[0] + Forward_offset)):
			PWM_factor = 0
		else:
			PWM_factor = 1
	if target_speed == 1 :
		#in between pose
		PWM_factor = 1
	print(fwd, PWM_factor, Servo_factor)
	mode = 0
	point.x = fwd
	point.y = Servo_factor
	point.z = PWM_factor
	pub = rospy.Publisher('control_input', Point32, queue_size=10)
	pub.publish(point)

def sub_waypoints_out():
    rospy.init_node('control_node', anonymous=True)
    # /waypoints_out must contain curr pose as zeroth waypoint
    rospy.Subscriber("/waypoints_out", Path, callback_waypoint_validator)
    rospy.spin()

if __name__ == '__main__':
    try:
        sub_waypoints_out()
    except rospy.ROSInterruptException:
        pass