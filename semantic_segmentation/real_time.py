from model.Enet import *
from data_processing import *
import time
import numpy as np
import cv2
import datetime
def rescale_frame(frame, percent=75):
    width = int(frame.shape[1] * percent/ 100)
    height = int(frame.shape[0] * percent/ 100)
    dim = (width, height)
    return cv2.resize(frame, dim, interpolation =cv2.INTER_AREA)

model = load_model('/home/akshay/Projects/autonomous-delivery-bot/road_dicebce.hdf5', custom_objects = {'loss':loss})

#cv2.namedWindow('mask', cv2.WINDOW_NORMAL)
cap = cv2.VideoCapture('/home/akshay/Videos/output.avi')
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out_shape = (640, 480)
out = cv2.VideoWriter('/home/akshay/Videos/mask.avi',fourcc, 15, (640, 480))
alpha = 0.7
while(cap.isOpened()): 
    # Capture frame-by-frame
    ret, frame = cap.read()
    if ret == True:
        # frame = cv2.flip(frame, 0)
        # frame = cv2.flip(frame, 1)
        # frame2 = cv2.resize(frame, (640, 480))
        frame2 = frame
        # frame = cv2.resize(frame, out_shape)
        # frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # frame = rescale_frame(frame, percent = 80)

        # Our operations on the frame come here

        frame = frame / 255
        #        frame = frame[:-300, :]
        frame = cv2.resize(frame, (512,512))
        frame = np.reshape(frame, (1,) + frame.shape)
        results = model.predict(frame, batch_size = 1,verbose = 1)
        results = np.squeeze(results)
        results = results * 255
        results = results.astype(np.uint8)
        # ret2,results = cv2.threshold(results,0,255,cv2.THRESH_OTSU)

        temp = np.zeros((512,512, 3), np.uint8)
        temp[:,:,1] = results
        temp = cv2.resize(temp, (640, 480))

        # segmented = cv2.addWeighted(frame2, alpha, temp, (1-alpha), 0.0)
        out.write(temp)
        # cv2.imshow('img', segmented)
    # if (cv2.waitKey(1) & 0xFF == ord('q')):
    #     break
    if ret == False :
        break
# When everything done, release the capture
cap.release()
out.release()
cv2.destroyAllWindows()
