#-*- coding: utf-8 -*-
import time
import numpy as np
import scipy.linalg
import matplotlib.pyplot as pyplot
from scipy.sparse import dia_matrix
from matplotlib.animation import FuncAnimation

#ヤコビ法によってN元連立1次方程式を解く関数
def Jacobi(A,b,error,maxiter):
    N = len(A)
    D = np.diag(A)
    G = A - np.diag(D)
    x = np.array([0.0]*N)
    n = 0
    while n < maxiter:
        n += 1
        x_n1 = (b - np.ravel(np.dot(G,x))) / D
        if ( np.linalg.norm(x_n1-x)/np.linalg.norm(x_n1) ) < error:
            break
        x = x_n1
    return x_n1,n

#ガウス・ザイデル法によってN元連立1次方程式を解く関数
def Gauss_Seidel(A,b,error,maxiter):
    N = len(A)
    D = np.diag(A)
    G = A - np.diag(D)
    x = np.array([0.0]*N)
    n = 0
    while n < maxiter:
        n += 1
        err = 0.0
        for i in range(N):
            x_cal = x[i]
            x[i] = ( b[i] - np.dot(G[i],x) ) / D[i]
            if x[i] != 0.0:
                err += abs( (x[i] - x_cal) / x[i] )
        if err < error:
            break
    return x,n

#LU法によってN元連立1次方程式を解く関数
def LU(A,b):
    N = len(A)
    x = np.zeros(N)
    y = np.zeros(N)
    P, L, U = scipy.linalg.lu(A)
    for i in range(N):
        temp = 0.0
        for j in range(0,i):
            temp += y[j] * L[i][j]
        y[i] = b[i] - temp
    for i in reversed(range(N)):
        temp = 0.0
        for j in range(i+1,N):
            temp += x[j] * U[i][j]
        x[i] = (y[i]-temp) / U[i][i]
    return np.dot(P,x)

#SOR法によってN元連立1次方程式を解く関数
def SOR(A,b,error,maxiter):
    N = len(A)
    D = np.diag(np.diag(A))
    L = np.tril(A, -1)
    U = np.triu(A, 1)
    G = A - D
    r_Mj = max(abs(np.linalg.eigvals(np.dot(np.linalg.inv(D), -(L+U)))))
    w = 2/(1+np.sqrt(1-r_Mj**2))
    x = np.array([0.0]*N)
    n = 0
    while n < maxiter:
        n += 1
        err = 0.0
        for i in range(N):
            x_cal = x[i]
            x[i] = (1-w)*x_cal + (w*(b[i]-np.dot(G[i],x)))/D[i][i]
            if x[i] != 0.0:
                err += np.abs( (x[i] - x_cal) / x[i] )
        if err < error:
            break
    return x,n

#前進差分(陽解法)で1次元熱伝導方程式を解く関数
#差分近似と、2・4次のルンゲクッタ法で計算を行う
def forward_diff(u_answer, c,u_init, x_n, t_n, cal_func):
    u = u_init.copy()
    u_ans = u_answer.copy()
    u_err = np.zeros(t_n)

    k1 = np.zeros(x_n)
    k2 = np.zeros(x_n)
    k3 = np.zeros(x_n)
    k4 = np.zeros(x_n)

    def func_runge(x_bef,x,x_aft):
        return x_bef - 2.0*x + x_aft

    for t in range(t_n-1):        
        if cal_func == "DA":
            for x in range(1, x_n-1):
                u[x, t+1] = u[x, t] + c*(u[x+1, t] - 2*u[x, t] + u[x-1, t])
        elif cal_func == "RK2":
            for x in range(1, x_n-1):
                k1[x] = c*func_runge(u[x+1, t], u[x, t], u[x-1, t])
            for x in range(1, x_n-1):
                k2[x] = c*func_runge(u[x+1, t]+k1[x+1]/2.0, u[x, t]+k1[x]/2.0, u[x-1, t]+k1[x-1]/2.0)
            for x in range(1, x_n-1):
                u[x, t+1] = u[x, t] + (k1[x] + k2[x])/2.0
        elif cal_func == "RK4":
            for x in range(1, x_n-1):
                k1[x] = c*func_runge(u[x+1, t], u[x, t], u[x-1, t])
            for x in range(1, x_n-1):
                k2[x] = c*func_runge(u[x+1, t]+k1[x+1]/2.0, u[x, t]+k1[x]/2.0, u[x-1, t]+k1[x-1]/2.0)
            for x in range(1, x_n-1):
                k3[x] = c*func_runge(u[x+1, t]+k2[x+1]/2.0, u[x, t]+k2[x]/2.0, u[x-1, t]+k2[x-1]/2.0)
            for x in range(1, x_n-1):
                k4[x] = c*func_runge(u[x+1, t]+k3[x+1], u[x, t]+k3[x], u[x-1, t]+k3[x-1])
            for x in range(1, x_n-1):
                u[x, t+1] = u[x, t] + (k1[x] + 2.0*k2[x] + 2.0*k3[x] + k4[x])/6.0

    #解との誤差を計算
    for t in range(t_n): 
        u_err[t] = np.sum(np.abs(u_ans[:,t] - u[:,t]))

    return u, u_err

