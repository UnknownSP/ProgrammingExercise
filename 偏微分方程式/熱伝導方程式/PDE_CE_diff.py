#-*- coding: utf-8 -*-
import time
import numpy as np
import matplotlib.pyplot as pyplot
from scipy.sparse import dia_matrix
from matplotlib.animation import FuncAnimation

#ガウス・ザイデル法によってN元連立1次方程式を解く関数
def Gauss_Seidel(A, b, error, maxiter):
    D = np.diag(A)
    G = A - np.diag(D)
    x = np.zeros_like(b)
    n = 0
    while n < maxiter:
        n += 1
        err = 0.0
        for i in range(len(x)):
            x_cal = x[i]
            x[i] = (b[i] - np.dot(G[i],x)) / D[i]
            if x[i] != 0:
                err += abs( (x[i] - x_cal) / x[i] )
        if err < error:
            return x

#前進差分(陽解法)で1次元熱伝導方程式を解く関数
def forward_diff(c,u_init, x_n, t_n):
    u = u_init.copy()

    for t in range(t_n-1):
        for x in range(1, x_n-1):
            u[x, t+1] = u[x, t] + c*(u[x+1, t] - 2*u[x, t] + u[x-1, t])

    return u

#後退差分(陰解法)で1次元熱伝導方程式を解く関数
def backward_diff(c, u_init, x_n, t_n):
    u = u_init.copy()

    #係数行列は対角線において対象であるのでdia_matrixによって初期設定
    A_val = np.array([np.full(x_n, -c), np.full(x_n, 1+2*c) ,np.full(x_n, -c)], dtype=float)
    A = dia_matrix((A_val, np.array([-1, 0, 1])), shape=(x_n, x_n))
    #2次元配列(行列)に変換
    A = A.todense()
    #境界条件
    A[0,1] = 0.0
    A[-1,-2] = 0.0
    
    #ガウス・ザイデル法によって連立方程式を解き、u[x,t+1]を求める
    for i in range(t_n - 1):
        u[:,i+1] = Gauss_Seidel(A, u[:,i], 1e-15, 1000)

    return u

#Crank-Nicolson法で1次元熱伝導方程式を解く関数
def Crank_Nicolson(c, u_init, x_n, t_n):
    u = u_init.copy()

    A_val = np.array([np.full(x_n, -c), np.full(x_n, 2+2*c) ,np.full(x_n, -c)], dtype=float)
    A = dia_matrix((A_val, np.array([-1, 0, 1])), shape=(x_n, x_n))
    A = A.todense()
    #境界条件
    A[0,1] = 0.0
    A[-1,-2] = 0.0

    #係数
    C_val = np.array([np.full(x_n, c), np.full(x_n, 2-2*c), np.full(x_n, c)], dtype=float)
    C = dia_matrix((C_val, np.array([-1, 0, 1])), shape=(x_n, x_n))
    #2次元配列に戻さず1次元の状態で計算する
    C = C.tolil()
    #境界条件
    C[0] = 0.0
    C[-1] = 0.0

    #ガウス・ザイデル法によって連立方程式を解き、u[x,t+1]を求める
    for i in range(t_n - 1):
        u[:,i+1] = Gauss_Seidel(A, C*u[:,i], 1e-15, 1000)

    return u

t_a = 0.0   #開始時刻
t_b = 4.0   #終了時刻
t_H = 0.0004 #時刻の刻み幅
t_n = int((t_b-t_a)/t_H)

x_a = 0.0   #始端
x_b = 4.0   #終端
x_H = 0.04  #xの刻み幅
x_n = int((x_b-x_a)/x_H) + 1 #u(2,t)=0を使用するため1つ多くする

alpha = 2.0 #係数
c = alpha * ((t_H)/(x_H**2))

x_i = np.arange(x_a, x_b+x_H, x_H)
t_i = np.arange(t_a, t_b, t_H)
u = np.zeros((x_n, t_n))

#初期条件
u[:, 0] = 1.0
#境界条件 (Dirichlet条件)
u[0, :] = 0.0
u[x_n-1, :] = 0.0

print("\n--Forward diff--")
start_time = time.time()
u_fd = forward_diff(c, u, x_n, t_n)
end_time = time.time()
print("time = "+str(end_time-start_time))

print("\n--Backward diff--")
start_time = time.time()
u_bd = backward_diff(c, u, x_n, t_n)
end_time = time.time()
print("time = "+str(end_time-start_time))

print("\n--Crank-Nicolson--")
start_time = time.time()
u_CN = Crank_Nicolson(c, u, x_n, t_n)
end_time = time.time()
print("time = "+str(end_time-start_time))

def plot(i):
    pyplot.cla()
    pyplot.ylim([-0.4, 1.2])
    pyplot.title('t=%f[s]' % (5*i/(t_n-1)*t_b))
    pyplot.xlabel('x')
    pyplot.ylabel('u')
    pyplot.grid()
    pyplot.plot(x_i, u_fd[:,5*i],label='forward_diff',color='#0000aa')
    pyplot.plot(x_i, u_bd[:,5*i],label='backward_diff',color='#00aa00')
    pyplot.plot(x_i, u_CN[:,5*i],label='Crank-Nicolson',color='#aa0000')
    pyplot.legend(loc='lower right')

fig1 = pyplot.figure()
animation = FuncAnimation(fig1, plot, frames=int(t_n/5.0)-1, interval=1)
animation.save("PDE_CE_diff.gif", writer="pillow")
pyplot.show()

fig2 = pyplot.figure()
#Crank-Nicolson法と前進差分の誤差
err_fd = np.sum(u_CN-u_fd, axis=0)
#Crank-Nicolson法と後退差分の誤差
err_bd = np.sum(u_CN-u_bd, axis=0)

pyplot.xlabel('t[s]')
pyplot.ylabel('error')
pyplot.grid()
pyplot.plot(t_i, err_fd, label='CrankNicolson - ForwardDiff',color='#aa0000')
pyplot.plot(t_i, err_bd, label='CrankNicolson - BackwardDiff',color='#0000aa')
pyplot.legend()

pyplot.show()
