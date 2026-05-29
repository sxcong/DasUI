import sys
import numpy as np
import matplotlib.pyplot as plt


def read_matrix(path):
    with open(path, "rb") as f:
        rows = int(np.fromfile(f, dtype=np.int32, count=1)[0])
        cols = int(np.fromfile(f, dtype=np.int32, count=1)[0])
        data = np.fromfile(f, dtype=np.float64, count=rows * cols).reshape(rows, cols)
    return data


def main():
    if len(sys.argv) < 3:
        print("usage: python plot_spectrum.py spectrum.bin band_energy.bin [sample_rate] [fft_size]")
        return 1

    spectrum = read_matrix(sys.argv[1])
    band_energy = read_matrix(sys.argv[2])
    sample_rate = float(sys.argv[3]) if len(sys.argv) > 3 else 2000.0
    fft_size = int(sys.argv[4]) if len(sys.argv) > 4 else (spectrum.shape[1] - 1) * 2

    freq = np.arange(spectrum.shape[1]) * sample_rate / fft_size
    bands = ["0-1Hz", "0-10Hz", "10-100Hz", "100-500Hz", "500-1000Hz"]
    depth = np.arange(band_energy.shape[0])

    fig, axes = plt.subplots(1, 2, figsize=(14, 6))

    for i in range(min(band_energy.shape[1], len(bands))):
        axes[0].plot(band_energy[:, i], depth, label=bands[i])
    axes[0].invert_yaxis()
    axes[0].set_xlabel("Energy")
    axes[0].set_ylabel("Depth index")
    axes[0].set_title("Band Energy")
    axes[0].grid(True, alpha=0.35)
    axes[0].legend()

    image = axes[1].imshow(
        spectrum,
        aspect="auto",
        origin="upper",
        extent=[freq[0], freq[-1], spectrum.shape[0] - 1, 0],
        cmap="jet",
    )
    axes[1].set_xlabel("Frequency (Hz)")
    axes[1].set_ylabel("Depth index")
    axes[1].set_title("DAS Spectrum")
    fig.colorbar(image, ax=axes[1], label="Power")

    plt.tight_layout()
    plt.show()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
