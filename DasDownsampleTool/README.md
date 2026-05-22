# DasDownsampleTool

This is a small C++/Qt console project for DAS HDF5 processing.

## What it does

- Reads a 2D DAS dataset from HDF5
- Downsamples by row and column step
- Computes FBE in the time direction for 10-100 Hz by default
- Computes F-K spectrum as a 2D FFT magnitude map
- Applies LF-DAS steps 3-4 on binary input
- Writes a binary file whose first two `int32` values are rows and cols

## Build

Open `DasDownsampleTool.pro` with Qt qmake and build it as a console app.

## Example

```bash
DasDownsampleTool --input input.h5 --dataset /das/raw --output output.bin --row-step 10 --col-step 1
```

FBE:

```bash
DasDownsampleTool --mode fbe --input input.h5 --dataset /default --output fbe.bin --fbe-low 10 --fbe-high 100 --fbe-window 2000 --fbe-fft 1024
```

## Python read + plot

```python
import numpy as np
import matplotlib.pyplot as plt

with open("output.bin", "rb") as f:
    rows = int(np.fromfile(f, dtype=np.int32, count=1)[0])
    cols = int(np.fromfile(f, dtype=np.int32, count=1)[0])
    data = np.fromfile(f, dtype=np.float64, count=rows * cols).reshape(rows, cols)

plt.imshow(data, aspect="auto", cmap="viridis")
plt.colorbar()
plt.title("Downsampled DAS")
plt.show()
```

For FBE, the same binary format is used. The matrix shape is:

- rows = time windows
- cols = depth channels, for example 4001

LF-DAS:

```bash
DasDownsampleTool --mode lfdas --input downsample.bin --output lfdas.bin --median-window 5 --lowpass 0.05
```

F-K:

```bash
DasDownsampleTool --mode fk --input input.h5 --dataset /default --output fk.bin
```

First second and depth 100-200:

```bash
DasDownsampleTool --mode fk --input input.h5 --dataset /default --output fk.bin --fk-time-start 0 --fk-time-count 2000 --fk-depth-start 100 --fk-depth-count 101
```

Useful FK options:

- `--fk-sr` time sample rate
- `--fk-dz` depth spacing
- `--fk-vmin` minimum velocity
- `--fk-vmax` maximum velocity
- `--fk-sr` and `--fk-dz` are used for velocity-domain masking
