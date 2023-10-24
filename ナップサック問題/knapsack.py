from ortoolpy import knapsack
import random
import time
import numpy as np

weight = [13,23,27,33,41,45,58,60]
price = [15,30,40,60,70,80,85,90]
capacity = 100
num = 0

for i in range(9992):
    weight.append(random.randint(10,99))
    price.append(random.randint(1,1000))

if len(weight) == len(price):
    num = len(weight)
else:
    exit("Error")

start_time = time.time()
ans = knapsack(weight,price,capacity)
choice_indx = list(map(lambda x: x+1,ans[1]))
end_time = time.time()

print("\n--Use knapsack Function--")
print("MaxPrice : "+str(ans[0])+"\nChoice : "+str(choice_indx)+"\ntime : "+str(end_time-start_time))

start_time = time.time()
dp = [[0]*(capacity+1) for j in range(num+1)]
choice = [[""]*(capacity+1) for j in range(num+1)]
choice_indx = []
for i in range(num):
    for j in range(capacity+1):
        if j >= weight[i]:
            dp[i+1][j] = max(dp[i][j],dp[i][j-weight[i]]+price[i])
            if dp[i+1][j] != dp[i][j]:
                choice[i+1][j] = choice[i][j-weight[i]] + "1"
            else:
                choice[i+1][j] = choice[i][j] + "0"
        else:
            dp[i+1][j] = dp[i][j]
            choice[i+1][j] = choice[i][j] + "0"
for i in range(num):
    if int(choice[num][capacity],2)>>i & 1 == 1:
        choice_indx.append(num-i)
choice_indx.sort()
end_time = time.time()

print("\n--Use DP--")
print("MaxPrice : "+str(dp[num][capacity])+"\nChoice : "+str(choice_indx)+"\ntime : "+str(end_time-start_time))

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

print("\n--Use DP Reverse--")
print("MaxPrice : "+str(dp[capacity])+"\nChoice : "+str(choice_indx)+"\ntime : "+str(end_time-start_time))

dp = []
start_time = time.time()
dp = np.zeros(capacity+1,dtype = int)
for i in range(num):
    dp[weight[i]:] = np.vstack([dp[weight[i]:],dp[:-weight[i]]+price[i]]).max(axis=0)
end_time = time.time()

print("\n--Use DP NumPy--")
print("MaxPrice : "+str(dp[capacity])+"\ntime : "+str(end_time-start_time))