#後退差分(陰解法)で1次元熱伝導方程式を解く関数
#得られた連立方程式をヤコビ法・ガウスザイデル法・LU分解法・SOR法によって解く
def backward_diff(u_answer, c, u_init, x_n, t_n, cal_func, error, max_iter):
    u = u_init.copy()
    u_ans = u_answer.copy()
    u_err = np.zeros(len(u[0]))

    #係数行列は対角線において対象であるのでdia_matrixによって初期設定
    A_val = np.array([np.full(x_n, -c), np.full(x_n, 1+2*c) ,np.full(x_n, -c)], dtype=float)
    A = dia_matrix((A_val, np.array([-1, 0, 1])), shape=(x_n, x_n))
    #2次元配列(行列)に変換
    A = A.todense()
    #境界条件
    A[0,1] = 0.0
    A[-1,-2] = 0.0
    
    #各計算方法で連立方程式を解き、u[x,t+1]を求める
    for i in range(t_n - 1):
        if cal_func == "Jacobi":
            u[:,i+1], n = Jacobi(A, u[:,i], error, max_iter)
        elif cal_func == "GS":
            u[:,i+1], n = Gauss_Seidel(A, u[:,i], error, max_iter)
        elif cal_func == "LU":
            u[:,i+1] = LU(A, u[:,i])
            n = 0
        elif cal_func == "SOR":
            u[:,i+1], n = SOR(A, u[:,i], error, max_iter)
    
    #解との誤差を計算
    for t in range(t_n): 
        u_err[t] = np.sum(np.abs(u_ans[:,t] - u[:,t]))

    return u, u_err, n

#Crank-Nicolson法で1次元熱伝導方程式を解く関数
#得られた連立方程式をヤコビ法・ガウスザイデル法・LU分解法・SOR法によって解く
def Crank_Nicolson(u_answer, c, u_init, x_n, t_n, cal_func, error, max_iter):
    u = u_init.copy()
    u_ans = u_answer.copy()
    u_err = np.zeros(len(u[0]))

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

    #各計算方法で連立方程式を解き、u[x,t+1]を求める
    for i in range(t_n - 1):
        if cal_func == "Jacobi":
            u[:,i+1], n = Jacobi(A, C*u[:,i], error, max_iter)
        elif cal_func == "GS":
            u[:,i+1], n = Gauss_Seidel(A, C*u[:,i], error, max_iter)
        elif cal_func == "LU":
            u[:,i+1] = LU(A, C*u[:,i])
            n = 0
        elif cal_func == "SOR":
            u[:,i+1], n = SOR(A, C*u[:,i], error, max_iter)

    #解との誤差を計算
    for t in range(t_n): 
        u_err[t] = np.sum(np.abs(u_ans[:,t] - u[:,t]))

    return u, u_err, n

#---------------------------------パラメータ設定---------------------------------#
t_a = 0.0   #開始時刻
t_b = 5.0   #終了時刻
t_H = 0.01 #時刻の刻み幅

x_a = 0.0   #始端
x_b = 1.0   #終端
x_H = 0.025  #xの刻み幅

alpha = 0.01 #係数 (熱拡散率)

error = 1e-10       #連立方程式を解く際の許容誤差
max_iter = 10000    #連立方程式を解く際の最大反復回数
#------------------------------------------------------------------------------#

t_n = int((t_b-t_a)/t_H)
x_n = int((x_b-x_a)/x_H) + 1 #u(2,t)=0を使用するため1つ多くする

c = alpha * ((t_H)/(x_H**2))

x_i = np.arange(x_a, x_b+x_H, x_H)
t_i = np.arange(t_a, t_b, t_H)
u = np.zeros((x_n, t_n))

#--------------------------------初期条件・境界条件--------------------------------#
#初期条件 sin(3πx)
u[:, 0] = np.sin(3*np.pi*x_i)
#境界条件 (Dirichlet条件)
u[0, :] = 0.0
u[x_n-1, :] = 0.0
#-------------------------------------------------------------------------------#

#偏微分方程式の解を計算
u_ans = np.zeros((x_n, t_n))
for i, t in enumerate(t_i):
    u_ans[:,i] = np.exp(-alpha*(9*np.pi**2)*t)*(np.sin(3*np.pi*x_i))

#各計算で用いるメソッド
FD_func = ["DA","RK2","RK4"]
BD_func = ["Jacobi","GS","LU","SOR"]
CN_func = ["Jacobi","GS","LU","SOR"]

line_style = ["-","--","-.",":"]

#陽解法
#差分近似・2次ルンゲクッタ・4次ルンゲクッタの3つの方法で解を求める
print("\n--Forward diff--")
u_fd_ans = []
u_fd_error = []
u_fd_time = []
for i in range(len(FD_func)):
    start_time = time.time()
    u_fd, u_fd_err = forward_diff(u_ans, c, u, x_n, t_n, FD_func[i])
    end_time = time.time()
    u_fd_ans.append(u_fd)
    u_fd_error.append(u_fd_err)
    u_fd_time.append(end_time-start_time)
    print("・method = "+FD_func[i])
    print("  time = "+str(u_fd_time[i]))

