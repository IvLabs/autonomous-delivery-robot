import keras
from keras import losses
from keras.layers import *
from keras.models import *
from keras.optimizers import *
from keras.callbacks import *
from keras import backend as keras
from model.initialblock import *
from model.bottleneck import *
from model.asneck import *
from model.upbneck import *


def dice_coef(y_true, y_pred, smooth=1):
    """
    Dice = (2*|X & Y|)/ (|X|+ |Y|)
         =  2*sum(|A*B|)/(sum(A^2)+sum(B^2))
    ref: https://arxiv.org/pdf/1606.04797v1.pdf
    """
    intersection = K.sum(K.abs(y_true * y_pred), axis=-1)
    return (2. * intersection + smooth) / (K.sum(K.square(y_true),-1) + K.sum(K.square(y_pred),-1) + smooth)

def dice_coef_loss(y_true, y_pred):
    return 1-dice_coef(y_true, y_pred)

def loss(y_true, y_pred):
	loss = losses.binary_crossentropy(y_true, y_pred) + dice_coef_loss(y_true, y_pred)
	return loss

# def iou(true, pred):

#     intersection = true * pred

#     notTrue = 1 - true
#     union = true + (notTrue * pred)

#     return K.sum(intersection)/K.sum(union)


def Enet(input_shape = (512,512,3)):

	inputs = Input(input_shape)
	x = initialblock(in_channels = 3, out_channels = 13, inp = inputs)
	x = bottleneck(dilation = 1, inp = x, in_channel = 16, out_channel = 64, downflag = True, p= 0.01)  				#b1.0
	x = bottleneck(dilation = 1, inp = x, in_channel = 64, out_channel = 64, downflag = False, p= 0.01)  				#b1.1
	x = bottleneck(dilation = 1, inp = x, in_channel = 64, out_channel = 64, downflag = False, p= 0.01)					#b1.2
	x = bottleneck(dilation = 1, inp = x, in_channel = 64, out_channel = 64, downflag = False, p= 0.01)  				#b1.3
	x = bottleneck(dilation = 1, inp = x, in_channel = 64, out_channel = 64, downflag = False, p= 0.01)	 				#b1.4
	
	x = bottleneck(dilation = 1, inp = x, in_channel = 64, out_channel = 128, downflag = True, p= 0.1)	 				#b2.0
	x = bottleneck(dilation = 1, inp = x, in_channel = 128, out_channel = 128, downflag = False, p= 0.1)	 			#b2.1
	x = bottleneck(dilation = 2, inp = x, in_channel = 128, out_channel = 128, downflag = False, p= 0.1) 				#b2.2
	x = asneck(in_channel = 128, out_channel = 128, inp = x)	 														#basy2.3
	x = bottleneck(dilation = 4, inp = x, in_channel = 128, out_channel = 128, downflag = False, p= 0.1)	 			#b2.4
	x = bottleneck(dilation = 1, inp = x, in_channel = 128, out_channel = 128, downflag = False, p= 0.1)	 			#b2.5
	x = bottleneck(dilation = 8, inp = x, in_channel = 128, out_channel = 128, downflag = False, p= 0.1)	 			#b2.6
	x = asneck(in_channel = 128, out_channel = 128, inp = x)	 														#basy2.7
	x = bottleneck(dilation = 16, inp = x, in_channel = 128, out_channel = 128, downflag = False, p= 0.1) 				#b2.8

	x = bottleneck(dilation = 1, inp = x, in_channel = 128, out_channel = 128, downflag = False, p= 0.1)	 			#b3.1
	x = bottleneck(dilation = 2, inp = x, in_channel = 128, out_channel = 128, downflag = False, p= 0.1) 				#b3.2
	x = asneck(in_channel = 128, out_channel = 128, inp = x)	 														#basy3.3
	x = bottleneck(dilation = 4, inp = x, in_channel = 128, out_channel = 128, downflag = False, p= 0.1)	 			#b3.4
	x = bottleneck(dilation = 1, inp = x, in_channel = 128, out_channel = 128, downflag = False, p= 0.1)	 			#b3.5
	x = bottleneck(dilation = 8, inp = x, in_channel = 128, out_channel = 128, downflag = False, p= 0.1)	 			#b3.6
	x = asneck(in_channel = 128, out_channel = 128, inp = x)	 														#basy3.7
	x = bottleneck(dilation = 16, inp = x, in_channel = 128, out_channel = 128, downflag = False, p= 0.1) 				#b3.8

	x = upbneck(in_channel = 128, out_channel = 64, inp = x, relu = True, upsample = True)								#upb4.0
	x = bottleneck(dilation = 1, inp = x, in_channel = 64, out_channel = 64, downflag = False, relu = True, p= 0.1)		#upb4.1
	x = bottleneck(dilation = 1, inp = x, in_channel = 64, out_channel = 64, downflag = False, relu = True, p= 0.1)		#upb4.2

	x = upbneck(in_channel = 64, out_channel = 16, inp = x, relu = True, upsample = True)								#upb5.0
	x = bottleneck(dilation = 1, inp = x, in_channel = 16, out_channel = 16, downflag = False, relu = True, p= 0.1)		#upb5.1
	# pad1 = ZeroPadding2D(padding = (1,1))(x)
	fullconv = Conv2DTranspose(1, 3, strides = (2,2), padding = 'same', output_padding = 1, use_bias = False, activation = 'sigmoid')(x)

	model = Model(input = inputs, output = fullconv)

	model.compile(optimizer = Adam(lr = 1e-4), loss = loss, metrics = ['accuracy'])

	#model.summary()

	return model