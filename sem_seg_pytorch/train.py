import torch
import torch.nn as nn
import torch.nn.functional as F

import os
from tqdm import tqdm

from helper import *
from models.enet.model import *

torch.cuda.set_device(0)

mean = [0.28689554, 0.32513303, 0.28389177]
std = [0.18696375, 0.19017339, 0.18720214]
transform = transforms.Compose([
                transforms.ToTensor(),
                transforms.Normalize(mean = mean, std = std)
            ])
dataset = CityscapesDataset(transform = transform)
dataloader = data.DataLoader(dataset, batch_size = 3, shuffle = True, drop_last = True)

net = ENet(num_classes = 1)
net = net.cuda()

optimizer = torch.optim.Adam(net.parameters(), lr = 1e-4)
criterion = nn.BCEWithLogitsLoss()
        
num_epochs = 5
loss_min = 100
for epoch in range(num_epochs) :
    train(model = net, train_loader = dataloader, loss_function = criterion, optimiser = optimizer, epoch = epoch, num_epochs = num_epochs, savename = 'road_bce_dice.pt')