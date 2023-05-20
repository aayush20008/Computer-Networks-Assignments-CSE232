import matplotlib.pyplot as plt
import numpy as np

file = open('tcp-example.tr','r')
df = file.readlines()

t_delay =0 
cnt = 0
queue = dict()
X = list()
Y = list()
or_val = 1


for i in df:
    
    val = i.strip().split()
    time = float(val[1])
    seqVal = (val[6*6][4:])
    seq = int(seqVal)
    
    dequeue = bool(val[0] == '-')
    enqueue = bool(val[0] == '+')
    
    if enqueue == True:
        queue[seq] = time
    else:
        if seq not in queue.keys():
            continue
        delay = time - queue[seq]
         
        if len(X) > 0 and X[-1] == time:
            Y[-1] = (cnt * Y[-1]  + delay)/(cnt + 1)
            cnt = cnt + 1
        else:
            cnt = or_val
            X.append(time)
            Y.append(delay)
        queue.pop(seq)


xp= np.array(X)
yp = np.array(Y)

plt.plot(xp,yp,"bo")

plt.xlabel("Time")
plt.ylabel("Queuing delay")
plt.savefig('q1')
        
