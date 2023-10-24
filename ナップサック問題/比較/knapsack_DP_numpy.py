import random
import time
import numpy as np

weight = []
price = []
capacity = 500

for i in range(20000000):
    weight.append(random.randint(10,99))
    price.append(random.randint(1,100000))

if len(weight) == len(price):
    num = len(weight)
else:
    exit("Error")

print("\nN = "+str(num))
print("Capacity = "+str(capacity))
print("Weight Range : 10 - 99")
print("Price Range : 1 - 100000")

dp = []
start_time = time.time()
dp = np.zeros(capacity+1,dtype = int)
for i in range(num):
    dp[weight[i]:] = np.vstack([dp[weight[i]:],dp[:-weight[i]]+price[i]]).max(axis=0)
end_time = time.time()

print("\n--Use DP NumPy--")
print("MaxPrice : "+str(dp[capacity])+"\ntime : "+str(end_time-start_time)+" [s]\n")