import sys, math, time, random
import pulp
import tkinter as tk
import numpy as np

node = [
    [23,39],[ 8,44],[34,36],[12,30],[42,37],[ 6,35],[ 1,15],[12,25],
    [ 4,39],[13,42],[23,13],[ 7,39],[11, 5],[ 6,44],[28,45],[20, 7],
    [ 3,16],[ 4,19],[ 3,39],[ 0, 2],[19,21],[ 3,43],[ 8,34],[20,39],
    [ 2,50],[20,26],[16,36],[24,30],[ 9,40],[ 5,22],[30,35],[ 2, 0],
    [21,36],[22,28],[ 3,33],[11,36],[14,34]
]

def pulp_solve(cal_node):
    size = len(cal_node)
    w = [[math.sqrt((p1[0] - p2[0])**2 + (p1[1] - p2[1])**2) for p2 in cal_node] for p1 in cal_node]

    problem = pulp.LpProblem('tsp')

    x = []
    for i in range(size):
        x1 = [pulp.LpVariable('x{}_{}'.format(i, j), cat='Binary') for j in range(size)]
        x.append(x1)
    us = [pulp.LpVariable('u{}'.format(i), cat='Integer', lowBound=0, upBound=size-1) for i in range(size)]

    problem += pulp.lpSum([pulp.lpDot(_w, _x) for _w, _x in zip(w, x)])

    for i in range(size):
        problem += pulp.lpSum([x[j][i] for j in range(size)]) == 1
        problem += pulp.lpSum([x[i][j] for j in range(size)]) == 1

    for i in range(size): problem += x[i][i] == 0

    for i in range(1, size):
        for j in range(1, size):
            if i == j: continue
            problem += us[i] + 1 - size * (1 - x[i][j]) + (size - 3) * x[j][i] <= us[j]
    for i in range(1, size):
        problem += 1 + (1 - x[0][i]) + (size - 3) * x[i][0] <= us[i]
        problem += us[i] <= (size - 1) - (1 - x[i][0]) - (size - 3) * x[0][i]

    problem += us[0] == 0
    problem.solve()
    return problem.objective.value()

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

def greedy_solve(size):
    global distance_table
    path = list(range(size))
    for i in range(size - 1):
        min_len = 1000000
        min_pos = 0
        for j in range(i + 1, size):
            len = distance_table[path[i]][path[j]]
            if len < min_len:
                min_len = len
                min_pos = j
        path[i + 1], path[min_pos] = path[min_pos], path[i + 1]
    return path

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

def or_opt_solve(size, path):
    global distance_table
    total = 0
    while True:
        count = 0
        for i in range(size):
            i0 = i - 1
            i1 = i + 1
            if i0 < 0: i0 = size - 1
            if i1 == size: i1 = 0
            for j in range(size):
                j1 = j + 1
                if j1 == size: j1 = 0
                if j != i and j1 != i:
                    l1 = distance_table[path[i0]][path[i]]
                    l2 = distance_table[path[i]][path[i1]]
                    l3 = distance_table[path[j]][path[j1]]
                    l4 = distance_table[path[i0]][path[i1]]
                    l5 = distance_table[path[j]][path[i]]
                    l6 = distance_table[path[i]][path[j1]] 
                    if l1 + l2 + l3 > l4 + l5 + l6:
                        p = path[i]
                        path[i:i + 1] = []
                        if i < j:
                            path[j:j] = [p]
                        else:
                            path[j1:j1] = [p]
                        count += 1
        total += count
        if count == 0: break
    return path, total

start_time = time.time()
length = pulp_solve(node)
end_time = time.time()
print("\n--PulP solve--")
print("Min Length : "+str(length)+"\ntime : "+str(end_time-start_time))

start_time = time.time()
node_len = len(node)
distance_table = distance(node)
path = greedy_solve(node_len)
length = path_length(path)
end_time = time.time()
greedy_solve_time = end_time-start_time
print("\n--Greedy solve--")
print("Min Length : "+str(length)+"\ntime : "+str(end_time-start_time))

start_time = time.time()
path, x = opt_2_solve(node_len, path)
length = path_length(path)
end_time = time.time()+greedy_solve_time
print("\n--2 Opt solve--")
print("Min Length : "+str(length)+"\ntime : "+str(end_time-start_time))

start_time = time.time()
path, x = or_opt_solve(node_len, path)
length = path_length(path)
end_time = time.time()+greedy_solve_time
print("\n--or Opt solve--")
print("Min Length : "+str(length)+"\ntime : "+str(end_time-start_time))