print(__doc__)

from sklearn import metrics
from sklearn import preprocessing
import numpy as np
from string import ascii_letters
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
sns.set(style="white")

###############################################################################
# Leyendo data
n, dim = 28, 2

X = np.zeros((n,dim)) 
cont = 0
index = 0

X[0][0] = 1
X[0][1] = 1

X[1][0] = 2
X[1][1] = 1

X[2][0] = 3
X[2][1] = 1

X[3][0] = 1
X[3][1] = 2

X[4][0] = 2
X[4][1] = 2

X[5][0] = 3
X[5][1] = 2

X[6][0] = 1
X[6][1] = 3

X[7][0] = 2
X[7][1] = 3

X[8][0] = 3
X[8][1] = 3

X[9][0] = 6
X[9][1] = 5

X[10][0] = 7
X[10][1] = 5

X[11][0] = 8
X[11][1] = 5

X[12][0] = 6
X[12][1] = 6

X[13][0] = 7
X[13][1] = 6

X[14][0] = 8
X[14][1] = 6

X[15][0] = 6
X[15][1] = 7

X[16][0] = 7
X[16][1] = 7

X[17][0] = 8
X[17][1] = 7

X[18][0] = 12
X[18][1] = 9

X[19][0] = 13
X[19][1] = 9

X[20][0] = 14
X[20][1] = 9

X[21][0] = 12
X[21][1] = 10

X[22][0] = 13
X[22][1] = 10

X[23][0] = 14
X[23][1] = 10

X[24][0] = 12
X[24][1] = 11

X[25][0] = 13
X[25][1] = 11

X[26][0] = 14
X[26][1] = 11

X[27][0] = 15
X[27][1] = 11

    
print(cont)
labels_true = np.zeros(n) 
print(X)

#####################################################


#X=X.T

#min_max_scaler = preprocessing.MinMaxScaler()
#X_train_minmax = min_max_scaler.fit_transform(X)

## normalizar data ##

# Generate a large random dataset
rs = np.random.RandomState(33)
d = pd.DataFrame(data=rs.normal(size=(100, 2)),
                 columns=list([1,2]))

#d = pd.DataFrame(X)
print(d)

print(d[2][0])

X = np.zeros((100,2)) 
for i in range(0,99):
    d[1][i] = X[i][0]
    d[2][i] = X[i][1]
    print(X[i][0])
    print(X[i][1])

print(d)

# Compute the correlation matrix

corr = d.corr()
print(corr)
# Generate a mask for the upper triangle
mask = np.zeros_like(corr, dtype=np.bool)
mask[np.triu_indices_from(mask)] = True

# Set up the matplotlib figure
f, ax = plt.subplots(figsize=(110, 90))

# Generate a custom diverging colormap
cmap = sns.diverging_palette(240, 10, n=12)

# Draw the heatmap with the mask and correct aspect ratio
sns.heatmap(corr, mask=mask, cmap=cmap, vmin=0.,vmax=1.,
            square=True,cbar_kws={"shrink": .5})

#plt.show()
