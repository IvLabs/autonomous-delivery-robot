from __future__ import print_function
from keras.preprocessing.image import ImageDataGenerator
from keras.utils import to_categorical
import numpy as np 
import os
import glob
import skimage.io as io
import skimage.transform as trans
import cv2

# np.set_printoptions(threshold=np.inf)

Ambulance = [128,128,128]
Bus = [128,0,0]
Car = [192,192,128]
Limousine = [128,64,128]
Motorcycle = [60,40,222]
Taxi = [128,128,0]
Truck = [192,128,128]
Van = [64,64,128]
# Car = [64,0,128]
# Pedestrian = [64,64,0]
# Bicyclist = [0,128,192]
# Unlabelled = [0,0,0]

COLOR_DICT = np.array([Ambulance, Bus, Car, Limousine, Motorcycle, Taxi, Truck, Van])


def adjustData(img,mask,flag_multi_class,num_class):
	if(flag_multi_class):
		img = img / 255
		batch_size =len(mask[0])
		# print(batch_size, 'mask shape', mask[0].shape)
		'''
		blacbox.shape)
		'''
		black_box = np.zeros((batch_size, 512, 512, num_class), np.uint8)

		for i, (mask,hot) in enumerate(zip(mask[0],mask[1])):
			# print('batch_size: ',i, '\n','hot: ',hot)
			for c in range(num_class):
				# print('class {}'.format(c))
				if hot[c] == 1:
					# print('found mask')
					# print(mask[:,:,0].shape,black_box[i,:,:,c].shape)
					black_box[i,:,:,c] = mask[:,:,0]
			# print(black_box.shape)
		mask = black_box
	elif(np.max(img) > 1):
		img = img / 255
		mask = mask /255
		mask[mask > 0.5] = 1
		mask[mask <= 0.5] = 0
	return (img,mask)



def trainGenerator(batch_size,train_path,image_folder,mask_folder,aug_dict,image_color_mode = "rgb",
					mask_color_mode = "grayscale",image_save_prefix  = "image",mask_save_prefix  = "mask",
					flag_multi_class = False,num_class = 1,save_to_dir = None,target_size = (256,256),seed = 1):
	'''
	can generate image and mask at the same time
	use the same seed for image_datagen and mask_datagen to ensure the transformation for image and mask is the same
	if you want to visualize the results of generator, set save_to_dir = "your path"
	'''
	image_datagen = ImageDataGenerator(**aug_dict)
	mask_datagen = ImageDataGenerator(**aug_dict)
	image_generator = image_datagen.flow_from_directory(
		train_path,
		classes = [image_folder],
		class_mode = None,
		color_mode = image_color_mode,
		target_size = target_size,
		batch_size = batch_size,
		save_to_dir = save_to_dir,
		save_prefix  = image_save_prefix,
		seed = seed)
	mask_generator = mask_datagen.flow_from_directory(
		train_path,
		classes = [mask_folder],
		class_mode = None,
		color_mode = mask_color_mode,
		target_size = target_size,
		batch_size = batch_size,
		save_to_dir = save_to_dir,
		save_prefix  = mask_save_prefix,
		seed = seed)
	train_generator = zip(image_generator, mask_generator)
	for (img,mask) in train_generator:
		img,mask = adjustData(img,mask,flag_multi_class,num_class)
		yield (img,mask)

def testGenerator(test_path,num_image,target_size = (512,512),flag_multi_class = False,as_gray = True):
	for i in range(num_image):
		img = cv2.imread(os.path.join(test_path,"%d.jpg"%i))
		img = img/255
		img = cv2.resize(img,target_size)
		#img = np.reshape(img,img.shape+(1,)) if (not flag_multi_class) else img
		img = np.reshape(img,(1,)+img.shape)
		yield img

def labelVisualize(num_class,color_dict,img):
	img = img[:,:,0] if len(img.shape) == 3 else img
	img_out = np.zeros(img.shape + (3,))
	for i in range(num_class):
		img_out[img == i,:] = color_dict[i]
	return img_out

def saveResult(save_path,npyfile,flag_multi_class = False,num_class = 2):
	for i,item in enumerate(npyfile):
		img = labelVisualize(num_class,COLOR_DICT,item) if flag_multi_class else item[:,:,0]
		io.imsave(os.path.join(save_path,"%d_predict.png"%i),img)