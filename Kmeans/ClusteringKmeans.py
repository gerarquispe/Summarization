from sklearn.cluster import KMeans
import numpy as np



#######################Leyendo data############################
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
        print(index)
        print(X[index])
        index = index+1
        
    cont = cont+1
    
print(cont)
labels_true = np.zeros(n) 
#print(X)

#################### Procesando el Kmeans #####################
n_clusters_ = 12;
kmeans = KMeans(n_clusters=n_clusters_, random_state=0).fit(X)
labels = kmeans.labels_

file = open("ClustersOutPut.txt","w")
for i in range(0,len(labels)):
    file.write(str(labels[i])+" "+ str(i) +'\n')
file.close()

print( kmeans.cluster_centers_ )
