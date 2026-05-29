import numpy as np
import matplotlib.pyplot as plt

def inspect_data(data):
    """
    data: 一个 numpy 数组 (例如 shape 为 (60, 4001))
    """
    print(f"--- 数据概况 ---")
    print(f"最小值 (Min): {np.min(data):.6f}")
    print(f"最大值 (Max): {np.max(data):.6f}")
    print(f"平均值 (Mean): {np.mean(data):.6f}")
    print(f"标准差 (Std): {np.std(data):.6f}")
    
    # 检查是否有 NaN 或无穷大 (这是滤波处理中常见的问题)
    print(f"包含 NaN: {np.isnan(data).any()}")
    print(f"包含 Inf: {np.isinf(data).any()}")

def load_spectrum(filepath):
    with open(filepath, 'rb') as f:

        header = np.fromfile(f, dtype=np.int32, count=2)
        rows, cols = header[0], header[1]
        print(rows, cols)

        data = np.fromfile(f, dtype=np.float64)

        inspect_data(data)

        return data.reshape((rows, cols))

# 显示
spec = load_spectrum("D:/output/4/h5_20251023-135006.042214963_333.h5.spectrum.down")

spec_plot = spec.T

#origin='lower',
im = plt.imshow(spec_plot, aspect='auto', cmap='jet',origin='upper',  vmin=-0.001, vmax=0.002)

cbar = plt.colorbar(im)
cbar.set_label('Energy (dB)')


plt.xlabel("Time Bin")
plt.ylabel("Depth Index")


plt.show()
