import keras
from keras.layers import *
from keras.models import *
from keras.optimizers import *
from keras.callbacks import *
from keras import backend as keras

def asneck(in_channel, out_channel, inp, projection_ratio = 4):
	temp = inp
	#side branch
	reduced_depth = int(in_channel/projection_ratio)

	conv1 = Conv2D(reduced_depth, 1, strides = (1,1), padding = 'valid', activation = None, use_bias = False, kernel_initializer = 'glorot_uniform')(inp)
	norm1 = BatchNormalization(axis = -1)(conv1)
	acti1 = PReLU()(norm1)	#(?,64,64,32)

	conv21 = Conv2D(reduced_depth, (1,5), strides = (1,1), padding = 'valid', activation = None, use_bias = False, kernel_initializer = 'glorot_uniform')(ZeroPadding2D(padding = (0,2))(acti1))
	conv22 = Conv2D(reduced_depth, (5,1), strides = (1,1), padding = 'valid', activation = None, use_bias = False, kernel_initializer = 'glorot_uniform')(ZeroPadding2D(padding = (2,0))(conv21))
	norm2 = BatchNormalization(axis = -1)(conv22)
	acti2 = PReLU()(norm2)	#(?,64,64,32)

	conv3 = Conv2D(out_channel, 1, strides = (1,1), padding = 'valid', activation = None, use_bias = False, kernel_initializer = 'glorot_uniform')(acti2)
	drop = SpatialDropout2D(0.1)(conv3)
	norm3 = BatchNormalization(axis = -1)(drop)	#(?,64,64,128)


	# main branch
	if in_channel != out_channel:
		out_shape = out_channel - in_channel
		extra = keras.zeros(shape = (inp.shape[0], norm3.shape[1], norm3.shape[2], out_shape))
		print(extra.shape)
		temp = Concatenate(axis = 3)([temp, extra])

	add = Add()([norm3, temp])
	acti3 = PReLU()(add)

	return acti3