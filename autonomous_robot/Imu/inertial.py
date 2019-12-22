# @Author: aniket
# @Date:   2019-12-23T00:02:09+05:30
# @Last modified by:   aniket
# @Last modified time: 2019-12-23T00:30:40+05:30



#!/usr/bin/env python
import rospy
from std_msgs.msg import Float32
from autonomous_robot.msg import Imu

msg = Imu()

def callback(Imu):
    rospy.loginfo(msg)

def imu():
    rospy.init_node('imu_data', anonymous=True)

    rospy.Subscriber("imu", Imu, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    imu()
