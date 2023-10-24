#-*- coding: utf-8 -*-
from scipy import integrate
import numpy as np
import math
import time
from matplotlib import pyplot

#計算対象の微分方程式
def f_prime(x,y):
    return math.exp(-math.sin(x)) - y * math.cos(x)

#計算対象の微分方程式の解(誤差計算のため)
def f(x):
    return (x+1)*math.exp(-math.sin(x))

#オイラー法で微分方程式を解く関数
def Euler(x,H,y_0):
    y = np.empty_like(x)
    err_ave = 0.0
    y[0] = y_0
    for i in range(len(x)-1):
        y[i+1] = y[i] + H * f_prime(x[i],y[i])
        err_ave += (f(x[i+1]) - y[i+1])**2
    err_ave = math.sqrt(err_ave/(len(x)-1))
    return y, err_ave

#2次のルンゲ=クッタ法(Heun法)で微分方程式を解く関数
def Runge_Kutta_2(x,H,y_0):
    y = np.empty_like(x)
    err_ave = 0.0
    y[0] = y_0
    for i in range(len(x)-1):
        k1 = H * f_prime(x[i],y[i])
        k2 = H * f_prime(x[i]+H, y[i]+k1)
        y[i+1] = y[i] + (k1+k2)/2.0
        err_ave += (f(x[i+1]) - y[i+1])**2
    err_ave = math.sqrt(err_ave/(len(x)-1))
    return y, err_ave

#4次のルンゲ=クッタ法で微分方程式を解く関数
def Runge_Kutta_4(x,H,y_0):
    y = np.empty_like(x)
    err_ave = 0.0
    y[0] = y_0
    for i in range(len(x)-1):
        k1 = H * f_prime(x[i],y[i])
        k2 = H * f_prime(x[i]+H/2.0, y[i]+k1/2.0)
        k3 = H * f_prime(x[i]+H/2.0, y[i]+k2/2.0)
        k4 = H * f_prime(x[i]+H, y[i]+k3)
        y[i+1] = y[i] + (k1+2*k2+2*k3+k4)/6.0
        err_ave += (f(x[i+1]) - y[i+1])**2
    err_ave = math.sqrt(err_ave/(len(x)-1))
    return y, err_ave

a = 0.0 #範囲(下限)
b = 20.0 #範囲(上限)
H = 0.001 #刻み幅
y_0 = 1 #初期値

x = np.arange(a,b,H)
y = np.empty_like(x)

print("\n--Euler--")
start_time = time.time()
y, err_ave = Euler(x,H,y_0)
end_time = time.time()
print('RMSE = {:.5e}'.format(err_ave)  + ', time = {:.5e}'.format(end_time-start_time))
pyplot.plot(x, y, label="Euler" ,marker='o', markersize=0, linewidth=0.8, color='#cc0000',linestyle='solid')

print("\n--Runge Kutta (2)--")
start_time = time.time()
y, err_ave = Runge_Kutta_2(x,H,y_0)
end_time = time.time()
print('RMSE = {:.5e}'.format(err_ave)  + ', time = {:.5e}'.format(end_time-start_time))
pyplot.plot(x, y, label="Runge-Kutta-2" ,marker='o', markersize=0, linewidth=0.8, color='#00cc00',linestyle='solid')

print("\n--Runge Kutta (4)--")
start_time = time.time()
y, err_ave = Runge_Kutta_4(x,H,y_0)
end_time = time.time()
print('RMSE = {:.5e}'.format(err_ave)  + ', time = {:.5e}'.format(end_time-start_time))
pyplot.plot(x, y, label="Runge-Kutta-4" ,marker='o', markersize=0, linewidth=0.8, color='#0000cc',linestyle='solid')

#Answer
for i in range(len(x)):
    y[i] = f(x[i])
pyplot.plot(x, y, label="Ans" ,marker='o', markersize=0, linewidth=0.8, color='#000000',linestyle='solid')

#グラフに描画
pyplot.xlim(a,b)
pyplot.ylim(min(y)-1.0,max(y)+1.0)
pyplot.xlabel('x')
pyplot.ylabel('y')
pyplot.legend()

pyplot.show()