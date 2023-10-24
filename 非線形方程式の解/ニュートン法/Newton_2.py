#-*- coding: utf-8 -*-
from scipy import optimize
import numpy as np
import time

#連立方程式の値を返すscipyで解くための関数
def Sim_Eq(x):
    return[x[0]**2+x[1]**2-1.0, x[1]-x[0]**3]

#連立方程式の式1
def f(x,y):
    return x**2+y**2-1

#連立方程式の式2
def g(x,y):
    return y-x**3

#f(x,y)をxで偏微分した関数
def f_x(x,y):
    return 2*x

#f(x,y)をyで偏微分した関数
def f_y(x,y):
    return 2*y

#g(x,y)をxで偏微分した関数
def g_x(x,y):
    return -3*(x**2)

#g(x,y)をyで偏微分した関数
def g_y(x,y):
    return 1

#ヤコビアンを用いたニュートン法によって連立方程式を解く関数
def Newton_2(init_x,init_y,error,maxiter):
    n = 0
    x = init_x
    y = init_y
    while(n < maxiter):
        n += 1
        J_det = f_x(x,y)*g_y(x,y)-f_y(x,y)*g_x(x,y)
        x_n1 = x - ( g_y(x,y)*f(x,y) - f_y(x,y)*g(x,y)) / J_det
        y_n1 = y - (-g_x(x,y)*f(x,y) + f_x(x,y)*g(x,y)) / J_det
        if( (x_n1-x)**2 + (y_n1-y)**2 < error**2 ):
            break
        x=x_n1
        y=y_n1
    return x_n1,y_n1,n

x0 = [1.0,-1.0] #初期値
y0 = [1.0,-1.0] 
eps = 1.0e-5   #許容誤差

ans_x = [0.0]*2
ans_y = [0.0]*2
n = [0]*2
ans = [""]*2

print("--scipy--")
start_time = time.time()
for i in range(2):
    ans[i] = optimize.root(Sim_Eq,[x0[i],y0[i]], tol=eps, method="krylov")
end_time = time.time()
for i in range(2):
    print("x_"+str(i)+" = "+str(ans[i].x[0])+", y_"+str(i)+" = "+str(ans[i].x[1])+", n = "+str(ans[i].nit))
    #print(ans[i])
print("time = "+str(end_time-start_time))

print("\n--original--")
start_time = time.time()
for i in range(2):
    ans_x[i], ans_y[i], n[i] = Newton_2(x0[i],y0[i],eps,100)
end_time = time.time()
for i in range(2):
    print("x_"+str(i)+" = "+str(ans_x[i])+", y_"+str(i)+" = "+str(ans_y[i])+", n = "+str(n[i]))
print("time = "+str(end_time-start_time))