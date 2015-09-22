import argparse
import numpy as np
import matplotlib.pyplot as plt
import csv

# Get input file from command line
parser = argparse.ArgumentParser()
parser.add_argument('-i','--input')
infile = vars(parser.parse_args())['input']

data = []
index = []
i = 0
# Read in csv
with open(infile, 'rb') as csvfile:
    reader = csv.reader(csvfile)
    for row in reader:
        for n in row:
            if float(n) < 200000:
                index.append(i)
                i += 1
                data.append(float(n))

# Rest of stuff
print min(data)
plt.plot(index, data)
plt.show()

