import torch
import torch.nn as nn
import torch.nn.functional as F
import rospy
from cv_bridge import CvBridge
from sensor_msgs.msg import Image as Img_ros
import os
import cv2, rospy
from PIL import Image
import time

from helper import *
from models.enet.model import *

bridge = CvBridge()

torch.cuda.set_device(0)

alpha = 0.7
# mean = [0.28689554, 0.32513303, 0.28389177]
# std = [0.18696375, 0.19017339, 0.18720214]
mean = [0.485, 0.456, 0.406]
std = [0.229, 0.224, 0.225]
transform = transforms.Compose([
                transforms.ToTensor(),
                transforms.Normalize(mean = mean, std = std)
            ])

net = ENet(num_classes = 1)
net.load_state_dict(torch.load('/home/ivlabs/epoch2.pt', map_location = 'cpu'))
net.cuda()
net.eval()

# cap = cv2.VideoCapture(0)

# cap = cv2.VideoCapture('test1.mp4')
fourcc = cv2.VideoWriter_fourcc(*'XVID')
writer = cv2.VideoWriter('lab_out.avi', fourcc, 15, (640, 480))
def segmentation():
    while not rospy.is_shutdown() :
        # ret, frame = cap.read()
        img_msg = Img_ros()
        img_msg = rospy.wait_for_message('/camera/rgb/image_raw', Img_ros)
        frame = bridge.imgmsg_to_cv2(img_msg, 'bgr8')
        start_time = time.time()
        frame = cv2.resize(frame, (640, 480))
        frame2 = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        frame_pil = Image.fromarray(frame2)
        frame_tf = transform(frame_pil)
        frame_tf = frame_tf.cuda()
        frame_tf = frame_tf.unsqueeze(0)
        out = net(frame_tf)
        
        out = torch.sigmoid(out)
        out = out.squeeze(1).detach().cpu()
        t = torch.Tensor([0.5])
        out = (out > t)
        out = out.numpy()
        temp = np.zeros(frame.shape, np.uint8)
        temp[:, :, 1] = out[0] * 255
        out = np.transpose(out, (2, 1, 0))
        out = out.astype(np.uint8)
        end_time = time.time()

        fps = 1 / (end_time - start_time)
        print('FPS: ', fps)
        
        segmented = cv2.addWeighted(frame, alpha, temp, (1 - alpha), 0.0)
        writer.write(segmented)
        out_ros = Img_ros()
        out_ros = bridge.cv2_to_imgmsg(out, encoding="mono8")
        pub.publish(out_ros)
            
    # cap.release()
    writer.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    try:
        rospy.init_node('segmap_node', anonymous=True)
        pub = rospy.Publisher('/segmap', Img_ros, queue_size=1)
        segmentation()
    except rospy.ROSInterruptException:
        pass
