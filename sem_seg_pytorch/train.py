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
dataloader = data.DataLoader(dataset, batch_size = 2, shuffle = True, drop_last = True)

net = ENet(num_classes = 1)
net = net.cuda()

optimizer = torch.optim.Adam(net.parameters(), lr = 1e-4)
criterion = nn.BCEWithLogitsLoss()

def train(model, train_loader, epoch, loss_function, optimiser):
    min_loss = 100
    model.train()
    loop = tqdm(train_loader)
    for data, target in loop:
        data, target = data.float().cuda(), target.float().cuda()
        
        optimiser.zero_grad()
        prediction = model(data)
        prediction = prediction.squeeze(1)
        
        loss = loss_function(prediction, target)
        losses.append(loss.item())
        
        loss.backward()
        optimiser.step()
        
        loop.set_description('Epoch {}/{}'.format(epoch + 1, num_epochs))
        loop.set_postfix(loss = loss.item())
        
        if (loss.item() < min_loss) :
            min_loss = loss.item()
            torch.save(net.state_dict(), 'road_bce.pt')
        
num_epochs = 10
losses = []
for epoch in range(num_epochs) :
    train(model = net, train_loader = dataloader, loss_function = criterion, optimiser = optimizer, epoch = epoch)