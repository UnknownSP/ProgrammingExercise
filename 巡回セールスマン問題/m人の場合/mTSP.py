import sys, math, time, random
import numpy as np
import numpy.random as nr
import matplotlib.pyplot as plt
from vcopt import vcopt

node = [
    [23,39],[ 8,44],[34,36],[12,30],[42,37],[ 6,35],[ 1,15],[12,25],
    [ 4,39],[13,42],[23,13],[ 7,39],[11, 5],[ 6,44],[28,45],[20, 7],
    [ 3,16],[ 4,19],[ 3,39],[ 0, 2],[19,21],[ 3,43],[ 8,34],[20,39],
    [ 2,50],[20,26],[16,36],[24,30],[ 9,40],[ 5,22],[30,35],[ 2, 0],
    [21,36],[22,28],[ 3,33],[11,36],[14,34]
]

#1TSPの評価関数
def tsp_1_score(param):

    #idが0のノードからスタートし0に戻ってくるように、先頭と末尾に0を追加
    param_full = np.hstack((0, param, 0))

    return np.sum(((node_x[param_full][:-1] - node_x[param_full][1:])**2 + (node_y[param_full][:-1] - node_y[param_full][1:])**2)**0.5)

#2TSPの評価関数
def tsp_2_score(param):
    #idが0のノードを中心に2つにわけ、それぞれ0に戻ってくるように、末尾に0を追加
    param_1 = np.hstack((param[:np.where(param==0)[0][0] + 1][::-1],0))
    param_2 = np.hstack((param[np.where(param==0)[0][0]:],0))

    #評価
    score_1 = np.sum(((node_x[param_1][:-1] - node_x[param_1][1:])**2 + (node_y[param_1][:-1] - node_y[param_1][1:])**2)**0.5)
    score_2 = np.sum(((node_x[param_2][:-1] - node_x[param_2][1:])**2 + (node_y[param_2][:-1] - node_y[param_2][1:])**2)**0.5)

    #距離が長い方を返す
    return max(score_1, score_2)

#3TSPの評価関数
def tsp_3_score(param):
    #idが0のノード2つを境に3つにわけ、それぞれ0に戻ってくるように、末尾に0を追加
    param_1 = np.hstack((param[:np.where(param==0)[0][0]+1][::-1],0))
    param_temp = param[np.where(param==0)[0][0]:]
    param_2 = np.hstack((param_temp[:np.where(param_temp==0)[0][1]],0))
    param_3 = np.hstack((param_temp[np.where(param_temp==0)[0][1]:],0))

    #評価
    score_1 = np.sum(((node_x[param_1][:-1] - node_x[param_1][1:])**2 + (node_y[param_1][:-1] - node_y[param_1][1:])**2)**0.5)
    score_2 = np.sum(((node_x[param_2][:-1] - node_x[param_2][1:])**2 + (node_y[param_2][:-1] - node_y[param_2][1:])**2)**0.5)
    score_3 = np.sum(((node_x[param_3][:-1] - node_x[param_3][1:])**2 + (node_y[param_3][:-1] - node_y[param_3][1:])**2)**0.5)

    return max(score_1,score_2,score_3)

#4TSPの評価関数
def tsp_4_score(param):
    #idが0のノード3つを境に4つにわけ、それぞれ0に戻ってくるように、末尾に0を追加
    param_1 = np.hstack((param[:np.where(param==0)[0][0]+1][::-1],0))
    param_temp = param[np.where(param==0)[0][0]:]
    param_2 = np.hstack((param_temp[:np.where(param_temp==0)[0][1]],0))
    param_temp = param_temp[np.where(param_temp==0)[0][1]:]
    param_3 = np.hstack((param_temp[:np.where(param_temp==0)[0][1]],0))
    param_4 = np.hstack((param_temp[np.where(param_temp==0)[0][1]:],0))

    #評価
    score_1 = np.sum(((node_x[param_1][:-1] - node_x[param_1][1:])**2 + (node_y[param_1][:-1] - node_y[param_1][1:])**2)**0.5)
    score_2 = np.sum(((node_x[param_2][:-1] - node_x[param_2][1:])**2 + (node_y[param_2][:-1] - node_y[param_2][1:])**2)**0.5)
    score_3 = np.sum(((node_x[param_3][:-1] - node_x[param_3][1:])**2 + (node_y[param_3][:-1] - node_y[param_3][1:])**2)**0.5)
    score_4 = np.sum(((node_x[param_4][:-1] - node_x[param_4][1:])**2 + (node_y[param_4][:-1] - node_y[param_4][1:])**2)**0.5)

    return max(score_1,score_2,score_3,score_4)
    

