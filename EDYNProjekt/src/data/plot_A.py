import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
file_path_A = os.path.join(script_dir, "A_field.csv")

df_A = pd.read_csv(file_path_A)
df_A.replace('-nan(ind)', 1000, inplace=True)
df_A = df_A[df_A['time'] == 0]

ax = plt.figure().add_subplot(projection='3d')

x = df_A.x
y = df_A.y
z = df_A.z

u = df_A.xvalue
v = df_A.yvalue
w = df_A.zvalue

ax.quiver(x, y, z, u, v, w, length=1, normalize=True)
ax.set_zlim(-2, 2)

plt.show()

