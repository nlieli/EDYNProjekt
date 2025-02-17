import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd
import os
import time
import datetime 

wd = os.path.dirname(os.path.abspath(__file__))
file_path_phi = os.path.join(wd, 'phi_field.csv')
file_path_traj = os.path.join(wd, 'particle_traj.csv')

df_phi_r = pd.read_csv(file_path_phi)
df_traj_r = pd.read_csv(file_path_traj)

t = df_traj_r.time

df_phi = df_phi_r[df_phi_r['time'] == t[0]]
df_traj = df_traj_r[df_traj_r['time'] == t[0]]

#x = df_phi.x.to_numpy()
#y = df_phi.y.to_numpy()

x_min = min(df_phi.x.to_numpy())
x_max = max(df_phi.x.to_numpy())
x_grid_points: int = int(np.sqrt(len(df_phi.x.to_numpy())))

y_min = min(df_phi.y.to_numpy())
y_max = max(df_phi.y.to_numpy())
y_grid_points: int = int(np.sqrt(len(df_phi.y.to_numpy())))

# x = np.linspace(x_max, x_min, x_grid_points)
# y = np.linspace(y_min, y_max, y_grid_points)

# x = np.linspace(x_min, x_max, x_grid_points)
# y = np.linspace(y_max, y_min, y_grid_points)

x = df_phi.x.to_numpy()
y = df_phi.y.to_numpy()
z = df_phi.value.to_numpy()

tx = df_traj.x
ty = df_traj.y
tz = df_traj.z

X, Y = np.meshgrid(x, y)
# Z = z.reshape(X.shape)

fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

(particle, ) = ax.plot3D(tx, ty, tz, 'o', color='red', zorder=10)
potential = ax.scatter(x, y, z, 'o', c=z, cmap='plasma_r')
# surf = ax.plot_surface(X, Y, Z, cmap='viridis')
ax.set_zlim(-3.5e-12, 1e-12)

def update(frame):
    # global surf
    global potential
    df_phi = df_phi_r[df_phi_r['time'] == t.iloc[frame]]
    df_traj = df_traj_r[df_traj_r['time'] == t.iloc[frame]]

    x = df_phi.x.to_numpy()
    y = df_phi.y.to_numpy()

    z = df_phi.value.to_numpy()
    # Z = z.reshape(X.shape)

    # surf.remove()
    # surf = ax.plot_surface(X, Y, Z, cmap='viridis')

    potential.remove()
    potential = ax.scatter(x, y, z, c=z, cmap='plasma_r', alpha=0.2)

    particle.set_data(df_traj.x, df_traj.y)
    particle.set_3d_properties(df_traj.z * 1e-12)

ani = animation.FuncAnimation(fig=fig, func=update, frames=len(df_traj_r), interval=1e-4)
ts = time.time()
fN = "../../graphs/AEB/AEB_example_" + datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d-%H-%M-%S') + ".gif"

# ani.save(filename=fN, writer="imagemagick", dpi=50)
plt.show()

