import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd
import os
import time
import datetime 

script_dir = os.path.dirname(os.path.abspath(__file__))
file_path_A = os.path.join(script_dir, "A_field.csv")
file_path_E = os.path.join(script_dir, "E_field.csv")
file_path_B = os.path.join(script_dir, "B_field.csv")
file_path_traj = os.path.join(script_dir, "particle_traj.csv")

df_A_r = pd.read_csv(file_path_A)
df_E_r = pd.read_csv(file_path_E)
df_B_r = pd.read_csv(file_path_B)
traj_r = pd.read_csv(file_path_traj)

t = traj_r.time

df_A = df_A_r[df_A_r['time'] == t[0]]
df_E = df_E_r[df_E_r['time'] == t[0]]
df_B = df_B_r[df_B_r['time'] == t[0]]
traj = traj_r[traj_r['time'] == t[0]]

fig, ax = plt.subplots(subplot_kw={"projection": "3d"}, figsize=(10,8))

x = df_A.x
y = df_A.y
z = df_A.z
u = df_A.xvalue
v = df_A.yvalue
w = df_A.zvalue

tx = traj.x.values
ty = traj.y.values
tz = traj.z.values

def scale_vectors(u, v, w, max_length=0.5, scale_factor=4e-10):
   
    u = u * scale_factor
    v = v * scale_factor
    w = w * scale_factor

    magnitudes = np.sqrt(u**2 + v**2 + w**2)

    exceed_mask = magnitudes > max_length

    u[exceed_mask] = u[exceed_mask] * (max_length / magnitudes[exceed_mask])
    v[exceed_mask] = v[exceed_mask] * (max_length / magnitudes[exceed_mask])
    w[exceed_mask] = w[exceed_mask] * (max_length / magnitudes[exceed_mask])

    return u, v, w

quiverA = ax.quiver(x, y, z, u, v, w, length=0.5, normalize=True)
quiverE = ax.quiver(df_E.x, df_E.y, df_E.z, df_E.xvalue, df_E.yvalue, df_E.zvalue, length=1, normalize=True, color='red')
quiverB = ax.quiver(df_B.x, df_B.y, df_B.z, df_B.xvalue, df_B.yvalue, df_B.zvalue, length=1, normalize=True, color='yellow')
# quiver.set_animated(True)
(particle,) = ax.plot3D(tx, ty, tz, 'o', color='purple')
ax.set_zlim(-5, 5)


def update(frame):
    global quiverA
    global quiverE
    global quiverB

    df_A = df_A_r[df_A_r['time'] == t.iloc[frame]]
    df_E = df_E_r[df_E_r['time'] == t.iloc[frame]]
    df_B = df_B_r[df_B_r['time'] == t.iloc[frame]]
    traj = traj_r[traj_r['time'] == t.iloc[frame]]

    u = df_A.xvalue
    v = df_A.yvalue
    w = df_A.zvalue

    uE = df_E.xvalue
    vE = df_E.yvalue
    wE = df_E.zvalue

    uB = df_B.xvalue
    vB = df_B.yvalue
    wB = df_B.zvalue

    u, v, w = scale_vectors(u, v, w, 0.5)
    uE, vE, wE = scale_vectors(uE, vE, wE, 0.5, 8e6)
    uB, vB, wB = scale_vectors(uB, vB, wB, 0.5, 8e6)

    quiverA.remove()
    quiverE.remove()
    quiverB.remove()
    quiverA = ax.quiver(df_A.x, df_A.y, df_A.z, u, v, w, length=1, normalize=False, color='blue')
    quiverE = ax.quiver(df_E.x, df_E.y, df_E.z, uE, vE, wE, length=1, normalize=False, color='red')
    quiverB = ax.quiver(df_B.x, df_B.y, df_B.z, uB, vB, wB, length=1, normalize=False, color=(0.9, 0.9, 0.0))

    particle.set_data(traj.x.values, traj.y.values)
    particle.set_3d_properties(traj.z.values)

ani = animation.FuncAnimation(fig=fig, func=update, frames=len(traj_r), interval=1e-4)
ts = time.time()
fN = "../../graphs/AEB/AEB_example_" + datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d-%H-%M-%S') + ".gif"

# ani.save(filename=fN, writer="pillow", dpi=100)
plt.show()



