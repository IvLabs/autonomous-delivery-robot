#!/usr/bin/env python
# license removed for brevity
import rospy
import std_msgs.msg
from nav_msgs.msg import Path
from geometry_msgs.msg import PoseStamped

def waypoints():
    pub = rospy.Publisher('waypoints_out', Path, queue_size=10)
    rospy.init_node('waypoints', anonymous=True)
    rate = rospy.Rate(1) # 10hz
    pose = PoseStamped()
    pose1 = PoseStamped()
    path = Path()
    path.poses = []
    h = std_msgs.msg.Header()
    pose.pose.position.x = 0
    pose.pose.position.y = 0
    pose.pose.position.z = 0
    pose.pose.orientation.x = 0
    h.stamp = rospy.Time.now()
    pose.header = h
    path.poses.append(pose)

    pose1.pose.position.x = 0
    pose1.pose.position.y = 1
    pose1.pose.position.z = 0
    pose1.pose.orientation.x = 0
    h.stamp = rospy.Time.now()
    pose1.header = h
    path.poses.append(pose1)

    while not rospy.is_shutdown():
        pub.publish(path)
        rate.sleep()

if __name__ == '__main__':
    try:
        waypoints()
    except rospy.ROSInterruptException:
        pass