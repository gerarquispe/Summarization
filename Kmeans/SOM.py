from minisom import MiniSom
import numpy as np
################# Leyendo data #######################
# Leyendo data
file = open("numberTrajectory2.txt","r")
n = int(file.read())
file.close()
print(n)
X = np.zeros((n,12)) 
cont = 0
index = 0
file = open("coeficientes3.txt","r")
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
print(index)
labels_true = np.zeros(n) 

#############################################################
data = X
#data = [5,6,3,3,3,5,5,6]
m = 3 #tamano de neuronas
som = MiniSom(m, m, 12, sigma=.8, learning_rate=0.005)
print ("Training...")

som.train_random(data, 5000)

qnt = som.distance_map()
print (data)
print (qnt)

F = np.zeros((m,m)) 

for xx in data:
    w = som.winner(xx)
    F[w[0]][w[1]] = F[w[0]][w[1]] + 1
    #print(w)

print(F)
