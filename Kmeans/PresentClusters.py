import numpy as np
import sys
import glob
import cv2
import os

################################################################################
def latex(file2):
    print(file2)
    name = file2.split('.')
    out     = name[0]+'.tex'
    script  = '\\documentclass[12pt,twoside]{report}\n\\usepackage{graphicx}\n\\usepackage{alphalph}\n\\usepackage{subcaption}\n\\usepackage[a5paper,margin=1cm]{geometry}\n\\renewcommand*{\\thesubfigure}{(\\arabic{subfigure})}\n\\begin{document}\n\\begin{figure}\n\\centering\n'
    sc_sub  = '\\begin{subfigure}[b]{0.20\\textwidth}\n\\centering\n\\includegraphics[width=\\textwidth]{'
    sc_sube = '\n\\end{subfigure}\n'
    sc_partir = '\\end{figure}\n\\begin{figure}\n'

    cont = 0
    #Leyendo file
    file = open(file2,"r")
    for line in file:

        print(line)
        
        line2 = line.split()
        ind = sc_sub +'../trajectories/'+line2[0]+'.png'+ '}\n\\caption{Id:'+line2[0]+'}' + sc_sube
        
        script += ind
        if(cont==31):
            script+= sc_partir
            cont=-1
        cont = cont + 1
    
    script += '\\end{figure}\n\\end{document}'
    print (script)

    fl = open(out,'w')
    fl.write(script)
    fl.close()

################################################################################
################################################################################
############################### MAIN ###########################################
if __name__ == '__main__':
    clustersFolder = 'clusters\\*.cluster'
    idcluster = 0
    for cfile in glob.glob(clustersFolder):
        id = cfile.index('\\')
        list1 = list(cfile)
        list1[id]='/'
        file2 = ''.join(list1)
        latex(file2)

# Escribe los clusters 
#    os.system("python generarPDF.py")
