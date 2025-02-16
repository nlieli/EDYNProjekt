import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
file_path_A = os.path.join(script_dir, "A_field.csv")
file_path_traj = os.path.join(script_dir, "particle_traj.csv")

df_A_r = pd.read_csv(file_path_A)
traj_r = pd.read_csv(file_path_traj)

t = traj_r.time

df_A = df_A_r[df_A_r['time'] == t[0]]
traj = traj_r[traj_r['time'] == t[0]]

fig, ax = plt.subplots(subplot_kw={"projection": "3d"})

x = df_A.x
y = df_A.y
z = df_A.z
u = df_A.xvalue
v = df_A.yvalue
w = df_A.zvalue

tx = traj.x.values
ty = traj.y.values
tz = traj.z.values

quiver = ax.quiver(x, y, z, u, v, w, length=1, normalize=True)
quiver.set_animated(True)
(particle,) = ax.plot3D(tx, ty, tz, 'o', color='red')
ax.set_zlim(-5, 5)

plt.ion()

def update(frame):
    global quiver

    df_A = df_A_r[df_A_r['time'] == t.iloc[frame]]
    traj = traj_r[traj_r['time'] == t.iloc[frame]]

    x = df_A.x
    y = df_A.y
    z = df_A.z
    u = df_A.xvalue
    v = df_A.yvalue
    w = df_A.zvalue

    quiver.remove()
    quiver = ax.quiver(x, y, z, u, v, w, length=1, normalize=True)

    particle.set_data(traj.x.values, traj.y.values)
    particle.set_3d_properties(traj.z.values)

ani = animation.FuncAnimation(fig=fig, func=update, frames=len(traj_r), interval=1e-4)
plt.show()



