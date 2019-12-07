import keras
from keras.layers import *
from keras.models import *
from keras.optimizers import *
from keras.callbacks import *
from keras import backend as keras

def upbneck(in_channel, out_channel, inp, relu = False, projection_ratio = 4, upsample = False):
	#inp = Input(shape = (64,64,128))
	temp = inp

	# side branch
	reduced_depth = int(in_channel/projection_ratio)
	convt1 = Conv2DTranspose(reduced_depth, 1, padding = 'valid', activation = None, use_bias = False, kernel_initializer = 'glorot_uniform')(inp)
	norm1 = BatchNormalization(axis = -1)(convt1)
	acti1 = ReLU()(norm1)  #(?,256,,256,64)

	convt2 = Conv2DTranspose(reduced_depth, 3, strides = (2,2), padding = 'same', output_padding = 1, activation = None, use_bias = False, kernel_initializer = 'glorot_uniform')(acti1)
	norm2 = BatchNormalization(axis = -1)(convt2)
	acti2 = ReLU()(norm2)   #(?,128,128,64)

	convt3 = Conv2DTranspose(out_channel, 1, padding = 'valid', activation = None, use_bias = False, kernel_initializer = 'glorot_uniform')(acti2)
	norm3 = BatchNormalization(axis = -1)(convt3)
	drop = SpatialDropout2D(0.1)(norm3) #(?,128,128,64)
	
	# main branch

	temp = Conv2D(out_channel, 1, padding = 'valid', activation = None, use_bias = False, kernel_initializer = 'glorot_uniform')(UpSampling2D(size = (2,2))(temp))
		
	merge = Add()([drop, temp])
	acti3 = ReLU()(merge)
	return acti3
#upbneck(128,64, True)

