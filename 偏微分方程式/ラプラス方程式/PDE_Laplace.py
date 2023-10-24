#-*- coding: utf-8 -*-
import time
import numpy as np
import matplotlib.pyplot as pyplot
from mpl_toolkits.mplot3d import Axes3D

#ヤコビ法によってラプラス方程式を解く関数
def Jacobi(phi_init, x_H, y_H, error, maxiter):
    phi = phi_init.copy()
    err = []
    n = 0
    while n < maxiter:
        n += 1
        phi_recent = phi.copy()
        for x in range(1,len(phi)-1):
            for y in range(1,len(phi[0])-1):
                phi[x,y] = (y_H**2 * (phi_recent[x+1, y] + phi_recent[x-1, y])
                          + x_H**2 * (phi_recent[x, y+1] + phi_recent[x, y-1])) / (2*(x_H**2+y_H**2))
        
        err.append(np.sum(np.abs(phi)-np.abs(phi_recent)) / np.sum(np.abs(phi_recent)))
        if err[n-1] < error:
            return phi, n, err

#ガウス・ザイデル法によってラプラス方程式を解く関数
def Gauss_Seidel(phi_init, x_H, y_H, error, maxiter):
    phi = phi_init.copy()
    err = []
    n = 0
    while n < maxiter:
        n += 1
        phi_recent = phi.copy()
        for x in range(1,len(phi)-1):
            for y in range(1,len(phi[0])-1):
                phi[x,y] = (y_H**2 * (phi[x+1, y] + phi[x-1, y])
                          + x_H**2 * (phi[x, y+1] + phi[x, y-1])) / (2*(x_H**2+y_H**2))
        
        err.append(np.sum(np.abs(phi)-np.abs(phi_recent)) / np.sum(np.abs(phi_recent)))
        if err[n-1] < error:
            return phi, n, err

#SOR法によってラプラス方程式を解く関数
def SOR(phi_init, x_H, y_H, error, maxiter):
    rho = 1.0/2.0 * ( np.cos(np.pi*x_H) + np.cos(np.pi*y_H) )
    w = 2/(1+np.sqrt(1-rho**2)) #加速パラメータ
    phi = phi_init.copy()
    err = []
    n = 0
    while n < maxiter:
        n += 1
        phi_recent = phi.copy()
        for x in range(1,len(phi)-1):
            for y in range(1,len(phi[0])-1):
                phi_cal = (y_H**2 * (phi[x+1, y] + phi[x-1, y])
                          + x_H**2 * (phi[x, y+1] + phi[x, y-1])) / (2*(x_H**2+y_H**2))
                phi[x,y] += w * (phi_cal - phi[x, y])
        err.append(np.sum(np.abs(phi)-np.abs(phi_recent)) / np.sum(np.abs(phi_recent)))
        if err[n-1] < error:
            return phi, n, err

x_a = 0.0   #xの範囲
x_b = 1.0   
x_H = 0.02  #xの刻み幅
x_n = int((x_b-x_a)/x_H) + 1

y_a = 0.0   #yの範囲
y_b = 1.0
y_H = 0.02  #yの刻み幅
y_n = int((y_b-y_a)/y_H) + 1

x_i = np.arange(x_a, x_b+x_H, x_H)
y_i = np.arange(y_a, y_b+y_H, y_H)

#3次元グラフを描画する関数
def plot(phi,title):
    X, Y = np.meshgrid(x_i, y_i)
    fig = pyplot.figure()
    ax = fig.add_subplot(111, projection="3d")
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('φ(x,y)')
    ax.set_xlim(x_a, x_b)
    ax.set_ylim(y_a, y_b)
    ax.set_title(title)
    ax.plot_surface(X, Y, phi[:], cmap = "winter")

phi = np.zeros((x_n, y_n))
#境界条件 (Dirichlet条件)
phi[:,0] = 0.0
phi[:,x_n-1] = x_i
phi[0] = 0.0
phi[y_n-1] = y_i
error = 1e-10       #許容誤差
max_iter = 10000    #最大反復回数

print("\n--Jacobi--")
start_time = time.time()
phi_ans, n_Jacobi, err_Jacobi = Jacobi(phi,x_H,y_H,error,max_iter)
end_time = time.time()
plot(phi_ans,"Jacobi")
print("time = "+str(end_time-start_time))
print("n = "+str(n_Jacobi))

print("\n--Gauss Siedel--")
start_time = time.time()
phi_ans, n_GS, err_GS = Gauss_Seidel(phi,x_H,y_H,error,max_iter)
end_time = time.time()
plot(phi_ans,"Gauss Siedel")
print("time = "+str(end_time-start_time))
print("n = "+str(n_GS))

print("\n--SOR--")
start_time = time.time()
phi_ans, n_SOR, err_SOR = SOR(phi,x_H,y_H,error,max_iter)
end_time = time.time()
plot(phi_ans,"SOR")
print("time = "+str(end_time-start_time))
print("n = "+str(n_SOR))

#誤差と反復回数の関係グラフを表示(x軸は対数軸)
fig_err = pyplot.figure()
pyplot.xlabel('number of iterations')
pyplot.ylabel('error')
pyplot.xscale("log")
pyplot.grid()
err_x = np.arange(1,n_Jacobi+1)
pyplot.plot(err_x, err_Jacobi, label='Jacobi',color='#aa0000')
err_x = np.arange(1,n_GS+1)
pyplot.plot(err_x, err_GS, label='Gauss Siedel',color='#00aa00')
err_x = np.arange(1,n_SOR+1)
pyplot.plot(err_x, err_SOR, label='SOR',color='#0000aa')
pyplot.legend()

pyplot.show()