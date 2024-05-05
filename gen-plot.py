#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
import os
from multiprocessing import Pool

def generate_plot(args):
    i, row, output_dir = args
    plt.plot(range(len(row)), row)
    plt.xlabel('Element Index')
    plt.ylabel('Value')
    plt.title(f'Line Plot - Row {i}')
    plt.ylim(-2, 2)  # Set y-axis range to -2 to 2
    plt.xticks(range(len(row)))  # Set x-axis ticks to integer values
    plt.xlim(0, len(row)-1)  # Adjust x-axis limit
    plt.savefig(f'{output_dir}/plot-{i}.png')
    plt.clf()  # Clear the plot for the next iteration

if __name__ == '__main__':
    # Read CSV file
    df = pd.read_csv('out/result.dat')

    # Create directory if it doesn't exist
    output_dir = 'plot-export'
    os.makedirs(output_dir, exist_ok=True)

    # Prepare arguments for parallel processing
    args_list = [(i, row, output_dir) for i, row in enumerate(df.values)]

    # Parallelize the generation of plots
    with Pool() as pool:
        pool.map(generate_plot, args_list)

