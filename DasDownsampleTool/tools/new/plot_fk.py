import sys
import numpy as np
import matplotlib.pyplot as plt

FS_HZ = 2000.0
N_TIME = 2000
F_LIMIT_HZ = 400.0
DX_M = 1.0



def load_spectrum(filepath):
    with open(filepath, "rb") as f:
        header = np.fromfile(f, dtype=np.int32, count=2)
        rows, cols = header[0], header[1]
        print(rows, cols)

        data = np.fromfile(f, dtype=np.float64)
        return data.reshape((rows, cols))



spec = load_spectrum("D:/output/4/h5_20251023-135006.042214963_333.h5.spectrum.fk2")


# FKProcessor saves rows as shifted frequency bins and columns as shifted
# wavenumber bins, matching imshow's y/x layout directly.
view = spec
freq_limit_hz = F_LIMIT_HZ
k_limit = 0.5 / DX_M
vmax = view.max()

im = plt.imshow(
    view,
    aspect="auto",
    cmap="jet",
    origin="upper",
    extent=[-k_limit, k_limit, -freq_limit_hz, freq_limit_hz],
    vmin=0,
    vmax=vmax,
)

cbar = plt.colorbar(im)
cbar.set_label("Energy (dB)")

plt.xlabel("Wavenumber (cycles/m)")
plt.ylabel("Frequency (Hz)")

plt.show()



# x=实际波数 K，y=正负频率并且 0 在中心。把 plot_fk.py 从 bin 轴改成物理坐标轴，默认 fs=2000、dx=1.0，并把 y 轴方向调成参考图那样：上方负频率、下方正频率。
