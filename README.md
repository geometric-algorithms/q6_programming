# Quadtree-based WSPD and t-Spanner Construction

This project implements the construction of a **Well-Separated Pair Decomposition (WSPD)** and a **geometric t-Spanner** using a **quadtree** structure in C++.

## Requirements
- C++17 compatible compiler (e.g., `g++`)
- Python version **3.10**
- `matplotlibcpp` and `numpy` Python packages

> **Note:** If you are using a different Python version, please update the `PYTHON_VERSION` variable in the `Makefile` accordingly.

---

## How to Run

### 1. Prepare the input
- Ensure you have an `in.txt` file containing the input points.
- Input format:
  ```
  <separation_constant s>
  <number_of_points n>
  <x1> <y1>
  <x2> <y2>
  ...
  <xn> <yn>
  ```

Example `in.txt`:
```
2.0
5
0 0
1 1
2 2
3 3
4 4
```

### 2. Build and Run
```bash
make
./test.sh
```
This will:
- Compile the program using the `Makefile`.
- Run the compiled executable (`./quadtree`) with `in.txt` as input.
- Automatically clean up build files afterward.

### 3. Output
After running, you will find two generated images:
- `wspd.png` — Visualization of the Quadtree and Well-Separated Pairs.
- `spanner.png` — Visualization of the constructed t-Spanner.

These images give graphical insight into how the points were partitioned and connected.

---

## Notes
- The implementation uses Python 3.10 headers for `matplotlibcpp`. If your system uses a different Python version (e.g., 3.8 or 3.11), modify `PYTHON_VERSION` in the `Makefile`.
- Make sure Python packages `matplotlib` and `numpy` are installed in your environment.

---

## Clean Up
To manually remove all generated files and binaries:
```bash
make clean
```

---

## Authors
- Raja Gopal (210050160)
- Praneeth (210050094)

**Date:** April 20, 2025
