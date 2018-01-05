import numpy as np
import sys
import os
import glob
import cv2
import os

if __name__ == '__main__':
    
    clustersFolder = '*.tex'
    for cfile in glob.glob(clustersFolder):
        token = cfile.split('.')
        cadena = "pdflatex " + token[0] + ".tex"
        print(cadena)
        os.system(cadena)
