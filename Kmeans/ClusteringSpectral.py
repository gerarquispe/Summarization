print(__doc__)

import numpy as np
import matplotlib.pyplot as plt

from sklearn.cluster import spectral_clustering

################# Read Data ################
file = open("numberTrajectory.txt","r")
n = int(file.read())
file.close()

X = np.zeros((n,2)) 
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
        isXcomponet=1
    else:
        #componentes en y vs t
        X[index][1]=line2[0]
        isXcomponet=0
        
    if(cont%2==1):
        index = index+1
        
    cont = cont+1
    
print(cont)
print(x)
labels_true = np.zeros(n) 
########## Llamar al clustering ############

labels = spectral_clustering(X, n_clusters=4, eigen_solver='arpack')


########## Mostrar resultados  #############
print(labels)
