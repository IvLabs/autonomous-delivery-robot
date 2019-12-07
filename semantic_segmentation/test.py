from model.Enet import *
from data_processing import *

model = load_model('/home/himanshu/dl/ADB/road.hdf5')
testGene = testGenerator("/home/himanshu/dl/ADB/vnit_test", flag_multi_class = False, num_image = 21, as_gray = True)
results = model.predict_generator(testGene,21,verbose=1)
saveResult("/home/himanshu/dl/ADB/vnit_mask",results)
# im = cv2.imread('/home/himanshu/dl/number_plate_detection/Enet/dataset/one_image/images/Ambulance/0.jpg', 0)
# lb = cv2.resize(im, (512,512))

# cv2.imshow('0', results[0,:,:,0])
# cv2.imshow('1', results[0,:,:,1])
# cv2.imshow('2', results[0,:,:,2])
# cv2.imshow('3', results[0,:,:,3])
# cv2.imshow('4', results[0,:,:,4])
# cv2.imshow('5', results[0,:,:,5])
# cv2.imshow('6', results[0,:,:,6])
# cv2.imshow('7', results[0,:,:,7])

# cv2.waitKey(0)
# cv2.destroyAllWindows()
#saveResult("dataset/test/tested_mask/Ambulance",results,flag_multi_class = False, num_class = 1)