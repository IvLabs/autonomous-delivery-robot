# @Author: aniket
# @Date:   2019-12-17T23:28:30+05:30
# @Last modified by:   aniket
# @Last modified time: 2019-12-19T01:30:09+05:30



import roslib
import rospy
from ackermann_msgs.msg import AckermannDriveStamped
from std_msgs.msg import Int16
from ackermann_teleop.msg import cmd
from getkey import getkey, keys
import sys, select, termios, tty
import thread
from numpy import clip

control_keys = {
    'w'    : '\x77',
    's'  : '\x73',
    'd' : '\x64',
    'a'  : '\x61',
    'space' : '\x20',
    'tab'   : '\x09'}

key_bindings = {
    '\x77' : ( 1.0 , 0.0),
    '\x73' : (-1.0 , 0.0),
    '\x64' : ( 0.0 ,-1.0),
    '\x61' : ( 0.0 , 1.0),
    '\x20' : ( 0.0 , 0.0),
    '\x09' : ( 0.0 , 0.0)}

def servo_pub():

    steering_angle = 90
    speedy = 0
    msg = cmd()

    pub = rospy.Publisher('servo', cmd, queue_size = 5)
    rospy.init_node('servo_pub', anonymous = True)
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        key = getkey()
        if key in key_bindings.keys():
            if key == control_keys['s'] and speedy > -1:
                speedy = speedy - 1
            elif key == control_keys['w'] and speedy < 1:
                speedy = speedy + 1
            elif key == control_keys['a'] and steering_angle > 60 :
                steering_angle = steering_angle - 30
            elif key == control_keys['d'] and steering_angle < 120:
                steering_angle = steering_angle + 30
            elif key == control_keys['space']:
                speedy = 0
		steering_angle = 90
        elif key == '\x03' or key == '\x71':  # ctr-c or q
            break
        else:
            continue
        msg.speedy = speedy
        msg.steering_angle = steering_angle
        rospy.loginfo(msg)
        pub.publish(msg)
        rate.sleep()

if __name__ == '__main__':
    try:
        servo_pub()
    except rospy.ROSInterruptException:
        pass
