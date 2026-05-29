import numpy as np
import matplotlib.pyplot as plt

# 假设你知道采样率 SR 和 FFT_SIZE
def show_spectrum(spec, fs, fft_size):
    plt.figure(figsize=(10, 6))
    # 频率轴：从 0 到 fs/2
    extent = [0, fs/2, spec.shape[0], 0] 
    im = plt.imshow(spec, aspect='auto', cmap='jet', vmin=-40, vmax=0, extent=extent)
    
    plt.colorbar(im, label='Energy (dB)')
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Depth Index")
    plt.title("LF-DAS Power Spectrum")
    plt.show()

def load_spectrum(filepath):
    with open(filepath, 'rb') as f:
        # 读取前两个 4 字节的 int32
        header = np.fromfile(f, dtype=np.int32, count=2)
        rows, cols = header[0], header[1]
        print(rows, cols)
        # 读取剩余的 double 数据
        data = np.fromfile(f, dtype=np.float64)
        # 重构为 2D 矩阵
        return data.reshape((rows, cols))

# 显示
spec = load_spectrum("D:/output/4/h5_20251023-135006.042214963_333.h5.spectrum.spectrum_db")

show_spectrum(spec,2000,1001)

