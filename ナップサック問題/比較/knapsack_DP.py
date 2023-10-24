import random
import time
import numpy as np

weight = []
price = []
capacity = 500

for i in range(100000):
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
print("MaxPrice : "+str(dp[capacity])+"\ntime : "+str(end_time-start_time)+" [s]\n")