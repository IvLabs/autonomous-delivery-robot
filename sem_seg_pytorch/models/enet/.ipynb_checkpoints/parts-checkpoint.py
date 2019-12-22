import torch
import torch.nn as nn
import torch.nn.functional as F

class InitialBlock(nn.Module) :
    def __init__(self, in_ch, out_ch, relu = True) : 
        super(InitialBlock, self).__init__()
        self.conv = nn.Conv2d(in_ch, out_ch - 3, kernel_size = 3, stride = 2, padding = 1, bias = False)
        self.maxpool = nn.MaxPool2d(kernel_size = 2, stride = 2)
        if relu : 
            self.out_activation = nn.ReLU()
        else : 
            self.out_activation = nn.PReLU()
        
    def forward(self, x) : 
        x1 = self.conv(x)
        x2 = self.maxpool(x)
        
        # Pad x1 to the size of x2
        diff_h = x2.shape[2] - x1.shape[2]
        diff_w = x2.shape[3] - x1.shape[3]
        
        x1 = F.pad(x1, [diff_w // 2, diff_w - diff_w // 2, diff_h // 2, diff_h - diff_h // 2])
        
        out = torch.cat([x1, x2], dim = 1)
        return out
    
class NormalBottleneck(nn.Module) :
    '''
    In a module which does not downsample, the input and output channels stay the same, only the intermediate feature map channels 
    reduce according to an internal_ratio.
    '''
    def __init__(self, in_ch, internal_ratio = 4, kernel_size = 3, padding = 0, dilation = 1, asymmetric = False, dropout_prob = 0, relu = True) :
        super(NormalBottleneck, self).__init__()
        if internal_ratio <= 1 or internal_ratio > in_ch : 
            raise RuntimeError("Value out of range. Expected value in interval [1, ", in_ch, "], got internal ratio ", internal_ratio)
        
        int_ch = in_ch // internal_ratio
        
        if relu : 
            activation = nn.ReLU
        else :
            activation = nn.PReLU
        
        # 1x1 projection convolution
        self.c1 = nn.Sequential(
            nn.Conv2d(in_ch, int_ch, kernel_size = 1, bias = False),
            nn.BatchNorm2d(int_ch),
            activation()
        )
        
        # main convolution
        if asymmetric :
            self.c2 = nn.Sequential(
                nn.Conv2d(int_ch, int_ch, kernel_size = (kernel_size, 1), padding = (padding, 0), dilation = dilation, bias = False),
                nn.BatchNorm2d(int_ch),
                activation(),
                nn.Conv2d(int_ch, int_ch, kernel_size = (1, kernel_size), padding = (0, padding), dilation = dilation, bias = False),
                nn.BatchNorm2d(int_ch),
                activation()
            )
        else :
            self.c2 = nn.Sequential(
                nn.Conv2d(int_ch, int_ch, kernel_size = kernel_size, padding = padding, dilation = dilation, bias = False), 
                nn.BatchNorm2d(int_ch),
                activation()
            )
        
        # 1x1 expansion convolution
        self.c3 = nn.Sequential(
            nn.Conv2d(int_ch, in_ch, kernel_size = 1, bias = False),
            nn.BatchNorm2d(in_ch), 
            activation()
        )
        
        self.reg = nn.Dropout2d(p = dropout_prob)
        
        self.out_activation = activation()
        
    def forward(self, x) :
        # main branch shortcut
        main = x
        
        # extension branch
        out = self.c1(x)
        out = self.c2(out)
        out = self.c3(out)
        out = self.reg(out)
        
        out = main + out
        return self.out_activation(out)

class DownsampleBottleneck(nn.Module) :
    '''
    If block has to downsample input, then main branch will have maxpooling and padding (instead of identity like in normal module)
    '''
    def __init__(self, in_ch, out_ch, internal_ratio = 4, return_indices = False, dropout_prob = 0, relu = True) :
        super(DownsampleBottleneck, self).__init__()
        self.return_indices = return_indices
        if internal_ratio <= 1 or internal_ratio > in_ch : 
            raise RuntimeError("Value out of range. Expected value in interval [1, ", in_ch, "], got internal ratio ", internal_ratio)
        
        int_ch = in_ch // internal_ratio
        
        if relu : 
            activation = nn.ReLU
        else :
            activation = nn.PReLU
         
        # main branch
        self.main1 = nn.MaxPool2d(kernel_size = 2, stride = 2, return_indices = self.return_indices)
        
        # extension branch
        # 2x2 projection convolution with stride 2
        self.ext1 = nn.Sequential(
            nn.Conv2d(in_ch, int_ch, kernel_size = 2, stride = 2, bias = False),
            nn.BatchNorm2d(int_ch), 
            activation()
        )
        
        # main convolution
        self.ext2 = nn.Sequential(
            nn.Conv2d(int_ch, int_ch, kernel_size = 3, padding = 1, bias = False),
            nn.BatchNorm2d(int_ch),
            activation()
        )
        
        # 1x1 projection convolution
        self.ext3 = nn.Sequential(
            nn.Conv2d(int_ch, out_ch, kernel_size = 1, bias = False),
            nn.BatchNorm2d(out_ch),
            activation()
        )
        
        self.ext_reg = nn.Dropout2d(p = dropout_prob)
        
        self.out_activation = activation()
        
    def forward(self, x) : 
        if self.return_indices :
            main, max_indices = self.main1(x)
        else : 
            main = self.main1(x)
            
        # extension branch
        ext = self.ext1(x)
        ext = self.ext2(ext)
        ext = self.ext3(ext)
        ext = self.ext_reg(ext)
        
        # main branch padding
        n, ch_ext, h, w = ext.shape
        ch_main = main.shape[1]
        padding = torch.zeros(n, ch_ext - ch_main, h, w)
        
        if main.is_cuda : 
            padding = padding.cuda()
            
        main = torch.cat([main, padding], dim = 1)
        out = main + ext
        
        if self.return_indices :
            return self.out_activation(out), max_indices
        else : 
            return self.out_activation(out)
        
class UpsampleBottleneck(nn.Module) :
    def __init__(self, in_ch, out_ch, internal_ratio = 4, dropout_prob = 0, relu = True) : 
        super(UpsampleBottleneck, self).__init__()
        
        if internal_ratio <= 1 or internal_ratio > in_ch : 
            raise RuntimeError("Value out of range. Expected value in interval [1, ", in_ch, "], got internal ratio ", internal_ratio)
        
        int_ch = in_ch // internal_ratio
        
        if relu : 
            activation = nn.ReLU
        else :
            activation = nn.PReLU
            
        # main branch
        self.main1 = nn.Sequential(
            nn.Conv2d(in_ch, out_ch, kernel_size = 1, bias = False),
            nn.BatchNorm2d(out_ch)
        )
        
        # kernel_size and stride are same in this by default
        self.main_unpool = nn.MaxUnpool2d(kernel_size = 2)
        
        # extension branch
        self.ext1 = nn.Sequential(
            nn.Conv2d(in_ch, int_ch, kernel_size = 1, bias = False),
            nn.BatchNorm2d(int_ch),
            activation()
        )
        
        # transposed convolution
        self.ext_tconv = nn.ConvTranspose2d(int_ch, int_ch, kernel_size = 2, stride = 2, bias = False)
        self.ext_bnorm = nn.BatchNorm2d(int_ch)
        self.ext_activation = activation()
        
        # 1x1 expansion convolution
        self.ext2 = nn.Sequential(
            nn.Conv2d(int_ch, out_ch, kernel_size = 1, bias = False),
            nn.BatchNorm2d(out_ch),
            activation()
        )
        
        self.ext_reg = nn.Dropout2d(p = dropout_prob)
        
        self.out_activation = activation()
    
    def forward(self, x, max_indices, output_size) :
        # main branch shortcut
        main = self.main1(x)
        main = self.main_unpool(main, max_indices, output_size = output_size)
        
        # extension branch
        ext = self.ext1(x)
        ext = self.ext_tconv(ext, output_size = output_size)
        ext = self.ext_bnorm(ext)
        ext = self.ext_activation(ext)
        ext = self.ext2(ext)
        ext = self.ext_reg(ext)
        
        out = main + ext
        return self.out_activation(out)