def get_route_length(route):
    length = 0
    for i in range(len(route)-1):
        length += math.sqrt((node[route[i]][0]-node[route[i+1]][0])**2 + (node[route[i]][1]-node[route[i+1]][1])**2)
    return length


node_x = np.array([0]*len(node))
node_y = np.array([0]*len(node))
for i in range(len(node)):
    node_x[i] = node[i][0]
    node_y[i] = node[i][1]

print("\n-------1 TSP-------")

print("\n--2 Opt solve (1 TSP)--")
start_time = time.time()
#道順を作成
param = np.arange(1,len(node))
min_length = 1000.0
#2-opt法で最適化し、一番スコアの良かったものを記録
for i in range(20):
    #道順をランダムに並び替える
    np.random.shuffle(param)
    param, length = vcopt().opt2(param, tsp_1_score, 0.0, seed=None)
    if length < min_length:
        min_length = length
        min_param = np.array(param)
end_time = time.time()
print("Min Length : "+str(min_length)+"\ntime : "+str(end_time-start_time))
print("Route : " + str(np.hstack((0,min_param,0))))

print("\n--GA solve (1 TSP)--")
start_time = time.time()
#パラメータの範囲を定義
param_range = np.arange(1,len(node))
#GAで最適化
param, length = vcopt().tspGA(param_range, tsp_1_score, 0.0, seed=None)
end_time = time.time()
print("Min Length : "+str(length)+"\ntime : "+str(end_time-start_time))
print("Route : " + str(np.hstack((0,param,0))))

print("\n-------2 TSP-------")

print("\n--2 Opt solve (2 TSP)--")
start_time = time.time()
#道順を作成
param = np.arange(len(node))
min_length = 1000.0
#2-opt法で最適化
for i in range(100):
    #道順をランダムに並び替える
    np.random.shuffle(param)
    param, max_length = vcopt().opt2(param, tsp_2_score, 0.0, seed=None)
    if max_length < min_length:
        min_length = max_length
        min_param = np.array(param)
end_time = time.time()

route_1 = np.hstack((min_param[:np.where(min_param==0)[0][0] + 1][::-1],0))
route_2 = np.hstack((min_param[np.where(min_param==0)[0][0]:],0))
route_1_len = get_route_length(route_1)
route_2_len = get_route_length(route_2)
print("Max Length : "+str(min_length)+"\ntime : "+str(end_time-start_time))
print("\nRoute 1 : " + str(route_1) + "\nRoute 1 Length : " + str(route_1_len))
print("\nRoute 2 : " + str(route_2) + "\nRoute 2 Length : " + str(route_2_len))

print("\n--GA solve (2 TSP)--")
start_time = time.time()
#パラメータの範囲を定義
param_range = np.arange(len(node))
#GAで最適化
param, max_length = vcopt().tspGA(param_range, tsp_2_score, 0.0, seed=None)
end_time = time.time()

route_1 = np.hstack((param[:np.where(param==0)[0][0] + 1][::-1],0))
route_2 = np.hstack((param[np.where(param==0)[0][0]:],0))
route_1_len = get_route_length(route_1)
route_2_len = get_route_length(route_2)
print("Max Length : "+str(max_length)+"\ntime : "+str(end_time-start_time))
print("\nRoute 1 : " + str(route_1) + "\nRoute 1 Length : " + str(route_1_len))
print("\nRoute 2 : " + str(route_2) + "\nRoute 2 Length : " + str(route_2_len))

print("\n-------3 TSP-------")

print("\n--2 Opt solve (3 TSP)--")
start_time = time.time()
#道順を作成 0を追加し配列内に2つ0を存在させることで、0で配列を区切った場合に3つルートができるようにしている。
param = np.hstack((np.arange(len(node)),0))
min_length = 1000.0
#2-opt法で最適化
for i in range(100):
    #道順をランダムに並び替える
    np.random.shuffle(param)
    param, max_length = vcopt().opt2(param, tsp_3_score, 0.0, seed=None)
    if max_length < min_length:
        min_length = max_length
        min_param = np.array(param)
end_time = time.time()

route_1 = np.hstack((min_param[:np.where(min_param==0)[0][0]+1][::-1],0))
route_temp = min_param[np.where(min_param==0)[0][0]:]
route_2 = np.hstack((route_temp[:np.where(route_temp==0)[0][1]],0))
route_3 = np.hstack((route_temp[np.where(route_temp==0)[0][1]:],0))
route_1_len = get_route_length(route_1)
route_2_len = get_route_length(route_2)
route_3_len = get_route_length(route_3)
print("Max Length : "+str(min_length)+"\ntime : "+str(end_time-start_time))
print("\nRoute 1 : " + str(route_1) + "\nRoute 1 Length : " + str(route_1_len))
print("\nRoute 2 : " + str(route_2) + "\nRoute 2 Length : " + str(route_2_len))
print("\nRoute 3 : " + str(route_3) + "\nRoute 3 Length : " + str(route_3_len))

