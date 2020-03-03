import rospy
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseWithCovarianceStamped
import csv

data_csv = [] 

def sub_waypoints_out():
    rospy.init_node('csv_appender', anonymous=True)
    rospy.Subscriber("/odom", Odometry , callback_csv_appender)
    rospy.spin()


def callback_csv_appender(data):
    x = data.pose.pose.position.x
    y = data.pose.pose.position.y
    cords = []
    cords = [str(x), str(y)]
    data_csv.append(cords)
    print(x)

sub_waypoints_out()
with open ('/home/unmesh/data_icp_out.csv', 'w') as d:
    writer1 = csv.writer(d)
    writer1.writerows(data_csv)
print("done1")
d.close()

