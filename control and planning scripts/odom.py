#!/usr/bin/env python
# license removed for brevity
import rospy
import std_msgs.msg
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseWithCovariance
from geometry_msgs.msg import TwistWithCovariance
'''
header: 
  seq: 8
  stamp: 
    secs: 1581016199
    nsecs: 522254000
  frame_id: "odom"
child_frame_id: "base_link"
pose: 
  pose: 
    position: 
      x: 0.0
      y: 0.0
      z: 0.0
    orientation: 
      x: 0.0
      y: 0.0
      z: 0.0
      w: 0.0
  covariance: [9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0]
twist: 
  twist: 
    linear: 
      x: 0.0
      y: 0.0
      z: 0.0
    angular: 
      x: 0.0
      y: 0.0
      z: 0.0
  covariance: [9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0]

'''
def waypoints():
    pub = rospy.Publisher('/odom', Odometry, queue_size=1)
    rospy.init_node('odomer', anonymous=True)
    rate = rospy.Rate(1) # 10hz
    odom = Odometry()
    covarian = [9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9999.0]
    odom.child_frame_id = "odom"
    odom.pose.pose.position.x = 1.0
    odom.pose.pose.position.y = 0.0
    odom.pose.pose.orientation.x = 0.0
    odom.pose.pose.orientation.y = 0.0
    odom.pose.pose.orientation.z = 0.0
    odom.pose.pose.orientation.w = 1.0
    odom.pose.covariance = covarian
    h = std_msgs.msg.Header()
    h.stamp = rospy.Time.now()
    odom.header = h
    odom.twist.twist.linear.x = 0.0
    odom.twist.twist.linear.y = 0.0
    odom.twist.twist.linear.z = 0.0
    odom.twist.twist.angular.x = 0.0
    odom.twist.twist.angular.y = 0.0
    odom.twist.twist.angular.z = 0.0
    odom.twist.covariance = covarian
    while not rospy.is_shutdown():
        pub.publish(odom)
        rate.sleep()

if __name__ == '__main__':
    try:
        waypoints()
    except rospy.ROSInterruptException:
        pass