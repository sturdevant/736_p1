import argparse
import numpy as np
import matplotlib.pyplot as plt
import csv

# Get input file from command line
parser = argparse.ArgumentParser()
parser.add_argument('-i','--input')
infile = vars(parser.parse_args())['input']

data = []
# Read in csv
with open(infile, 'rb') as csvfile:
    reader = csv.reader(csvfile)
    for row in reader:
        for n in row:
            if int(n) < 60:
                data.append(int(n))

print "Count of 48's: ", data.count(48)
print "Count of 52's: ", data.count(52)
print "Count of 32's: ", data.count(32)
print "Count of 36's: ", data.count(36)

# Rest of stuff
n, bins, patches = plt.hist(data, 60)
plt.show()

