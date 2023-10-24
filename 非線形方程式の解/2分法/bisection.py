#-*- coding: utf-8 -*-
from scipy import optimize
import time

#計算対象の関数
def f(x):
    return (x**5) - 25*(x**4) + 220*(x**3) - 780*(x**2) + 870*(x) - 38

#2分法で解を求める関数
def bisec(function, a, b, error):
    if function(a)*function(b) > 0.0:
        print("Section error")
        exit()
    while(b-a > error):
        c = (a+b)/2.0
        if(function(a)*function(c) < 0.0):
            b = c
        else:
            a = c
    return (a+b)/2.0


a = [-1.0, 1.0, 5.0, 7.0, 8.5] #区間a
b = [1.0, 3.0, 7.0, 8.5, 9.5]  #区間b
ans = [0.0]*5
eps = 1e-7 #許容誤差

print("--scipy--")
start_time = time.time()
for i in range(5):
    ans[i] = optimize.bisect(f, a[i], b[i])
end_time = time.time()
for i in range(5):
    print("x_"+str(i)+" = "+str(ans[i]))
print("time = "+str(end_time-start_time))

print("--original--")
start_time = time.time()
for i in range(5):
    ans[i] = bisec(f, a[i], b[i], eps)
end_time = time.time()
for i in range(5):
    print("x_"+str(i)+" = "+str(ans[i]))
print("time = "+str(end_time-start_time))