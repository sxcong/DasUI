#ifndef DAS_MATRIX_H
#define DAS_MATRIX_H

#include <cstddef>
#include <vector>

struct DasMatrix {
    std::size_t rows = 0;
    std::size_t cols = 0;
    std::vector<double> data;

    bool empty() const { return rows == 0 || cols == 0 || data.empty(); }
    double* ptr() { return data.data(); }
    const double* ptr() const { return data.data(); }
    double at(std::size_t r, std::size_t c) const { return data[r * cols + c]; }
};

#endif

