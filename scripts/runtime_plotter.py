import matplotlib.pyplot as plt
from matplotlib.animation import PillowWriter

import pandas as pd
import numpy as np
import sys
import os

# Read csv data from stdin
df = pd.read_csv(sys.stdin)
fig = plt.figure()
print(df)

# Create a line object for each algorithm with a label
lines = {}
for algo in df['algorithm'].unique():
    line, = plt.plot([], [], linestyle='-', color='tab:blue', label=algo)
    lines[algo] = line

# Axis label, title, legend, and grid
plt.xlabel("Input Size (n)")
plt.ylabel("CPU Time (seconds)")
plt.title("Algorithm Runtime (seconds) vs Input Size (n)")
plt.legend(loc='upper left')
plt.grid(color='gray', linestyle='--', linewidth=0.5)

metadata = dict(title = "AlgoRuntime")
writer = PillowWriter(fps = 25, metadata=metadata)



# #! FOR SINGLE FRAME
# # Plot each algorithm
# for algo, lines in lines.items():
#     # Get subsets for each algorithm
#     subset = df[df['algorithm'] == algo]
    
#     # Set x, y limits and lists
#     plt.xlim(min(subset['input_size']), max(subset['input_size']))
#     plt.ylim(min(subset['t1']), max(subset['t1']))
#     x_list = []
#     y_list = []

#     # Generate runtime vs n plot animation
#     output_path = os.path.join("output", f"{algo}.gif")
#     with writer.saving(fig, output_path, 100):
#         for x, y in zip(subset['input_size'], subset['t1']):
#             x_list.append(x)
#             y_list.append(y)

#             line.set_data(x_list, y_list)
#             writer.grab_frame()



#! FOR LINEAR REG FRAMES
# Plot each algorithm
for algo, lines in lines.items():
    # Get subsets for each algorithm
    subset = df[df['algorithm'] == algo]
    
    # time_cols = ['t0', 't1', ..., 'tm']
    time_cols = [c for c in subset.columns if c.startswith('t')]

    # Set x, y limits and lists
    flat = subset[time_cols].values.flatten()
    plt.xlim(1, len(df))
    plt.ylim(np.percentile(flat, 0), np.percentile(flat, 97.5))
    
    x_list = subset['input_size']

    # Generate runtime vs n plot animation
    output_path = os.path.join("output", f"{algo}.gif")
    with writer.saving(fig, output_path, 100):
        for n_cols in range(1, len(time_cols)+1):
            # Determing running average for current time
            y_list = []
            for i in range(len(subset)):
                y_list.append(subset[time_cols[:n_cols]].iloc[i-2:i+3].mean(axis=1).mean())
            
            line.set_data(x_list, y_list)
            writer.grab_frame()



# Save static runtime vs n plot
png_path = os.path.join("output", f"{algo}.png")
plt.savefig(png_path, dpi=300)