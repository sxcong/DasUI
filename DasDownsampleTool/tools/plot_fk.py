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


if len(sys.argv) < 2:
    print("usage: python plot_fk.py fkview.bin")
    raise SystemExit(1)

spec = load_spectrum(sys.argv[1])

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
