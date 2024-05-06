#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
import os
from multiprocessing import Pool

def generate_imshow(args):
    filename, input_dir, output_dir = args
    # Remove the '.csv' extension from the filename before saving
    output_filename = filename[:-5] + '.png'  # Strips the last 4 characters '.csv' and adds '.png'
    # Load CSV as a DataFrame
    df = pd.read_csv(os.path.join(input_dir, filename))
    # Convert DataFrame to a 2D array for imshow
    data = df.values
    plt.imshow(data, aspect='auto', cmap='viridis', vmin=-2, vmax=2)  # You can change the colormap 'viridis' to others like 'gray', etc.
    plt.colorbar()  # Add a color bar to the side
    plt.title(f'Heatmap - {output_filename}')
    plt.xlabel('X coordinate')
    plt.ylabel('Y coordinate')
    plt.savefig(f'{output_dir}/{output_filename}')
    plt.clf()  # Clear the figure after saving

if __name__ == '__main__':
    input_dir = 'out'  # Directory containing the CSV files
    output_dir = 'plot-export'

    # Ensure output directory exists
    os.makedirs(output_dir, exist_ok=True)

    # List all CSV files in the input directory
    csv_files = [f for f in os.listdir(input_dir) if f.endswith('.csv')]

    # Prepare arguments for parallel processing
    args_list = [(filename, input_dir, output_dir) for filename in csv_files]

    # Parallelize the generation of imshow plots
    with Pool() as pool:
        pool.map(generate_imshow, args_list)

