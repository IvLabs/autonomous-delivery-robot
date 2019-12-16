import torch
import torch.nn as nn
import torch.nn.functional as F

import os
from tqdm import tqdm

from helper import *
from models.enet.model import *

mean = [0.28689554, 0.32513303, 0.28389177]
std = [0.18696375, 0.19017339, 0.18720214]
transform = transforms.Compose([
                transforms.ToTensor(),
                transforms.Normalize(mean = mean, std = std)
            ])
trainset = CityscapesDataset(transform = transform)
valset = CityscapesDataset(image_path = 'validation', transform = transform)
trainloader = data.DataLoader(trainset, batch_size = 32, shuffle = True, drop_last = True)
valloader = data.DataLoader(valset, batch_size = 16, shuffle = True, drop_last = True)

net = ENet(num_classes = 1)
net.load_state_dict(torch.load('road_bce_dice.pt', map_location = 'cpu'))
net = net.to('cuda:0')

optimizer = torch.optim.Adam(net.parameters(), lr = 1e-5, weight_decay = 1e-3)
optimizer.load_state_dict(torch.load('road_bce_dice_opt.pt'))
criterion = nn.BCEWithLogitsLoss()
        
num_epochs = 10
highest_iou = 0.9188
for epoch in range(num_epochs) :
    train(model = net, train_loader = trainloader, val_loader = valloader, loss_function = criterion, optimiser = optimizer, epoch = epoch, num_epochs = num_epochs, savename = 'road_bce_dice.pt', highest_iou = highest_iou)
