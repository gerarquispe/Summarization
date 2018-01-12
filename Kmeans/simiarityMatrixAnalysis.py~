print(__doc__)

from sklearn import metrics
from sklearn import preprocessing
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
sns.set(style="white")

###############################################################################
# Leyendo data
file = open("numberTrajectory.txt","r")
line = file.readlines()
n, dim = [int(val) for val in line[0].split()] # read first line

X = np.zeros((n,dim)) 
cont = 0
index = 0
file = open("coeficientesClusters.txt","r")
x=[]
y=[]
isXcomponet = 0
for line in file:
    line2 = line.split(",")
    if(isXcomponet==0):
        #componentes en x vs t
        X[index][0]=line2[0]
        X[index][1]=line2[1]
        X[index][2]=line2[2]
        X[index][3]=line2[3]
        X[index][4]=line2[4]
        X[index][5]=line2[5]
        isXcomponet=1
    else:
        #componentes en y vs t
        X[index][6]=line2[0] 
        X[index][7]=line2[1]
        X[index][8]=line2[2]
        X[index][9]=line2[3]
        X[index][10]=line2[4]
        X[index][11]=line2[5]
        isXcomponet=0
        
    if(cont%2==1):
        #print(index)
        #print(X[index])
        index = index+1
        
    cont = cont+1
    
print(cont)
labels_true = np.zeros(n) 
#print(X)

#####################################################

#subX1,subX2,subX3,subX4,subX5,subX6,subX7,subX8,subX9,subX10 = np.split(X,10)
#subX1 = subX5.T
X = X.T

min_max_scaler = preprocessing.MinMaxScaler()
X_train_minmax = min_max_scaler.fit_transform(X)

## normalizar data ##

d = pd.DataFrame(X_train_minmax)
print(d)

# Compute the correlation matrix
corr = d.corr()

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

plt.show()
