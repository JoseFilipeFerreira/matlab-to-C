import sys

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from os import path

db = []
for line in open(sys.argv[1]):
    db.append([int(i) for i in line.split(',')])

db= np.array(db).T.tolist()

df = pd.DataFrame(db, columns=['Monte Carlo', 'Annealing'])
boxplot = df.boxplot(
    patch_artist = True,
    vert=0,
    column=['Monte Carlo', 'Annealing'])

props = sys.argv[1].replace(".output", "").split("_")
plt.title(f"Simulation with maxi={props[0]}00, Tmin={props[1]}, Tdecay={props[2]}")
plt.savefig("graph_"+sys.argv[1].replace(".output", ".png"),transparent=True, bbox_inches='tight', dpi=300)
