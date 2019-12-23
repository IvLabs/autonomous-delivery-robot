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
trainset = CityscapesDataset(transform = transform, size = 2)
valset = CityscapesDataset(image_path = 'validation', transform = transform, size = 2)
trainloader = data.DataLoader(valset, batch_size = 8, shuffle = True, drop_last = True)
valloader = data.DataLoader(valset, batch_size = 4, shuffle = True, drop_last = True)

net = ENet(num_classes = 1)
# net.load_state_dict(torch.load('saved_models/new_road3.pt', map_location = 'cpu'))
net = net.to('cuda:0')

optimizer = torch.optim.Adam(net.parameters(), lr = 1e-2)
scheduler = torch.optim.lr_scheduler.OneCycleLR(optimizer, max_lr = 1e-2, steps_per_epoch = len(trainloader), epochs = 50)
# optimizer.load_state_dict(torch.load('saved_models/new_road3_opt.pt'))
criterion = nn.BCEWithLogitsLoss()
        
num_epochs = 50
highest_iou = 0
for epoch in range(num_epochs) :
    highest_iou = train(model = net, train_loader = trainloader, val_loader = valloader, loss_function = criterion, optimiser = optimizer, scheduler = scheduler, epoch = epoch, num_epochs = num_epochs, savename = 'saved_models/testing.pt', highest_iou = highest_iou)
