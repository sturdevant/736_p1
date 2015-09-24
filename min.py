import argparse
import numpy as np
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
            data.append(float(n))

# Rest of stuff
print min(data)

