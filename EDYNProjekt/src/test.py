import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import ani 

#with open('file.txt', 'r') as file:
#    for line in file:

x = [0, 1, 2]
y = [0, 1, 2]
z = [0, 1, 2]

fig, ax = plt.subplots(subplot_kw={"projection" : "3d"})
ax.plot(x, y, z)

plt.show()


