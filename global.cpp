#include "global.h"
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QElapsedTimer>
#include <fstream>

Global::Global() {}

bool createFullFilePath(const QString &fullPath) {
    QFileInfo fileInfo(fullPath);
    QDir dir = fileInfo.dir();

    // 1. 检查并创建所有父级目录
    // mkpath 会递归创建不存在的目录（如 /a/b/c），如果目录已存在则直接返回 true
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "无法创建目录路径:" << dir.path();
            return false;
        }
    }
    return true;
}
/*
// 配置常量
const double SAMPLE_RATE = 2000.0;
const int TOTAL_ROWS = 4001;

Eigen::MatrixXf readTimeSlice(const std::string& filename,
                              const std::string& dataset_name,
                              double start_sec,
                              double duration_sec) {

    // 1. 计算 hyperslab 参数
    hsize_t col_start = static_cast<hsize_t>(start_sec * SAMPLE_RATE);
    hsize_t col_count = static_cast<hsize_t>(duration_sec * SAMPLE_RATE);

    // 定义数组 (注意维度顺序: [Row, Col])
    hsize_t start[2]  = {0, col_start};          // 从第 0 行，指定列开始
    hsize_t count[2]  = {TOTAL_ROWS, col_count}; // 读取所有行，指定数量的列
    // stride 和 block 使用默认 (nullptr)，表示连续读取

    try {
        // 2. 打开文件和数据集
        H5File file(filename, H5F_ACC_RDONLY);
        DataSet dataset = file.openDataSet(dataset_name);

        // 3. 获取数据空间 (DataSpace)
        DataSpace filespace = dataset.getSpace();

        // 4. 执行 selectHyperslab
        // 注意参数顺序: op, count, start, stride, block
        filespace.selectHyperslab(H5S_SELECT_SET, count, start);

        // 5. 创建内存空间 (Memory Space)
        // 必须与 count 定义的尺寸完全一致
        hsize_t mem_dims[2] = {TOTAL_ROWS, col_count};
        DataSpace memspace(2, mem_dims);

        // 6. 准备缓冲区
        // 总元素个数 = 行 * 列
        size_t total_elements = static_cast<size_t>(TOTAL_ROWS) * static_cast<size_t>(col_count);
        std::vector<float> buffer(total_elements);

        // 7. 读取数据

        dataset.read(buffer.data(), PredType::NATIVE_DOUBLE, memspace, filespace);

        // 8. 转换到 Eigen 矩阵 (处理 Row-Major 到 Col-Major 的布局差异)
        Eigen::MatrixXf result(TOTAL_ROWS, col_count);

        // HDF5 读取的是行优先 (Row-Major): [R0C0, R0C1..., R1C0...]
        // Eigen 默认是列优先 (Col-Major)
        for (int r = 0; r < TOTAL_ROWS; ++r) {
            size_t offset = r * col_count;
            // 将 buffer 的一段映射为行向量并赋值
            result.row(r) = Eigen::Map<Eigen::RowVectorXf>(buffer.data() + offset, col_count);
        }

        qDebug() << "成功读取 Hyperslab: 时间 [" << start_sec << ", "
                  << (start_sec + duration_sec) << "]s, 尺寸: "
                  << result.rows() << "x" << result.cols() ;

        return result;

    } catch (FileIException& error) {
        error.printErrorStack();
        throw;
    } catch (DataSetIException& error) {
        error.printErrorStack();
        throw;
    } catch (DataSpaceIException& error) {
        error.printErrorStack();
        throw;
    }
}

int readAllH5File(const std::string& h5_file_path,
                             const std::string& dataset_name,
                             Eigen::MatrixXf& eigen_mat)
{
    try {

        QElapsedTimer timer;
        timer.start();

        FileAccPropList fapl;
        // 参数说明：
        // 1. mdc_nelmts: 元数据缓存节点数，通常设为 0 使用默认值。
        // 2. rdcc_nelmts: 缓存槽数量。应该是一个质数，通常建议是最大分块数的 10-100 倍。
        // 3. rdcc_nbytes: 缓存总大小（字节）。建议设为比一个完整数据集略大，或者至少 256MB-512MB。
        // 4. rdcc_w0: 淘汰策略（0-1）。0 表示只淘汰已完全读取的块，0.5-1 适合随机读取。顺序读建议设为 0。

        int mdc_nelmts = 0;
        size_t rdcc_nelmts = 10007;          // 质数，增加哈希效率
        size_t rdcc_nbytes = 512 * 1024 * 1024; // 512MB 缓存
        double rdcc_w0 = 0.0;                // 顺序读取模式优化

        fapl.setCache(mdc_nelmts, rdcc_nelmts, rdcc_nbytes, rdcc_w0);
        // 1. 打开文件和数据集
        H5File file(h5_file_path, H5F_ACC_RDONLY, fapl);
        DataSet dataset = file.openDataSet("/default");

        // 2. 获取数据集维度
        DataSpace dataspace = dataset.getSpace();
        hsize_t dims[2];
        dataspace.getSimpleExtentDims(dims, NULL);
        int rows = static_cast<int>(dims[0]); // 120000
        int cols = static_cast<int>(dims[1]); // 4001

        // 3. 定义【行优先】的 Eigen 矩阵 (必须匹配 HDF5 的存储方式)
        // 这样读入的数据指针顺序与文件完全一致，速度最快
        typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> RowMajorMatrixXf;
        RowMajorMatrixXf mat(rows, cols);

        // 4. 读取数据
        // PredType::NATIVE_FLOAT 对应文件中的 H5T_IEEE_F32LE
        dataset.read(mat.data(), PredType::NATIVE_FLOAT);

        // 5. 如果你后续需要使用 Eigen 默认的列优先矩阵，直接赋值即可

        eigen_mat = mat.transpose(); //转置

        //只看前3382通道
        int startInvalidChannel = 3382;
        int numInvalid = eigen_mat.rows() - startInvalidChannel - 1; // 4001 - 3382 = 619
        qDebug()<<"startInvalidChannel numInvalid"<<startInvalidChannel<<numInvalid;

        if (numInvalid > 0) {
            // 将从 3852 行开始的所有行清零
            eigen_mat.middleRows(startInvalidChannel, numInvalid).setZero();
        }

        qDebug()<<"read h5 time = "<<timer.elapsed() << "ms";

    } catch (FileIException& error) {
        error.printErrorStack();
        qDebug()<<"FileIException"<<error.getDetailMsg().c_str();
    } catch (DataSetIException& error) {
        error.printErrorStack();
        qDebug()<<"DataSetIException"<<error.getDetailMsg().c_str();
    }
    return 0;
}


//如果保存成.npy文件，使用Python可以直接读
void saveToNpy(const std::string& filename, const Eigen::MatrixXf& matrix) {
    // 1. 准备描述字典
    // 注意：Eigen 默认是 ColMajor (列优先)，所以 shape 设为 (cols, rows) 再在 Python 转置，
    // 或者直接存为 (rows, cols) 但确保数据是行优先。
    // 这里我们假设 matrix 是 RowMajor（行优先），更符合 Python 习惯。
    std::string dict = "{'descr': '<f4', 'fortran_order': False, 'shape': ("
                       + std::to_string(matrix.rows()) + ", "
                       + std::to_string(matrix.cols()) + "), }";

    // 2. 计算填充，使头部对齐 64 字节
    // 10 字节是固定前缀 (8字节魔数 + 2字节长度位)
    int remainder = 16 - ((10 + dict.size()) % 16);
    dict.append(remainder, ' ');
    dict.back() = '\n'; // 最后一位换行

    std::ofstream os(filename, std::ios::binary);

    // 3. 写入魔数与版本
    os.write("\x93NUMPY\x01\x00", 8);

    // 4. 写入头部长度 (小端)
    unsigned short header_len = static_cast<unsigned short>(dict.size());
    os.write(reinterpret_cast<char*>(&header_len), 2);

    // 5. 写入描述字典
    os.write(dict.c_str(), dict.size());

    // 6. 写入矩阵数据
    // 注意：如果是 Eigen::MatrixXf (默认 ColMajor)，存入后 Python 读出来是转置的。
    // 建议在 process 结果时就定义为 Eigen::Matrix<float, ..., Eigen::RowMajor>
    os.write(reinterpret_cast<const char*>(matrix.data()), matrix.size() * sizeof(float));

    os.close();
}


RowMatrixXf loadFromNpy(const std::string& filename) {
    std::ifstream is(filename, std::ios::binary);
    if (!is) {
        throw std::runtime_error("Cannot open file");
    }

    // 1. 校验魔数 (前 6 字节应该是 \x93NUMPY)
    char magic[6];
    is.read(magic, 6);
    if (std::string(magic, 6) != "\x93NUMPY") {
        throw std::runtime_error("Invalid .npy file");
    }

    // 2. 跳过版本号 (2 字节)
    is.seekg(2, std::ios::cur);

    // 3. 读取头部长度 (小端序, 2 字节)
    unsigned short header_len;
    is.read(reinterpret_cast<char*>(&header_len), 2);

    // 4. 读取并解析描述字典 (获取 shape)
    std::string header(header_len, ' ');
    is.read(&header[0], header_len);

    // 极简解析 shape: 寻找 "(rows, cols)" 格式
    // 注意：实际应用中建议使用更严谨的字符串处理，这里针对 (4001, 60) 这种格式
    size_t shape_pos = header.find("'shape': (");
    size_t comma_pos = header.find(',', shape_pos);
    size_t end_pos = header.find(')', comma_pos);

    int rows = std::stoi(header.substr(shape_pos + 10, comma_pos - (shape_pos + 10)));
    int cols = std::stoi(header.substr(comma_pos + 1, end_pos - (comma_pos + 1)));

    // 5. 读取原始数据
    RowMatrixXf matrix(rows, cols);
    is.read(reinterpret_cast<char*>(matrix.data()), rows * cols * sizeof(float));

    is.close();
    return matrix;
}
*/
