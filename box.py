import argparse
import numpy as np
import matplotlib.pyplot as plt
import csv

# Get input file from command line
parser = argparse.ArgumentParser()
parser.add_argument('-i','--input')
infile = vars(parser.parse_args())['input']

# Read in csv
data = []
with open(infile, 'rb') as csvfile:
    reader = csv.reader(csvfile)
    for row in reader:
        for n in row:
            data.append(float(n))

# Split data into quartiles
data = np.array(data)
quartiles = np.percentile(data, np.arange(0, 100, 25))
iqr = quartiles[3] - quartiles[1]

# Remove outliers (1.5 above IQR)

blah = []
for d in data:
    if d < 3*iqr + quartiles[3]:
        blah.append(d)
data = blah

# Plot in a Box-Whisker plot!
plt.boxplot(data, notch=0, sym='+', vert=1, whis=1.5)
plt.show()