print("\n--GA solve (3 TSP)--")
start_time = time.time()
#パラメータの範囲を定義
param_range = np.hstack((np.arange(len(node)),0))
np.random.shuffle(param_range)
#GAで最適化
param, max_length = vcopt().tspGA(param_range, tsp_3_score, 0.0, seed=None)
end_time = time.time()

route_1 = np.hstack((param[:np.where(param==0)[0][0]+1][::-1],0))
route_temp = param[np.where(param==0)[0][0]:]
route_2 = np.hstack((route_temp[:np.where(route_temp==0)[0][1]],0))
route_3 = np.hstack((route_temp[np.where(route_temp==0)[0][1]:],0))
route_1_len = get_route_length(route_1)
route_2_len = get_route_length(route_2)
route_3_len = get_route_length(route_3)
print("Max Length : "+str(max_length)+"\ntime : "+str(end_time-start_time))
print("\nRoute 1 : " + str(route_1) + "\nRoute 1 Length : " + str(route_1_len))
print("\nRoute 2 : " + str(route_2) + "\nRoute 2 Length : " + str(route_2_len))
print("\nRoute 3 : " + str(route_3) + "\nRoute 3 Length : " + str(route_3_len))

print("\n-------4 TSP-------")

print("\n--2 Opt solve (4 TSP)--")
start_time = time.time()
#道順を作成　0を2つ追加し配列内に3つ0を存在させることで、0で配列を区切った場合に4つルートができるようにしている。
param = np.hstack((np.arange(len(node)),0,0))
min_length = 1000.0
#2-opt法で最適化
for i in range(100):
    #道順をランダムに並び替える
    np.random.shuffle(param)
    param, max_length = vcopt().opt2(param, tsp_4_score, 0.0, seed=None)
    if max_length < min_length:
        min_length = max_length
        min_param = np.array(param)
end_time = time.time()

route_1 = np.hstack((min_param[:np.where(min_param==0)[0][0]+1][::-1],0))
route_temp = min_param[np.where(min_param==0)[0][0]:]
route_2 = np.hstack((route_temp[:np.where(route_temp==0)[0][1]],0))
route_temp = route_temp[np.where(route_temp==0)[0][1]:]
route_3 = np.hstack((route_temp[:np.where(route_temp==0)[0][1]],0))
route_4 = np.hstack((route_temp[np.where(route_temp==0)[0][1]:],0))
route_1_len = get_route_length(route_1)
route_2_len = get_route_length(route_2)
route_3_len = get_route_length(route_3)
route_4_len = get_route_length(route_4)
print("Max Length : "+str(min_length)+"\ntime : "+str(end_time-start_time))
print("\nRoute 1 : " + str(route_1) + "\nRoute 1 Length : " + str(route_1_len))
print("\nRoute 2 : " + str(route_2) + "\nRoute 2 Length : " + str(route_2_len))
print("\nRoute 3 : " + str(route_3) + "\nRoute 3 Length : " + str(route_3_len))
print("\nRoute 4 : " + str(route_4) + "\nRoute 4 Length : " + str(route_4_len))

print("\n--GA solve (4 TSP)--")
start_time = time.time()
#パラメータの範囲を定義
param_range = np.hstack((np.arange(len(node)),0,0))
np.random.shuffle(param_range)
#GAで最適化
param, max_length = vcopt().tspGA(param_range, tsp_4_score, 0.0, seed=None)
end_time = time.time()

route_1 = np.hstack((param[:np.where(param==0)[0][0]+1][::-1],0))
route_temp = param[np.where(param==0)[0][0]:]
route_2 = np.hstack((route_temp[:np.where(route_temp==0)[0][1]],0))
route_temp = route_temp[np.where(route_temp==0)[0][1]:]
route_3 = np.hstack((route_temp[:np.where(route_temp==0)[0][1]],0))
route_4 = np.hstack((route_temp[np.where(route_temp==0)[0][1]:],0))
route_1_len = get_route_length(route_1)
route_2_len = get_route_length(route_2)
route_3_len = get_route_length(route_3)
route_4_len = get_route_length(route_4)
print("Max Length : "+str(max_length)+"\ntime : "+str(end_time-start_time))
print("\nRoute 1 : " + str(route_1) + "\nRoute 1 Length : " + str(route_1_len))
print("\nRoute 2 : " + str(route_2) + "\nRoute 2 Length : " + str(route_2_len))
print("\nRoute 3 : " + str(route_3) + "\nRoute 3 Length : " + str(route_3_len))
print("\nRoute 4 : " + str(route_4) + "\nRoute 4 Length : " + str(route_4_len))