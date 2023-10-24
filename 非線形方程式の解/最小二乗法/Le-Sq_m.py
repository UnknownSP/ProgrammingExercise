#-*- coding: utf-8 -*-
import numpy as np
import scipy.linalg
import time
from matplotlib import pyplot

m = 1   #最高次数
n = m+1 #項数

#xデータ
x = np.array([
    0.10,
    0.20,
    0.30,
    0.40,
    0.50,
    0.60,
    0.70,
    0.80,
    0.90,
    1.00
])

#yデータ
y = np.array([
    2.4824,
    1.9975,
    1.6662,
    1.3775,
    1.0933,
    0.7304,
    0.4344,
    0.2981,
    -0.0017,
    -0.0026,
])

#係数計算行列
mat_A = np.zeros((n,n))
mat_B = np.zeros(n)
#係数
coeff = np.zeros(n)

#合計を求める際の計算回数を減らす為、正方行列を斜めに探索し値を代入
for i in range(2*m+1):
    sum = np.sum(x**i)
    if i < n:
        mat_A[0][i] = sum
    for j in reversed(range(i)):
        if (i-j < n) and (j < n):
            mat_A[i-j][j] = sum

for i in range(n):
    mat_B[i] = np.sum(x**i * y)

#行列から係数を計算
coeff = np.dot(np.linalg.inv(mat_A), mat_B)

#表示用数式
form = "y = "
for i in reversed(range(n)):
    if (coeff[i] >= 0) and (i != n-1):
        form += "+ "
    form += str(coeff[i])
    if i != 0:
        form += "*x^" + str(i) + " "

print(form)

#グラフの表示
p = np.linspace(np.min(x)-0.1,np.max(x)+0.1,100)
q = 0
for i in range(n):
    q += coeff[i] * p**i

pyplot.plot(x, y, marker='o', markersize=8, color='red',linestyle='None')
pyplot.plot(p,q,label="m = "+str(m),color='#000000')
pyplot.xlabel('x')
pyplot.ylabel('y')
pyplot.xlim(np.min(x)-0.1,np.max(x)+0.1)
pyplot.ylim(np.min(y)-0.1,np.max(y)+0.1)
pyplot.legend()

pyplot.show()