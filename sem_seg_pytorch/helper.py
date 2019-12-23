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

def dice_loss(inp, target):
    smooth = 1.
    intersection = (torch.abs(inp * target)).sum()
    return (1 - (((2. * intersection) + smooth) / (torch.pow(inp, 2).sum() + torch.pow(target, 2).sum() + smooth)))

def iou(mask1, mask2, smooth = 1e-6) :
    pred_inds = (mask1 > 0)
    target_inds = (mask2 > 0)
    intersection = (pred_inds[target_inds]).long().sum().item()
    union = pred_inds.long().sum().item() + target_inds.long().sum().item() - intersection
    return(float(intersection + smooth) / float(union + smooth))

def train(model, train_loader, val_loader, epoch, num_epochs, loss_function, optimiser, scheduler, savename, highest_iou):
    model.train()
    losses = list()
    gpu1 = 'cuda:0'
    gpu2 = 'cuda:1'
    ious = list()
    max_iou = highest_iou
    count = 0
    savename2 = savename[ : -3] + '_opt.pt'
    loop = tqdm(train_loader)
    num_steps = len(loop)
    for data, target in loop:
        model.train()
        model = model.to(gpu1)
        data, target = data.float().to(gpu1), target.float().to(gpu1)
        
        optimiser.zero_grad()
        prediction = model(data)
        prediction = prediction.squeeze(1)
        
        loss = loss_function(prediction, target) + dice_loss(torch.sigmoid(prediction), target)
        losses.append(loss.item())
        
        loss.backward()
        optimiser.step()
        scheduler.step()
        
        loop.set_description('Epoch {}/{}'.format(epoch + 1, num_epochs))
        loop.set_postfix(loss = loss.item())
        count += 1
        if count % (num_steps // 3) == 0 :
            model.eval()
            for data, target in val_loader : 
                model = model.to(gpu1)
                data, target = data.float().to(gpu1), target.float()

                prediction = model(data)
                prediction = prediction.squeeze(1)

                ious.append(iou(target, prediction))
    
            avg_iou = sum(ious) / len(ious)

            if avg_iou > max_iou :
                max_iou = avg_iou
                torch.save(model.state_dict(), savename)
                torch.save(optimiser.state_dict(), savename2)
                print('new max_iou', max_iou)
    
            print('avg_iou: ', avg_iou)
        
    print('avg_loss: ', sum(losses) / len(losses))
    return max_iou

class CityscapesDataset(data.Dataset) :
    def __init__(self, root = '/home/himanshu/dl/dataset/cityscape/', image_path = 'image_edited', transform = None, size = 4) :
        self.img_list, self.mask_list = CityscapesDataset.get_filenames(root, image_path)
        self.transform = transform
        self.inshape = [2029 // size, 803 // size]
        # in case of odd dimensions, make it even (since network can't handle odd dimensions).
        if (self.inshape)[0] % 2 == 1 : 
        	(self.inshape)[0] -= 1
        if (self.inshape)[1] % 2 == 1 : 
        	(self.inshape)[1] -= 1
        	
        self.cv_shape = [3]
        self.cv_shape.append((self.inshape)[1])
        self.cv_shape.append((self.inshape)[0])
        
        self.pil_shape = list()
        (self.pil_shape).append((self.inshape)[0])
        (self.pil_shape).append((self.inshape)[1])
        (self.pil_shape).append(3)
        
        self.mask_shape = ((self.inshape)[1], (self.inshape)[0])
        
        self.inshape = tuple(self.inshape)
        self.pil_shape = tuple(self.pil_shape)
        self.cv_shape = tuple(self.cv_shape)
        
    def __getitem__(self, idx) :
        img = cv2.imread(self.img_list[idx])
        img = cv2.resize(img, self.inshape)
        
        mask = cv2.imread(self.mask_list[idx], cv2.IMREAD_GRAYSCALE)
        mask = cv2.resize(mask, self.inshape)
        mask = CityscapesDataset.encode_segmap(mask)
        
        if self.transform :
            img = self.transform(img)
            assert(img.shape == self.cv_shape)
        else :
            assert(img.shape == self.pil_shape)
        assert(mask.shape == self.mask_shape)
        return img, mask
    
    def __len__(self) :
        return len(self.img_list)
    
    @staticmethod
    def encode_segmap(mask) :
        '''
        During resizing of mask, the pixels other than zero (originally 255) get interpolated
        and become a range of 1 to 255, so we set all those pixels to 1 (road class)
        '''
        mask[mask < 0] = 0
        mask[mask != 0] = 1
        return mask
    
    @staticmethod
    def get_filenames(path, image_path) :
        img_list = list()
        mask_list = list()
        for filename in os.listdir(os.path.join(path, image_path)):
            img_list.append(os.path.join(path, image_path, filename))
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
