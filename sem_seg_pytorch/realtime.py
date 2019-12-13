import torch
import torch.nn as nn
import torch.nn.functional as F

import os
import cv2
from PIL import Image

from helper import *
from models.enet.model import *

torch.cuda.set_device(0)

alpha = 0.7
mean = [0.28689554, 0.32513303, 0.28389177]
std = [0.18696375, 0.19017339, 0.18720214]
transform = transforms.Compose([
                transforms.ToTensor(),
                transforms.Normalize(mean = mean, std = std)
            ])

net = ENet(num_classes = 1)
net.load_state_dict(torch.load('road_bce_dice.pt', map_location = 'cpu'))
net.cuda()
net.eval()

cap = cv2.VideoCapture('test1.mp4')
fourcc = cv2.VideoWriter_fourcc(*'XVID')
writer = cv2.VideoWriter('out.avi', fourcc, 30, (640, 320))

while(cap.isOpened()) :
    ret, frame = cap.read()
    if ret == True :
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
        out = out.float().numpy()
        
        temp = np.zeros(frame.shape, np.uint8)
        temp[:, :, 1] = out[0] * 255
        
        segmented = cv2.addWeighted(frame, alpha, temp, (1 - alpha), 0.0)
        segmented = cv2.resize(segmented, (640, 320))
#         cv2.imshow('segmentation_out', segmented)
        writer.write(segmented)
    
    else :
        break
        
cap.release()
writer.release()
cv2.destroyAllWindows()