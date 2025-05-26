import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('steering_only.csv')

# Basic stats
print("Summary:\n", df.describe())

# Plot time series
plt.figure(figsize=(10, 6))
plt.plot(df['steering'], label='Steering')
plt.plot(df['throttle'], label='Throttle')
plt.plot(df['engage'], label='Engage')
plt.legend()
plt.title("PWM Signal High Times")
plt.xlabel("Sample")
plt.ylabel("High Time (µs)")
plt.grid(True)
plt.show()

# Plot histograms in the same window but as separate subplots
fig, axes = plt.subplots(3, 1, figsize=(10, 8), sharex=True)

channels = ['steering', 'throttle', 'engage']
for i, ch in enumerate(channels):
    axes[i].hist(df[ch], bins=30, alpha=0.7, color='C'+str(i))
    axes[i].set_title(f"{ch.capitalize()} Pulse Width Distribution")
    axes[i].set_ylabel("Count")
    axes[i].grid(True)

# plt.figure(figsize=(8, 4))
# plt.hist(df['steering'], bins=30, alpha=0.7, color='blue')
# plt.title("Steering Pulse Width Distribution")
# plt.xlabel("High Time (µs)")
# plt.ylabel("Count")
# plt.grid(True)
# plt.show()

axes[-1].set_xlabel("High Time (µs)")
plt.tight_layout()
plt.show()
