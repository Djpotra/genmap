import os
import sys
import numpy as np

data = []

dirname = os.getcwd()
fname = os.path.join(dirname, sys.argv[1])

with open(fname, "r") as f:
    for line in f:
        print(line)
        data = data + line.split()

data = np.array(data, dtype=np.int64)

f = open(fname + ".bin", "wb")
f.write(data.tobytes())
