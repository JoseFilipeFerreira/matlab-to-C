import sys

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from os import path

db = []
for line in open(sys.argv[1]):
    db.append([int(i) for i in line.split(',')])

db= np.array(db).T.tolist()

df = pd.DataFrame(db, columns=['Monte-Carlo','Monte-Carlo c/ Annealing','Greedy Search'])
boxplot = df.boxplot(
    patch_artist = True,
    vert=0,
    column=['Monte-Carlo','Monte-Carlo c/ Annealing','Greedy Search'])

plt.title("Comparação de métodos com maxi=300 Tmin=.0001 Tdecay=0.999")
plt.savefig("graph_"+sys.argv[1].replace(".output", ".png"), bbox_inches='tight', dpi=300)
