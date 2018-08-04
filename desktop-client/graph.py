import datetime

import numpy as np
from plotly.offline import plot
import plotly.graph_objs as go

temperatures = np.loadtxt("temperatures.npy")
timestamps = np.loadtxt("timestamps.npy")
dates = []

for ts in timestamps:
    dates.append(datetime.datetime.fromtimestamp(ts).strftime('%H:%M:%S'))

line_conf = {'shape': 'spline', 'smoothing': 1.3}
plot([go.Scatter(x=dates, y=temperatures, line=line_conf)])
