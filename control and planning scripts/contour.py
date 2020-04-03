#!/usr/bin/env python
import rospy
import std_msgs.msg
from opencv_apps.msg import Contour
from opencv_apps.msg import Point2D

def waypoints():
    pub = rospy.Publisher('/contour_surround', Contour, queue_size=5)
    rospy.init_node('contour_surround', anonymous=True)
    rate = rospy.Rate(1) # 10hz
    contour = Contour()
    contour.points = []
    for i in range(4):
        point = Point2D()
        point.x = 1.9
        point.y = 0.1*i
        contour.points.append(point)
    print(contour.points)   
    while not rospy.is_shutdown():
        pub.publish(contour)
        rate.sleep()

if __name__ == '__main__':
    try:
        waypoints()
    except rospy.ROSInterruptException:
        pass