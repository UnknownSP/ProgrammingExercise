import math, time, random
import numpy as np
from vcopt import vcopt

node = [
    [23,39],[ 8,44],[34,36],[12,30],[42,37],[ 6,35],[ 1,15],[12,25],
    [ 4,39],[13,42],[23,13],[ 7,39],[11, 5],[ 6,44],[28,45],[20, 7],
    [ 3,16],[ 4,19],[ 3,39],[ 0, 2],[19,21],[ 3,43],[ 8,34],[20,39],
    [ 2,50],[20,26],[16,36],[24,30],[ 9,40],[ 5,22],[30,35],[ 2, 0],
    [21,36],[22,28],[ 3,33],[11,36],[14,34]
]

def distance(cal_node):
    size = len(cal_node)
    return_table = [[0] * size for x in range(size)]
    for i in range(size):
        for j in range(size):
            if i != j:
                dx = cal_node[i][0] - cal_node[j][0]
                dy = cal_node[i][1] - cal_node[j][1]
                return_table[i][j] = math.sqrt(dx * dx + dy * dy)
    return return_table

def path_length(path):
    global distance_table
    n = 0
    for i in range(1, len(path)):
        n += distance_table[path[i - 1]][path[i]]
    n += distance_table[path[0]][path[-1]]
    return n

def opt_2_solve(size, path):
    global distance_table
    total = 0
    while True:
        count = 0
        for i in range(size - 2):
            i1 = i + 1
            for j in range(i + 2, size):
                if j == size - 1:
                    j1 = 0
                else:
                    j1 = j + 1
                if i != 0 or j1 != 0:
                    l1 = distance_table[path[i]][path[i1]]
                    l2 = distance_table[path[j]][path[j1]]
                    l3 = distance_table[path[i]][path[j]]
                    l4 = distance_table[path[i1]][path[j1]]
                    if l1 + l2 > l3 + l4:
                        new_path = path[i1:j+1]
                        path[i1:j+1] = new_path[::-1]
                        count += 1
        total += count
        if count == 0: break
    return path, total

def NN(size):
    global distance_table
    cal_path = np.delete(np.arange(size),0,0)
    return_path = np.array([0])
    for i in range(int(size)-1):
        for j in range(len(cal_path)):
            if j==0 or (min_len > distance_table[return_path[i]][cal_path[j]]):
                del_idx = j
                min_idx = cal_path[j]
                min_len = distance_table[return_path[i]][cal_path[j]]
        return_path = np.insert(return_path,len(return_path),min_idx)
        cal_path = np.delete(cal_path,del_idx,0)
    return return_path

def tsp_1_score(path):
    path_full = np.hstack((0, path))
    return path_length(path_full)

#各ノード間距離のテーブルを作成
distance_table = distance(node)
node_len = len(node)

print("\n--Nearest Neighber solve--")
start_time = time.time()
#NearestNeighber法で最適化
min_path = NN(node_len)
min_length = path_length(min_path)
min_path = np.hstack((min_path,0))
end_time = time.time()
print("Min Length : "+str(min_length)+"\ntime : "+str(end_time-start_time))
print("Route : " + str(min_path))

print("\n--2 Opt solve--")
start_time = time.time()
#道順を作成
path = np.arange(node_len)
min_length = 0
#2-opt法で最適化し、一番スコアの良かったものを記録
for i in range(10):
    #道順をランダムに並び替える
    np.random.shuffle(path)
    path, x = opt_2_solve(node_len, path)
    length = path_length(path)
    if i == 0 or min_length > length:
        min_length = length
        min_path = np.array(path)
min_path = np.hstack((np.roll(min_path,-np.where(min_path == 0)[0][0]),0))
end_time = time.time()
print("Min Length : "+str(min_length)+"\ntime : "+str(end_time-start_time))
print("Route : " + str(min_path))

print("\n--2 Opt solve (vcopt)--")
start_time = time.time()
#道順を作成
path = np.arange(1,node_len)
min_length = 0
#2-opt法(vcopt)で最適化し、一番スコアの良かったものを記録
for i in range(10):
    #道順をランダムに並び替える
    np.random.shuffle(path)
    path, length = vcopt().opt2(path, tsp_1_score, 0.0, seed=None)
    if i == 0 or length < min_length:
        min_length = length
        min_path = np.array(path)
min_path = np.hstack((0,min_path,0))
end_time = time.time()
print("Min Length : "+str(min_length)+"\ntime : "+str(end_time-start_time))
print("Route : " + str(min_path))