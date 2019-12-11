import torch
import torch.nn as nn
import torch.nn.functional as F

from .parts import *

class ENet(nn.Module) :
    def __init__(self, num_classes = 19, encoder_relu = False, decoder_relu = True) : 
        super(ENet, self).__init__()
        self.initial_block = InitialBlock(3, 16, relu = encoder_relu)
        
        self.downsample1_0 = DownsampleBottleneck(16, 64, return_indices = True, dropout_prob = 0.01, relu = encoder_relu)
        self.regular1_1 = NormalBottleneck(64, padding = 1, dropout_prob = 0.01, relu = encoder_relu)
        self.regular1_2 = NormalBottleneck(64, padding = 1, dropout_prob = 0.01, relu = encoder_relu)
        self.regular1_3 = NormalBottleneck(64, padding = 1, dropout_prob = 0.01, relu = encoder_relu)
        self.regular1_4 = NormalBottleneck(64, padding = 1, dropout_prob = 0.01, relu = encoder_relu)
        
        self.downsample2_0 = DownsampleBottleneck(64, 128, return_indices = True, dropout_prob = 0.1, relu = encoder_relu)
        self.regular2_1 = NormalBottleneck(128, padding = 1, dropout_prob = 0.1, relu = encoder_relu)
        self.dilated2_2 = NormalBottleneck(128, dilation = 2, padding = 2, dropout_prob = 0.1, relu = encoder_relu)
        self.asymmetric2_3 = NormalBottleneck(128, kernel_size = 5, padding = 2, asymmetric = True, dropout_prob = 0.1, relu = encoder_relu)
        self.dilated2_4 = NormalBottleneck(128, dilation = 4, padding = 4, dropout_prob = 0.1, relu = encoder_relu)
        self.regular2_5 = NormalBottleneck(128, padding = 1, dropout_prob = 0.1, relu = encoder_relu)
        self.dilated2_6 = NormalBottleneck(128, dilation = 8, padding = 8, dropout_prob = 0.1, relu = encoder_relu)
        self.asymmetric2_7 = NormalBottleneck(128, kernel_size = 5, asymmetric = True, padding = 2, dropout_prob = 0.1, relu = encoder_relu)
        self.dilated2_8 = NormalBottleneck(128, dilation = 16, padding = 16, dropout_prob = 0.1, relu = encoder_relu)
        
        self.regular3_0 = NormalBottleneck(128, padding = 1, dropout_prob = 0.1, relu = encoder_relu)
        self.dilated3_1 = NormalBottleneck(128, dilation = 2, padding = 2, dropout_prob = 0.1, relu = encoder_relu)
        self.asymmetric3_2 = NormalBottleneck(128, kernel_size = 5, padding = 2, asymmetric = True, dropout_prob = 0.1, relu = encoder_relu)
        self.dilated3_3 = NormalBottleneck(128, dilation = 4, padding = 4, dropout_prob = 0.1, relu = encoder_relu)
        self.regular3_4 = NormalBottleneck(128, padding = 1, dropout_prob = 0.1, relu = encoder_relu)
        self.dilated3_5 = NormalBottleneck(128, dilation = 8, padding = 8, dropout_prob = 0.1, relu = encoder_relu)
        self.asymmetric3_6 = NormalBottleneck(128, kernel_size = 5, asymmetric = True, padding = 2, dropout_prob = 0.1, relu = encoder_relu)
        self.dilated3_7 = NormalBottleneck(128, dilation = 16, padding = 16, dropout_prob = 0.1, relu = encoder_relu)
        
        self.upsample4_0 = UpsampleBottleneck(128, 64, dropout_prob = 0.1, relu = decoder_relu)
        self.regular4_1 = NormalBottleneck(64, padding = 1, dropout_prob = 0.1, relu = decoder_relu)
        self.regular4_2 = NormalBottleneck(64, padding = 1, dropout_prob = 0.1, relu = decoder_relu)
        
        self.upsample5_0 = UpsampleBottleneck(64, 16, dropout_prob = 0.1, relu = decoder_relu)
        self.regular5_1 = NormalBottleneck(16, padding = 1, dropout_prob = 0.1, relu = decoder_relu)
        self.transposed_conv = nn.ConvTranspose2d(16, num_classes, kernel_size = 3, stride = 2, padding = 1)
        
    def forward(self, x) : 
        input_size = x.shape
        x = self.initial_block(x)
        
        stage1_input_size = x.shape
        x, max_indices1_0 = self.downsample1_0(x)
        x = self.regular1_1(x)
        x = self.regular1_2(x)
        x = self.regular1_3(x)
        x = self.regular1_4(x)
        
        stage2_input_size = x.shape
        x, max_indices2_0 = self.downsample2_0(x)
        x = self.regular2_1(x)
        x = self.dilated2_2(x)
        x = self.asymmetric2_3(x)
        x = self.dilated2_4(x)
        x = self.regular2_5(x)
        x = self.dilated2_6(x)
        x = self.asymmetric2_7(x)
        x = self.dilated2_8(x)
        
        x = self.regular3_0(x)
        x = self.dilated3_1(x)
        x = self.asymmetric3_2(x)
        x = self.dilated3_3(x)
        x = self.regular3_4(x)
        x = self.dilated3_5(x)
        x = self.asymmetric3_6(x)
        x = self.dilated3_7(x)
        
        x = self.upsample4_0(x, max_indices2_0, output_size = stage2_input_size)
        x = self.regular4_1(x)
        x = self.regular4_2(x)

        x = self.upsample5_0(x, max_indices1_0, output_size = stage1_input_size)
        x = self.regular5_1(x)
        x = self.transposed_conv(x, output_size = input_size)

        return x