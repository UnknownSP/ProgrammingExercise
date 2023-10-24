#-*- coding: utf-8 -*-
from scipy import integrate
import numpy as np
import time
from matplotlib import pyplot
from matplotlib.animation import FuncAnimation

#三体問題の計算値を返す関数
def func(t, val, m1, m2, m3):
    #(bx, by) Body_nの座標
    #(dx, dy) Body_nの微小時間後の座標　
    bx = 0
    by = 1
    dx = 2
    dy = 3
    
    Body_1 = np.array(val[0:4])
    Body_2 = np.array(val[4:8])
    Body_3 = np.array(val[8:12])

    r12 = np.sqrt((Body_1[bx]-Body_2[bx])**2 + (Body_1[by]-Body_2[by])**2)**3
    r13 = np.sqrt((Body_1[bx]-Body_3[bx])**2 + (Body_1[by]-Body_3[by])**2)**3
    r21 = np.sqrt((Body_2[bx]-Body_1[bx])**2 + (Body_2[by]-Body_1[by])**2)**3
    r23 = np.sqrt((Body_2[bx]-Body_3[bx])**2 + (Body_2[by]-Body_3[by])**2)**3
    r31 = np.sqrt((Body_3[bx]-Body_1[bx])**2 + (Body_3[by]-Body_1[by])**2)**3
    r32 = np.sqrt((Body_3[bx]-Body_2[bx])**2 + (Body_3[by]-Body_2[by])**2)**3

    val_p = np.zeros(len(Body_1)+len(Body_2)+len(Body_3))

    val_p[0] = Body_1[dx]
    val_p[1] = Body_1[dy]
    val_p[2] = m2*(Body_2[bx]-Body_1[bx])/r21 + m3*(Body_3[bx]-Body_1[bx])/r31
    val_p[3] = m2*(Body_2[by]-Body_1[by])/r21 + m3*(Body_3[by]-Body_1[by])/r31
    val_p[4] = Body_2[dx]
    val_p[5] = Body_2[dy]
    val_p[6] = m1*(Body_1[bx]-Body_2[bx])/r12 + m3*(Body_3[bx]-Body_2[bx])/r32
    val_p[7] = m1*(Body_1[by]-Body_2[by])/r12 + m3*(Body_3[by]-Body_2[by])/r32
    val_p[8] = Body_3[dx]
    val_p[9] = Body_3[dy]
    val_p[10] = m1*(Body_1[bx]-Body_3[bx])/r13 + m2*(Body_2[bx]-Body_3[bx])/r23
    val_p[11] = m1*(Body_1[by]-Body_3[by])/r13 + m2*(Body_2[by]-Body_3[by])/r23

    return val_p

#グラフをウィンドウ毎にプロットする関数
def plot(ans,solver,num):
    pyplot.figure(num)
    pyplot.title(solver)
    pyplot.xlim(-5,5)
    pyplot.ylim(-5,5)
    pyplot.xlabel('x')
    pyplot.ylabel('y')
    pyplot.plot(ans.y[0],ans.y[1],label="m1",marker='o', markersize=0, linewidth=0.8, color='#0000cc',linestyle='solid')
    pyplot.plot(ans.y[4],ans.y[5],label="m2",marker='o', markersize=0, linewidth=0.8, color='#00cc00',linestyle='solid')
    pyplot.plot(ans.y[8],ans.y[9],label="m3",marker='o', markersize=0, linewidth=0.8, color='#cc0000',linestyle='solid')
    pyplot.legend(loc='lower right')

t_a = 0.0
t_b = 80.0
P1_0 = [1.0,3.0]
P2_0 = [-2.0,-1.0]
P3_0 = [1.0,-1.0]
m1 = 3.0
m2 = 4.0
m3 = 5.0
Init_val = np.array([P1_0[0],P1_0[1],0.0,0.0,P2_0[0],P2_0[1],0.0,0.0,P3_0[0],P3_0[1],0.0,0.0])

solver = ["Runge-Kutta 2","Runge-Kutta 4","Runge-Kutta 8"]

print("\n--Runge Kutta (2)--")
start_time = time.time()
ans = integrate.solve_ivp(func, [t_a, t_b], Init_val, method='RK23', args=(m1, m2, m3), rtol=1e-13, atol=1e-13)
end_time = time.time()
print('time = {:.5e}'.format(end_time-start_time))
plot(ans,solver[0],0)

print("\n--Runge Kutta (4)--")
start_time = time.time()
ans = integrate.solve_ivp(func, [t_a, t_b], Init_val, method='RK45', args=(m1, m2, m3), rtol=1e-13, atol=1e-13)
end_time = time.time()
print('time = {:.5e}'.format(end_time-start_time))
plot(ans,solver[1],1)

print("\n--Runge Kutta (8)--")
start_time = time.time()
ans = integrate.solve_ivp(func, [t_a, t_b], Init_val, method='DOP853', args=(m1, m2, m3), rtol=1e-13, atol=1e-13)
end_time = time.time()
print('time = {:.5e}'.format(end_time-start_time))
plot(ans,solver[2],2)

pyplot.show()
