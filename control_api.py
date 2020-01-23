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

2. Waypoint (stamped vector whose frameId represents velocity)
	x
	y
	t
	v 

3. Arduino struct
	bool start flag (1)
	bool Fwd. or reverse (1 fwd  0 rev)
	int servo angle (0 to 180 in degrees)
	bool PWM factor (0 or 1)
	bool emergency stop (1 emergency & 0 no emergency)
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
max_angle_threshold = 90 # max abs possible angle between two consecutive waypoints
Servo_factor = 0
PWM_factor = 0
Stop_factor = 0

import math
import rospy
import sys
from nav_msgs.msg import Path


#::::::::::::::::::::::::::::::: helper funcions

def Angular_deviation(xg, yg, curr_pose):
	#tells the quadrant of immediate next waypoint assuming origin at curr pose and cuu theta as pos y axis
	curr_theta = curr_pose.pose.yaw
	curr_x = curr_pose.pose.x
	curr_y = curr_pose.pose.y
	slope = (math.atan2((yg - curr_y),(xg - curr_x)))*180/math.pi
	angle = slope - curr_theta
	if angle < 90 and angle > 0:
		q =1 # fwd left
	if angle < 180 and angle > 90:
		q =2 # rev left	
	if angle < -90 and angle > -180:
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
		curvature = (2*abs(xg-xc)/l_sq)
	Servo_factor = curvature * steer_scaling_factor
	return Servo_factor

#:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::callbacks

def callback_waypoint_validator(data):
	validated_waypoints = [] #waypoint_list contains curr pose as zeroth waypoint but validated_waypoints does not
	waypoint_list = [data.poses.pose for pose in data.poses.pose]
	waypoint = [0, 0, 0, 0]
	goal_flag = 0
	if(len(waypoint_list) < 2):
		return false
	curr_pose = waypoint_list[0]
	i = 0
	for i in range(len(waypoint_list)-2):
		xgi = waypoint_list[i].x
		ygi = waypoint_list[i].y
		tgi = waypoint_list[i].t
		xgf = waypoint_list[i+1].x
		ygf = waypoint_list[i+1].y
		tgf = waypoint_list[i+1].t
		vgf  = waypoint_list[i+1].v
		d = math.sqrt((xgf - xgi)**2 + (ygf - ygi)**2)
		if(d > Lookahead_distance):
			print('bad waypoint detected'+ waypoint_list[i+1]+ 'long distance')
			return 0
		theta =  atan2((ygf - ygi),(xgf - xgi))
		angle = abs(tgi - theta)
		if(angle > min_angle_threshold and angle < max_angle_threshold):
			print('bad waypoint detected'+ waypoint_list[i+1] + 'out of direct scope')
			return 0 # out of direct goal scope
		if(abs(tgi - tgf)> Theta_max_abs and abs(tgi - tgf)< (180 - Theta_max_abs)):
			print('bad waypoint detected'+ waypoint_list[i+1] + 'out of turning range')
			return 0 # out of max turnable angle
		if(i+2 == len(waypoint_list)):
			goal_flag = 1  #setting last waypoint as goal
			vgf = 0
		waypoint = [xgf, ygf, tgf, vgf, goal_flag]
		validated_waypoints.append(waypoint)
	waypoint_follower(validated_waypoints, curr_pose)

def waypoint_follower(validated_waypoints, curr_pose):
	
	if(len(validated_waypoints) < 2):
		return false
	curr_theta = curr_pose.pose.yaw
	curr_x = curr_pose.pose.x
	curr_y = curr_pose.pose.y
	angle_0, q = Angular_deviation(validated_waypoints[0][0] , validated_waypoints[0][1], curr_pose )
	initial = [curr_x, curr_y, curr_theta, curr_pose.velocity.v]
	final = validated_waypoints[0]
	go_to_local_goal(initial, final, angle_0, q)

def go_to_local_goal(initial, final, angle_0, q):
	if q == 1 or q == 4:
		fwd = 1
	if q == 2 or q == 3:
		fwd = 0 #rev
	Servo_factor_raw = get_curvature_from_goal(initial[0], initial[1], final[0], final[1])
	Servo_factor = Servo_factor_raw * steer_scaling_factor + Steer_to_servo_offset
	target_speed = final[3]
	control_speed = initial[3]
	if target_speed == 0 :
		#goal pose
		if (final[0] > (initial[0] - Forward_offset)) and (final[0] < (initial[0] + Forward_offset)):
			PWM_factor = 0
		else:
			PWM_factor = 1
	if target_speed == 1 :
		#in between pose
		PWM_factor = 1

	start_flag = 1
	emergency_stop = 0
	mode = 0
	Custom_ds = []
	Custom_ds = [start_flag, fwd, Servo_factor, PWM_factor, emergency_stop, mode]
	pub = rospy.Publisher('/control_api', Custom_ds, queue_size=10)
	pub.publish(Custom_ds)

def sub_waypoints_out():
    rospy.init_node('control_api', anonymous=True)
    # /waypoints_out must contain curr pose as zeroth waypoint
    rospy.Subscriber("/waypoints_out", Path, callback_waypoint_validator)
    rospy.spin()

if __name__ == '__main__':
    try:
        sub_waypoints_out()
    except rospy.ROSInterruptException:
        pass