import matplotlib.pyplot as plt
import pandas as pd


# ==================================================
# Plotting Performance Metrics for Parallel Vector Dot Product
# Course: Parallel Computing
# ==================================================

import matplotlib.pyplot as plt
import pandas as pd

# Read data from CSV file
df = pd.read_csv('vector_dot_product.csv')
# df = pd.read_csv('integral_results.csv')

# Create figure with 3 subplots (for slide)
fig, axes = plt.subplots(1, 3, figsize=(18, 4))

# --- Plot 1: Speedup vs Vector Size ---
ax1 = axes[0]
for t in [1, 2, 4]:
    subset = df[df['Threads'] == t].sort_values('Size')
    ax1.plot(subset['Size'], subset['Speedup'], marker='o', label=f'{t} Threads')
ax1.set_title('Speedup vs Vector Size')
ax1.set_xlabel('Vector Size (n)')
ax1.set_ylabel('Speedup Factor')
ax1.set_xscale('log')
ax1.grid(True, which="both", ls="-", alpha=0.5)
ax1.legend()

# --- Plot 2: Execution Time vs Vector Size ---
ax2 = axes[1]
for t in [1, 2, 4]:
    subset = df[df['Threads'] == t].sort_values('Size')
    label = 'Serial' if t == 1 else f'{t} Threads'
    ax2.plot(subset['Size'], subset['Time'], marker='s', label=label)
ax2.set_title('Execution Time vs Vector Size')
ax2.set_xlabel('Vector Size (n)')
ax2.set_ylabel('Time (seconds)')
ax2.set_xscale('log')
ax2.set_yscale('log')
ax2.grid(True, which="both", ls="-", alpha=0.5)
ax2.legend()

# --- Plot 3: Efficiency vs Vector Size ---
ax3 = axes[2]
for t in [1, 2, 4]:
    subset = df[df['Threads'] == t].sort_values('Size')
    ax3.plot(subset['Size'], subset['Efficiency'], marker='^', label=f'{t} Threads')
ax3.set_title('Efficiency vs Vector Size')
ax3.set_xlabel('Vector Size (n)')
ax3.set_ylabel('Efficiency (%)')
ax3.set_xscale('log')
ax3.set_ylim(0, 110)
ax3.grid(True, which="both", ls="-", alpha=0.5)
ax3.legend()

plt.tight_layout()
plt.savefig('vector_dot_product.png', dpi=300, bbox_inches='tight')
# plt.savefig('integral_plots.png', dpi=300, bbox_inches='tight')
plt.show()