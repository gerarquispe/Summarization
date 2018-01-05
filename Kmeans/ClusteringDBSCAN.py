print(__doc__)

from sklearn.cluster import AffinityPropagation
from sklearn.cluster import DBSCAN
from sklearn.cluster import MeanShift, estimate_bandwidth
from sklearn import metrics
from sklearn.datasets.samples_generator import make_blobs
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler

###############################################################################
# Leyendo data
file = open("numberTrajectory.txt","r")
line = file.readlines()
n, dim = [int(val) for val in line[0].split()] # read first line

X = np.zeros((n,dim))
cont = 0
index = 0
file = open("coeficientes.txt","r")
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
        index = index+1
        
    cont = cont+1
    
print(cont)
labels_true = np.zeros(n) 
#print(X)


####################################################
db = DBSCAN(eps=181, min_samples=6, metric='euclidean', algorithm='auto', leaf_size=30, p=None, n_jobs=1).fit(X)
#db = DBSCAN(eps=100, min_samples=6, metric='euclidean', algorithm='auto', leaf_size=30, p=None, n_jobs=1).fit(X)

core_samples_mask = np.zeros_like(db.labels_, dtype=bool)
core_samples_mask[db.core_sample_indices_] = True
labels = db.labels_

# Number of clusters in labels, ignoring noise if present.
n_clusters_ = len(set(labels)) - (1 if -1 in labels else 0)

#####################################################
print(n_clusters_)
file = open("ClustersOutPut.txt","w")
for i in range(0,len(labels)):
    file.write(str(labels[i])+" "+ str(i) +'\n')
file.close()