#陰解法
#ヤコビ法・ガウスザイデル法・LU分解法・SOR法の4つの方法で解を求める
print("\n--Backward diff--")
u_bd_ans = []
u_bd_error = []
u_bd_iter = []
u_bd_time = []
for i in range(len(BD_func)):
    start_time = time.time()
    u_bd, u_bd_err, n = backward_diff(u_ans, c, u, x_n, t_n, BD_func[i], error, max_iter)
    end_time = time.time()
    u_bd_ans.append(u_bd)
    u_bd_error.append(u_bd_err)
    u_bd_iter.append(n)
    u_bd_time.append(end_time-start_time)
    print("・method = "+BD_func[i])
    print("  time = "+str(u_bd_time[i]))
    if BD_func[i] != "LU":
        print("  iter = "+str(u_bd_iter[i]))

#クランク・ニコルソン法
#ヤコビ法・ガウスザイデル法・LU分解法・SOR法の4つの方法で解を求める
print("\n--Crank-Nicolson--")
u_CN_ans = []
u_CN_error = []
u_CN_iter = []
u_CN_time = []
for i in range(len(CN_func)):
    start_time = time.time()
    u_CN, u_CN_err, n = Crank_Nicolson(u_ans, c, u, x_n, t_n, CN_func[i], error, max_iter)
    end_time = time.time()
    u_CN_ans.append(u_CN)
    u_CN_error.append(u_CN_err)
    u_CN_iter.append(n)
    u_CN_time.append(end_time-start_time)
    print("・method = "+CN_func[i])
    print("  time = "+str(u_CN_time[i]))
    if CN_func[i] != "LU":
        print("  iter = "+str(u_CN_iter[i]))

#アニメーションを生成するための関数
def plot_ans(i):
    pyplot.cla()
    pyplot.ylim([-1.2, 1.2])
    pyplot.title('t=%f[s]' % (5*i/(t_n-1)*t_b))
    pyplot.xlabel('x')
    pyplot.ylabel('u')
    pyplot.grid()
    for j in range(len(FD_func)):
        pyplot.plot(x_i, u_fd_ans[j][:,5*i],line_style[j],label="forward_diff : "+FD_func[j],color='#0000aa',linewidth = 1.5)
    for j in range(len(BD_func)):
        pyplot.plot(x_i, u_bd_ans[j][:,5*i],line_style[j],label="backward_diff : "+BD_func[j],color='#00aa00',linewidth = 1.5)
    for j in range(len(CN_func)):
        pyplot.plot(x_i, u_CN_ans[j][:,5*i],line_style[j],label="Crank-Nicolson : "+CN_func[j],color='#aa0000',linewidth = 1.5)
    pyplot.plot(x_i, u_ans[:,5*i],label="answer",color='#000000',linewidth = 1.5)
    pyplot.legend(loc='lower right')

#解のアニメーションを生成
fig1 = pyplot.figure(figsize=(12,8))
animation = FuncAnimation(fig1, plot_ans, frames=int(t_n/5.0)-1, interval=1)
animation.save("PDE.gif", writer="pillow")
pyplot.show()

#(時刻)-(誤差)のグラフを作成
fig2 = pyplot.figure(figsize=(12,8))
pyplot.xlabel('t[s]')
pyplot.ylabel('error')
pyplot.grid()
for j in range(len(FD_func)):
    pyplot.plot(t_i, u_fd_error[j], line_style[j], label="forward_diff : "+FD_func[j],color='#0000aa',linewidth = 1.5)
for j in range(len(BD_func)):
    pyplot.plot(t_i, u_bd_error[j], line_style[j], label="backward_diff : "+BD_func[j],color='#00aa00',linewidth = 1.5)
for j in range(len(CN_func)):
    pyplot.plot(t_i, u_CN_error[j], line_style[j], label="Crank-Nicolson : "+CN_func[j],color='#aa0000',linewidth = 1.5)
pyplot.legend()

#(時刻)-(誤差*計算時間)のグラフを作成
fig3 = pyplot.figure(figsize=(12,8))
pyplot.xlabel('t[s]')
pyplot.ylabel('error*cal_time')
pyplot.grid()
for j in range(len(FD_func)):
    pyplot.plot(t_i, u_fd_time[j]*u_fd_error[j], line_style[j], label="forward_diff : "+FD_func[j],color='#0000aa',linewidth = 1.5)
for j in range(len(BD_func)):
    pyplot.plot(t_i, u_bd_time[j]*u_bd_error[j], line_style[j], label="backward_diff : "+BD_func[j],color='#00aa00',linewidth = 1.5)
for j in range(len(CN_func)):
    pyplot.plot(t_i, u_CN_time[j]*u_CN_error[j], line_style[j], label="Crank-Nicolson : "+CN_func[j],color='#aa0000',linewidth = 1.5)
pyplot.legend()

pyplot.show()
