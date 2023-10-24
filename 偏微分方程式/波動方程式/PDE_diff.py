#-*- coding: utf-8 -*-
from scipy import integrate
import numpy as np
import time
import matplotlib.pyplot as pyplot
from matplotlib.animation import FuncAnimation

# 初期条件 : t=0 (u(x,0)) における状態
def init_func(x):
    return 0.05*x*(2.0-x)

print("\n--PDE diff--")
start_time = time.time()

t_a = 0.0   #開始時刻
t_b = 5.0   #終了時刻
t_H = 0.001 #時刻の刻み幅
t_n = int((t_b-t_a)/t_H)

x_a = 0.0   #始端
x_b = 2.0   #終端
x_H = 0.01  #xの刻み幅
x_n = int((x_b-x_a)/x_H) + 1 #u(2,t)=0を使用するため1つ多くする

c_2 = 9.0 #係数
a = 0.09 # a = c_2 * ((t_H**2)/(x_H**2))

x_i = np.arange(x_a, x_b+x_H, x_H)
u = np.zeros((x_n,t_n))

#境界条件 (Dirichlet条件)
u[0,:] = 0.0
u[x_n-1,:] = 0.0

#t=0における状態を計算
for x in range(1,x_n-1):
    u[x,0] = init_func(x_a + x_H*x)

#t=1における状態を計算
for x in range(1,x_n-1):
    u[x,1] = u[x,0] + (a/2.0)*(u[x+1,0]-2*u[x,0]+u[x-1,0])

#t=n (n>=2) における状態を逐次計算
for t in range(1,t_n-1):
    for x in range(1,x_n-1):
        u[x,t+1] = 2.0*u[x,t] - u[x,t-1] + a * ( u[x+1,t] - 2*u[x,t] + u[x-1,t] )

end_time = time.time()
print('time = {:.5e}'.format(end_time-start_time))

def plot(t):
    pyplot.cla()
    pyplot.ylim([-0.1,0.1])
    pyplot.plot(x_i,u[:,t*40],color='#000000')

fig = pyplot.figure()
animation = FuncAnimation(fig, plot, frames=int(t_n/40.0)-1, interval=1)
#pyplot.show()
animation.save("PDE_diff.gif", writer="pillow")
