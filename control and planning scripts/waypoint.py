#!/usr/bin/env python
# license removed for brevity
import rospy
import std_msgs.msg
from nav_msgs.msg import Path
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Pose

def waypoints():
    pub = rospy.Publisher('/global_path', Path, queue_size=10)
    rospy.init_node('waypoints_global', anonymous=True)
    rate = rospy.Rate(1) # 10hz
    path = Path()
    path.poses = []
    for i in range(15):
        posestamped = PoseStamped()
        ipose = Pose()
        ipose.position.x = (1 + i*0.15)
        ipose.position.y = 0
        h = std_msgs.msg.Header()
        h.stamp = rospy.Time.now()
        posestamped.header = h
        posestamped.pose = ipose
        path.poses.append(posestamped)

    while not rospy.is_shutdown():
        pub.publish(path)
        rate.sleep()

if __name__ == '__main__':
    try:
        waypoints()
    except rospy.ROSInterruptException:
        pass