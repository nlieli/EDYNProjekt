import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os

def plot_phi():
    wd = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(wd, 'phi_field.csv')

    df = pd.read_csv(file_path)
    df = df[df['time'] == 0.0]
    #df.replace('-nan(ind)', 0, inplace=True)

    x = df.x.to_numpy()
    y = df.y.to_numpy()
    z = df.value.to_numpy()

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    ax.plot3D(x, y, z, 'o')
    ax.plot3D(0.680788, -0.732481, 0, 'o')

