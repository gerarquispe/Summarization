import pandas as pd # Es utilizado para leer archivos csv
import numpy as np
from mpl_toolkits.mplot3d import Axes3D # Grafico 3D plot
from sklearn.preprocessing import LabelEncoder
from sklearn.preprocessing import StandardScaler
from sklearn.cross_validation import train_test_split

# visualize the important characteristics of the dataset
import matplotlib.pyplot as plt

########### Read Data #######################################
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

"""
###########################################################
# step 3: get features (x) and scale the features
# get x and convert it to numpy array
x = dataframe_all.ix[:,:-1].values
"""
standard_scaler = StandardScaler()
x_std = standard_scaler.fit_transform(X)

# step 4: get class labels y and then encode it into number 
# get class label data
#y = dataframe_all.ix[:,-1].values
# encode the class label
#class_labels = np.unique(y)
#label_encoder = LabelEncoder()
#y = label_encoder.fit_transform(y)

# step 5: split the data into training set and test set
#test_percentage = 0.1
#x_train, x_test, y_train, y_test = train_test_split(x_std, y, test_size = test_percentage, random_state = 0)

# t-distributed Stochastic Neighbor Embedding (t-SNE) visualization
from sklearn.manifold import TSNE
tsne = TSNE(n_components=3, random_state=0)

#x_test_2d = tsne.fit_transform(x_test)
x_test_2d = tsne.fit_transform(x_std)

# Verificar x_test_2d si es lo que precisa.


# scatter plot the sample points among 5 classes
markers=('s', 'd', 'o', '^', 'v')
color_map = {0:'red', 1:'blue', 2:'lightgreen', 3:'purple', 4:'cyan'}

print(x_test_2d)

fig = plt.figure()
ax = fig.add_subplot(111,projection='3d')

for idx, cl in enumerate(x_test_2d):
    ax.scatter(x_test_2d[idx][0],x_test_2d[idx][1],x_test_2d[idx][2])
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

plt.show()

""" 
plt.figure()
print(x_test_2d)
for idx, cl in enumerate(x_test_2d):
    plt.scatter(x=x_test_2d[idx][0], y=x_test_2d[idx][1])
plt.xlabel('X in t-SNE')
plt.ylabel('Y in t-SNE')
plt.legend(loc='upper left')
plt.title('t-SNE visualization of test data')
plt.show()
"""
