import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.utils import data
import torchvision
import torchvision.transforms as transforms
import os
import cv2
import numpy as np
from tqdm import tqdm

losses = list()

def dice_loss(inp, target):
    smooth = 1.

    iflat = inp.view(-1)
    tflat = target.view(-1)
    intersection = (iflat * tflat).sum()
    
    return -((2. * intersection + smooth) / (iflat.sum() + tflat.sum() + smooth))

def train(model, train_loader, epoch, num_epochs, loss_function, optimiser, savename):
    model.train()
    count = 0
    loop = tqdm(train_loader)
    for data, target in loop:
        data, target = data.float().cuda(), target.float().cuda()
        
        optimiser.zero_grad()
        prediction = model(data)
        prediction = prediction.squeeze(1)
        
        loss = loss_function(prediction, target) + dice_loss(F.softmax(prediction, dim = 1), target)
        losses.append(loss.item())
        
        loss.backward()
        optimiser.step()
        
        loop.set_description('Epoch {}/{}'.format(epoch + 1, num_epochs))
        loop.set_postfix(loss = loss.item())
        count += 1
        if count % 100 == 0 : 
            torch.save(model.state_dict(), savename)
            count = 0
            
class CityscapesDataset(data.Dataset) :
    def __init__(self, root = '/home/himanshu/dl/dataset/cityscape/', transform = None) :
        self.img_list, self.mask_list = self.get_filenames(root)
        self.transform = transform
        
    def __getitem__(self, idx) :
        img = cv2.imread(self.img_list[idx])
        img = cv2.resize(img, (2028, 802))
        
        mask = cv2.imread(self.mask_list[idx], cv2.IMREAD_GRAYSCALE)
        mask = cv2.resize(mask, (2028, 802))
        mask = self.encode_segmap(mask)
        
        if self.transform :
            img = self.transform(img)
            assert(img.shape == (3, 802, 2028))
        else :
            assert(img.shape == (802, 2028, 3))
        assert(mask.shape == (802, 2028))
        return img, mask
    
    def __len__(self) :
        return len(self.img_list)
    
    def encode_segmap(self, mask) :
        '''
        During resizing of mask, the pixels other than zero (originally 255) get interpolated
        and become a range of 1 to 255, so we set all those pixels to 1 (road class)
        '''
        mask[mask < 0] = 0
        mask[mask != 0] = 1
        return mask
    
    def get_filenames(self, path) :
        img_list = list()
        mask_list = list()
        for filename in os.listdir(os.path.join(path, 'image_edited')):
            img_list.append(os.path.join(path, 'image_edited', filename))
            mask_list.append(os.path.join(path, 'mask_edited', filename[: -15]) + 'gtCoarse_color.png')
        return img_list, mask_list
    
class UnNormalize(object):
    def __init__(self, mean, std):
        self.mean = mean
        self.std = std

    def __call__(self, tensor):
        """
        Args:
            tensor (Tensor): Tensor image of size (C, H, W) to be normalized.
        Returns:
            Tensor: Normalized image.
        """
        for t, m, s in zip(tensor, self.mean, self.std):
            t.mul_(s).add_(m)
            # The normalize code -> t.sub_(m).div_(s)
        return tensor
