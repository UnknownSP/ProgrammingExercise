#-*- coding: utf-8 -*-
import numpy as np
import scipy.linalg
import time

#ガウスの消去法によってN元連立1次方程式を解く関数(ピボット選択あり)
def Elimination(A,b,min_err):
    N = len(A)
    for i in range(N):
        #ピボットは考慮する列において最大のものを選択し、入れ替える
        Max_idx = np.argmax(np.abs(A[i:N]), axis=0)[i] + i
        A_temp = A[i].copy()
        b_temp = b[i].copy()
        A[i] = A[Max_idx]
        b[i] = b[Max_idx]
        A[Max_idx] = A_temp
        b[Max_idx] = b_temp

        pivot = A[i][i]
        if abs(pivot) < min_err:
            print("pivot error")
            exit()
        
        #行基本変形を用いてAの対角成分を1、左下三角を0にする
        A[i] = A[i] / pivot
        b[i] = b[i] / pivot
        for j in range(i+1,N):
            coeff = A[j][i]
            A[j] -= coeff * A[i]
            b[j] -= coeff * b[i]
    
    #求まった行列から各変数の値を計算
    x = np.zeros(N)
    for i in reversed(range(N)):
        temp = 0.0
        for j in range(i+1,N):
            temp += x[j] * A[i][j]
        x[i] = (b[i]-temp) / A[i][i]

    return x

#係数
mat_A = np.array([
    [ 2.0, 7.0,-1.0, 5.0,-3.0],
    [ 1.0,-4.0, 2.0,-1.0, 6.0],
    [ 3.0, 1.0,-9.0,-2.0, 1.0],
    [10.0,-2.0,-5.0, 8.0,-7.0],
    [-4.0, 3.0,12.0,-4.0,-2.0],
])

#定数
mat_b = np.array([
    6.0,
    1.0,
    -2.0,
    4.0,
    -10.0,
])

variable_name = ["x","y","z","u","w"]

print("\n--original--")
start_time = time.time()
ans = np.zeros(len(mat_A))
ans = Elimination(mat_A,mat_b,1e-10)
end_time = time.time()
for i in range(len(mat_A)):
    print(variable_name[i]+" = "+str(ans[i]))
print("time = "+str(end_time-start_time))

print("\n--scipy--")
start_time = time.time()
ans = scipy.linalg.solve(mat_A,mat_b)
end_time = time.time()
for i in range(len(mat_A)):
    print(variable_name[i]+" = "+str(ans[i]))
print("time = "+str(end_time-start_time))