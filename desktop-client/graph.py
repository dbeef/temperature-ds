import datetime
import numpy as np
import plotly.graph_objs as go
from plotly.offline import plot
from scipy.interpolate import interp1d

temperatures = np.loadtxt("temperatures.npy")
timestamps = np.loadtxt("timestamps.npy")
dates = []

for ts in timestamps:
    dates.append(datetime.datetime.fromtimestamp(ts).strftime('%H:%M:%S'))

interpolated = interp1d(x=temperatures, y=timestamps).x

line_conf = {'shape': 'spline', 'smoothing': 1.3}
plot([go.Scatter(x=dates, y=interpolated, line=line_conf)])