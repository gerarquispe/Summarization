import matplotlib.pyplot as plt 
import numpy as np
from sklearn.decomposition import RandomizedPCA
from sklearn.decomposition import PCA
import numpy as np
from mpl_toolkits.mplot3d import Axes3D # Grafico 3D plot
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
        #print(index)
        #print(X[index])
        index = index+1
        
    cont = cont+1
    
print(cont)
labels_true = np.zeros(n) 
#print(X)

######################################################################
# Create a Randomized PCA model that takes two components
randomized_pca = RandomizedPCA(n_components=3)

# Fit and transform the data to the model
reduced_data_rpca = randomized_pca.fit_transform(X)

# Create a regular PCA model 
pca = PCA(n_components=3)

# Fit and transform the data to the model
reduced_data_pca = pca.fit_transform(X)

# Inspect the shape
reduced_data_pca.shape

# Print out the data
print(reduced_data_rpca)
print(reduced_data_pca)


colors = ['black', 'blue', 'purple', 'yellow', 'white', 'red', 'lime', 'cyan', 'orange', 'gray']

# 3D
fig = plt.figure()
ax = fig.add_subplot(111,projection='3d')
for idx, cl in enumerate(reduced_data_rpca):
    ax.scatter(reduced_data_rpca[idx][0],reduced_data_rpca[idx][1],reduced_data_rpca[idx][2])
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

plt.show()


"""  2D
for i in range(len(colors)):
    x = reduced_data_rpca[:, 0]
    y = reduced_data_rpca[:, 1]
    plt.scatter(x, y)
#plt.legend(, bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)
plt.xlabel('First Principal Component')
plt.ylabel('Second Principal Component')
plt.title("PCA Scatter Plot")
plt.show()
"""
