import torch
import torch.nn as nn

class GATModel(nn.Module):
    def __init__(self, in_channels, out_channels):
        super(GATModel, self).__init__()
        # Placeholder for GAT layers
        self.conv1 = nn.Linear(in_channels, out_channels)

    def forward(self, x, edge_index):
        return torch.relu(self.conv1(x))
