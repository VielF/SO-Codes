from random import seed
from random import randint

seed(1)
f = open('data_memory.txt', 'w')
for i in range(1000000):
   number =  randint(0, 100)
   f.write(str(number)+'\n')

f.close()