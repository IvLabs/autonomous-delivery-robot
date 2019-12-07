import keras
from keras.layers import *
from keras.models import *
from keras.optimizers import *
from keras.callbacks import *
from keras import backend as keras

def initialblock(in_channels, out_channels, inp):
	
	pad1 = ZeroPadding2D(padding = (1,1))(inp)
	conv1 = Conv2D(out_channels, 3, strides = (2,2), padding = 'valid',activation = None, use_bias = False, kernel_initializer = 'glorot_uniform')(pad1)
	norm1 = BatchNormalization(axis = -1)(conv1)
	pool1 = MaxPooling2D(pool_size = (2,2), strides = 2)(inp)
	merged = Concatenate(axis = 3)([norm1, pool1])
	acti = PReLU()(merged)
	return acti