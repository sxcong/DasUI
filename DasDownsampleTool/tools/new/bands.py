import numpy as np
import matplotlib.pyplot as plt

def load_binary_matrix(filepath):
    with open(filepath, 'rb') as f:
        # 读取头部的两个 int (rows, cols)
        header = np.fromfile(f, dtype=np.int32, count=2)
        rows, cols = header[0], header[1]
        
        # 读取后续的所有 double 数据
        data = np.fromfile(f, dtype=np.float64)
        
        # 将其 reshape 回原来的维度
        # 注意：C++ 中你是一行一行存的，numpy 默认 reshape 也是按行优先，完全匹配
        return data.reshape((rows, cols))

# 使用示例
matrix = load_binary_matrix("D:/output/5/band_energy.merge.bin")


depths = np.arange(matrix.shape[0])  # 深度索引 0-4090
freq_labels = ["0-1Hz", "0-10Hz", "10-100Hz", "100-500Hz", "500-1000Hz"]

plt.figure(figsize=(6, 8))

# 遍历 5 列进行绘图
for i in range(matrix.shape[1]):
    plt.plot(matrix[:, i], depths, label=freq_labels[i])

# 翻转 Y 轴，让 0 在上，4090 在下
plt.gca().invert_yaxis()

plt.xlabel("Energy")
plt.ylabel("Depth (m)")
plt.legend()
plt.grid(True, linestyle='--')
plt.show()
