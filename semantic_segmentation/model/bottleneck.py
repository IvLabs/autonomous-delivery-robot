import keras
from keras.layers import *
from keras.models import *
from keras.optimizers import *
from keras.callbacks import *
from keras import backend as keras

# Down-Sampling
def bottleneck(dilation, in_channel, out_channel, inp, downflag, relu = False, projection_ratio = 4, p = 0.1):
	#inp = Input(batch_shape = (12,256,256,16))
	# side branch
	temp = inp

	if downflag:
		stride = 2
		reduced_depth = int(in_channel//projection_ratio)
	else:
		stride = 1
		reduced_depth = int(out_channel//projection_ratio)
	
	conv1 = Conv2D(reduced_depth, 1, strides = (1,1), padding = 'valid', activation = None, use_bias = False, dilation_rate = 1, kernel_initializer = 'glorot_uniform')(inp)
	norm1 = BatchNormalization(axis = -1)(conv1)
	
	if relu == True:
		acti1 = ReLU()(norm1)
	else:
		acti1 = PReLU()(norm1)  #(?,256,,256,64)

	pad1 = ZeroPadding2D(padding = dilation)(acti1)
	conv2 = Conv2D(reduced_depth, 3, strides = stride, padding = 'valid', activation = None, use_bias = False, dilation_rate = dilation, kernel_initializer = 'glorot_uniform')(pad1)
	norm2 = BatchNormalization(axis = -1)(conv2)
	if relu == True:
		acti2 = ReLU()(norm2)
	else:
		acti2 = PReLU()(norm2)   #(?,128,128,64)

	conv3 = Conv2D(out_channel, 1, strides = (1,1), padding = 'valid', activation = None, use_bias = False, dilation_rate = 1, kernel_initializer = 'glorot_uniform')(acti2)
	norm3 = BatchNormalization(axis = -1)(conv3)
	drop = SpatialDropout2D(p)(norm3) #(?,128,128,64)

	# main branch
	
	
	if downflag == True:
		temp = MaxPooling2D(pool_size = (2,2), strides = 2)(temp) #(?,128,128,16)
		temp = Permute((1, 3, 2))(temp)
		pad_feature_maps = out_channel - inp.get_shape().as_list()[3]
		tb_pad = (0, 0)
		lr_pad = (0, pad_feature_maps)
		temp = ZeroPadding2D(padding=(tb_pad, lr_pad))(temp)
		temp = Permute((1, 3, 2))(temp)

	add = Add()([drop, temp])
	if relu == True:
		acti3 = ReLU()(add)
	else:
		acti3 = PReLU()(add)  #(?,128,128,64)
	return acti3

#bottleneck(dilation = 1, in_channel= 16, out_channel = 64, downflag = True)

'''
	if 	in_channel != out_channel:
		out_shape = out_channel - in_channel
		temp = ZeroPadding2D(padding = ((0,0),(0,outshape))(temp)
		#extra = keras.zeros(shape = (temp.shape[0], drop.shape[1], drop.shape[2], out_shape))
		#temp = Concatenate(axis = 3)([temp, extra]) #(?,128,128,64)
	else
'''