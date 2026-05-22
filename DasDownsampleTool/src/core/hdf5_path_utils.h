#ifndef HDF5_PATH_UTILS_H
#define HDF5_PATH_UTILS_H

#include <H5Cpp.h>
#include <string>
#include <vector>

namespace Hdf5PathUtils {

inline std::string normalizePath(const std::string& path) {
    if (path.empty()) return "/";
    if (path.front() == '/') return path;
    return "/" + path;
}

inline std::string parentPath(const std::string& path) {
    std::string p = normalizePath(path);
    auto pos = p.find_last_of('/');
    if (pos == std::string::npos || pos == 0) return "/";
    return p.substr(0, pos);
}

inline std::string leafName(const std::string& path) {
    std::string p = normalizePath(path);
    auto pos = p.find_last_of('/');
    if (pos == std::string::npos) return p;
    return p.substr(pos + 1);
}

inline bool ensureGroups(H5::H5File& file, const std::string& datasetPath) {
    std::string p = normalizePath(datasetPath);
    std::string current;
    std::size_t start = 1;
    while (true) {
        auto pos = p.find('/', start);
        std::string part = p.substr(start, pos == std::string::npos ? std::string::npos : pos - start);
        if (!part.empty()) {
            current += "/" + part;
            try {
                file.openGroup(current);
            } catch (...) {
                try {
                    file.createGroup(current);
                } catch (...) {
                    return false;
                }
            }
        }
        if (pos == std::string::npos) break;
        start = pos + 1;
    }
    return true;
}

inline std::string shapeToString(const std::vector<hsize_t>& shape) {
    std::string out;
    for (std::size_t i = 0; i < shape.size(); ++i) {
        if (i) out += "x";
        out += std::to_string(static_cast<unsigned long long>(shape[i]));
    }
    return out;
}

}

#endif

