import random
import time
import numpy as np
import math
import sys

weight = []
price = []
capacity = 500

for i in range(3000):
    weight.append(random.randint(10,99))
    price.append(random.randint(1,1000))

if len(weight) == len(price):
    num = len(weight)
else:
    exit("Error")

print("\nN = "+str(num))
print("Capacity = "+str(capacity))
print("Weight Range : 10 - 99")
print("Price Range : 1 - 1000")

dp = []
choice = []
start_time = time.time()
dp = [0] * (capacity + 1)
choice = [""] * (capacity + 1)
for i in range(num):
    for j in range(capacity-weight[i],-1,-1):
        if dp[j] + price[i] > dp[j+weight[i]]:
            dp[j+weight[i]] = dp[j] + price[i]
            choice[j+weight[i]] = choice[j] + "1"
        else:
            choice[j+weight[i]] += "0"
    for j in range(weight[i]):
        choice[j] += "0"
choice_indx = []
for i in range(num):
    if int(choice[capacity],2)>>i & 1 == 1:
        choice_indx.append(num-i)
choice_indx.sort()
end_time = time.time()

print("\n--Use DP--")
print("MaxPrice : "+str(dp[capacity])+"\nChoice : "+str(choice_indx)+"\ntime : "+str(end_time-start_time)+" [s]")

dp = []
start_time = time.time()
dp = np.zeros(capacity+1,dtype = int)
for i in range(num):
    dp[weight[i]:] = np.vstack([dp[weight[i]:],dp[:-weight[i]]+price[i]]).max(axis=0)
end_time = time.time()

print("\n--Use DP NumPy--")
print("MaxPrice : "+str(dp[capacity])+"\ntime : "+str(end_time-start_time)+" [s]")

sys.setrecursionlimit(1000000000)

start_time = time.time()
Weight_Price = [np.array([weight[i],price[i]]) for i in range(num)]

#降順ソート
Weight_Price = np.array(sorted(Weight_Price, key=lambda x: x[1] / x[0], reverse=True))

#分枝関数
def BB(num, capacity, Weight_Price, CumSum, ans):
    if num <= 0 or capacity <= 0:
        return 0

    #整数計画問題を貪欲法で解く
    idx = np.searchsorted(CumSum[:, 0], capacity, side="right") - 1
    ans_ip = CumSum[idx, 1]

    #線形計画問題を貪欲法で解く
    if idx < num:
        ans_lp = ans_ip + (capacity - CumSum[idx, 0]) / Weight_Price[idx, 0] * Weight_Price[idx, 1]
    else:
        ans_lp = ans_ip

    if ans_lp <= ans:
        return ans
    ans = max(ans, ans_ip)

    #線形計画問題と整数計画問題の解が一致しない場合は再度分岐
    if ans_lp != ans_ip:
        new_Weight_Price = np.vstack([Weight_Price[:idx], Weight_Price[idx + 1:]])

        nxt_CumSum = CumSum.copy()
        nxt_CumSum[idx + 1:] -= Weight_Price[idx]
        nxt_CumSum = np.vstack([nxt_CumSum[:idx], nxt_CumSum[idx + 1:]])

        if capacity - Weight_Price[idx, 0] >= 0:
            ans = max(ans, BB(
                num - 1, capacity - Weight_Price[idx, 0], new_Weight_Price, nxt_CumSum, ans - Weight_Price[idx, 1]) + Weight_Price[idx, 1])

        if capacity >= 0:
            ans = max(ans, BB(num - 1, capacity, new_Weight_Price, nxt_CumSum, ans))

    return ans


CumSum = np.vstack([np.array([0, 0]), Weight_Price]).cumsum(axis=0)
max_price = BB(num, capacity, Weight_Price, CumSum, 0)
end_time = time.time()

print("\n--Use BB--")
print("MaxPrice : "+str(max_price)+"\ntime : "+str(end_time-start_time)+" [s]\n")

