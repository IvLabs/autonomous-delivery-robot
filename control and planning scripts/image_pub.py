#!/usr/bin/env python
# license removed for brevity
import rospy, cv2
from cv_bridge import CvBridge
import std_msgs.msg
from sensor_msgs.msg import Image
image_message = Image()
bridge = CvBridge()
def img_pub():
    pub = rospy.Publisher('/segmap', Image, queue_size=1)
    rospy.init_node('seg_map', anonymous=True)
    rate = rospy.Rate(1) # 10hz
    img = cv2.imread("/home/unmesh/snap.png", 0)
    image_message = bridge.cv2_to_imgmsg(img, encoding="mono8")
    print('......')
    while not rospy.is_shutdown():
        pub.publish(image_message)
        rate.sleep()

if __name__ == '__main__':
    try:
        img_pub()
    except rospy.ROSInterruptException:
        pass