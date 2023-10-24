#-*- coding: utf-8 -*-
from scipy import optimize
import time

#計算対象の関数
def f(x):
    return (x**5) - 25*(x**4) + 220*(x**3) - 780*(x**2) + 870*(x) - 38

def f_prime(x):
    return 5*(x**4) - 100*(x**3) + 660*(x**2) - 1560*(x) + 870

#ニュートン法で解を求める関数
def Newton(function, function_prime, x0, error, maxiter):
    n = 0
    x = x0
    while(abs(function(x)) > error and n <= maxiter):
        x = x - ((function(x))/(function_prime(x)))
        n += 1
    return x


x0 = [0.0, 3.0, 5.0, 8.3, 9.5]  #初期値
ans = [0.0]*5
eps = 1e-12 #許容誤差

print("--scipy--")
start_time = time.time()
for i in range(5):
    ans[i] = optimize.newton(f, x0[i], tol=eps, maxiter=100)
end_time = time.time()
for i in range(5):
    print("x_"+str(i)+" = "+str(ans[i]))
print("time = "+str(end_time-start_time))

print("--original--")
start_time = time.time()
for i in range(5):
    ans[i] = Newton(f, f_prime, x0[i], eps, 100)
end_time = time.time()
for i in range(5):
    print("x_"+str(i)+" = "+str(ans[i]))
print("time = "+str(end_time-start_time))