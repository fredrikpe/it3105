import random
import time
import numpy

tlist = list(range(10000000))
random.shuffle(tlist)


s = time.time()
index_min, _ = min(enumerate(tlist), key=lambda p:p[1])
print(index_min)
print(time.time() - s)



s = time.time()
index_min = min(range(len(tlist)), key=tlist.__getitem__)
print(index_min)
print(time.time() - s)


s = time.time()
index_min = tlist.index(min(tlist))
print(index_min)
print(time.time() - s)


s = time.time()
index_min = numpy.argmin(tlist)
print(index_min)
print(time.time() - s)




