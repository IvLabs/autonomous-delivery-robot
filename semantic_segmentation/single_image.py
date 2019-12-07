from model.Enet import *
from data_processing import *
import time
import numpy as np
import cv2
import datetime
# out_shape = (640, 480)
scale = 0.5
out_shape = (int(1920 * scale), int(1080 * scale))
alpha = 0.7

def percentage_pixels(img):
    return np.count_nonzero(img) / (img.shape[0] * img.shape[1])

model = load_model('/home/akshay/Projects/autonomous-delivery-bot/road_dicebce.hdf5', custom_objects = {'loss':loss})

frame = cv2.imread('/home/akshay/Projects/autonomous-delivery-bot/test_images/img4.png')
frame2 = frame
frame = cv2.resize(frame, out_shape)
frame = frame / 255

frame = cv2.resize(frame, (512,512))
frame = np.reshape(frame, (1,) + frame.shape)
results = model.predict(frame, batch_size = 1,verbose = 1)
results = np.squeeze(results)
results = results * 255
results = results.astype(np.uint8)

temp = np.zeros((512,512, 3), np.uint8)
temp[:,:,1] = results

# print(temp[:, :, 1])
temp = cv2.resize(temp, (1920, 1080))
# cv2.imwrite('mask.jpg', temp)
print(percentage_pixels(temp[:, :, 1]))
segmented = cv2.addWeighted(frame2, alpha, temp, (1-alpha), 0.0)

# cv2.imshow('segmented_out', segmented)
cv2.imwrite('img4.jpg', segmented)