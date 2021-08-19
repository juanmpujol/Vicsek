import glob
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib import rc



textFiles = glob.glob('*.txt')


for fileName in textFiles:
    dataFile = pd.read_csv(fileName, sep = '\s*, \s*', engine = 'python')
    cleanName = fileName.strip(".txt").split("_")
    densidad = fileName.strip(".txt").split("_")[1].split("=")[1][:5]
    fig = plt.figure()
    ax = fig
    plt.figure(figsize=(15,10))
    plt.ylabel(r'$\varphi$', fontsize = 30)
    plt.xlabel(r'$\eta$', fontsize = 30)
    plt.title(r'$\rho = $' + densidad , fontsize = 30)
    plt.xticks(fontsize = 18)
    plt.yticks(fontsize = 20)
    plt.scatter(dataFile.iloc[:, 0], dataFile.iloc[:, 2])
    plt.savefig("phi_densidad="+densidad+".png") 
    plt.close(fig)
    
    fig = plt.figure()
    ax = fig
    plt.figure(figsize=(15,10))
    plt.ylabel(r'$\chi$', fontsize = 30)
    plt.xlabel(r'$\eta$', fontsize = 30)
    plt.title(r'$\rho = $' + densidad, fontsize = 30)
    plt.xticks(fontsize = 20)
    plt.yticks(fontsize = 20)
    plt.plot(dataFile.iloc[:, 0], dataFile.iloc[:, 1], "-o")
    plt.savefig("susc_densidad="+densidad+".png") 
    plt.close(fig)

    plt.close('all')
