#-*- coding: utf-8 -*-
import numpy as np
import scipy.linalg
import time

#ヤコビ法によってN元連立1次方程式を解く関数
def Jacobi(A,b,error,maxiter):
    N = len(A)
    D = np.diag(A)
    G = A - np.diag(D)
    x = np.array([0.0]*N)
    n = 0
    while n < maxiter:
        n += 1
        x_n1 = (b - np.dot(G,x)) / D
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
    L = np.tril(mat_A, -1)
    U = np.triu(mat_A, 1)
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
            err += abs( (x[i] - x_cal) / x[i] )
        if err < error:
            break
    return x,n

#係数
mat_A = np.array([
    [ 9.0, 2.0, 1.0, 1.0],
    [ 2.0, 8.0,-2.0, 1.0],
    [-1.0,-2.0, 7.0,-2.0],
    [ 1.0,-1.0,-2.0, 6.0],
])

#定数
mat_b = np.array([
    20.0,
    16.0,
    8.0,
    17.0,
])

#許容誤差
eps = 1e-10
#最大反復回数
max_iter = 100

variable_name = ["x","y","z","u"]
ans = np.zeros(len(mat_A))

print("\n--Jacobi--")
start_time = time.time()
ans, n = Jacobi(mat_A,mat_b,eps,max_iter)
end_time = time.time()
for i in range(len(mat_A)):
    print(variable_name[i]+" = "+str(ans[i]))
print("time = "+str(end_time-start_time))
print("n = "+str(n))

print("\n--Gauss Siedel--")
start_time = time.time()
ans, n = Gauss_Seidel(mat_A,mat_b,eps,max_iter)
end_time = time.time()
for i in range(len(mat_A)):
    print(variable_name[i]+" = "+str(ans[i]))
print("time = "+str(end_time-start_time))
print("n = "+str(n))

print("\n--SOR--")
start_time = time.time()
ans, n = SOR(mat_A,mat_b,eps,max_iter)
end_time = time.time()
for i in range(len(mat_A)):
    print(variable_name[i]+" = "+str(ans[i]))
print("time = "+str(end_time-start_time))
print("n = "+str(n))

print("\n--LU--")
start_time = time.time()
ans = LU(mat_A,mat_b)
end_time = time.time()
for i in range(len(mat_A)):
    print(variable_name[i]+" = "+str(ans[i]))
print("time = "+str(end_time-start_time))