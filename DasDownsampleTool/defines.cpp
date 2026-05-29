#include "defines.h"
#include "iir/Butterworth.h"

Defines::Defines() {}


bool saveBinary(const std::string& file, const Matrix& m)
{
    std::ofstream out(file, std::ios::binary);
    if(!out) return false;

    int r = m.rows();
    int c = m.cols();

    out.write(reinterpret_cast<const char*>(&r), sizeof(int));
    out.write(reinterpret_cast<const char*>(&c), sizeof(int));

    std::vector<double> data;
    data.reserve(static_cast<std::size_t>(r) * static_cast<std::size_t>(c));
    for(int i = 0; i < r; ++i)
    {
        for(int j = 0; j < c; ++j)
        {
            data.push_back(static_cast<double>(m(i, j)));
        }
    }

    out.write(reinterpret_cast<const char*>(data.data()),
              static_cast<std::streamsize>(sizeof(double) * data.size()));
    return static_cast<bool>(out);
}

bool loadBinary(const std::string& file, Matrix& m)
{
    std::ifstream in(file, std::ios::binary);
    if(!in) return false;

    int r = 0;
    int c = 0;
    in.read(reinterpret_cast<char*>(&r), sizeof(int));
    in.read(reinterpret_cast<char*>(&c), sizeof(int));

    if(!in || r < 0 || c < 0) return false;

    const std::size_t count = static_cast<std::size_t>(r) * static_cast<std::size_t>(c);
    std::vector<double> data(count);
    in.read(reinterpret_cast<char*>(data.data()),
            static_cast<std::streamsize>(sizeof(double) * data.size()));
    if(!in) return false;

    m.resize(r, c);
    for(int i = 0; i < r; ++i)
    {
        for(int j = 0; j < c; ++j)
        {
            m(i, j) = static_cast<float>(data[static_cast<std::size_t>(i) * c + j]);
        }
    }

    return true;
}

bool mergeSpectrumPowerToDb(const std::vector<std::string>& spectrumPowerFiles,
                            const std::string& outDbFile,
                            bool relativeDb,
                            float eps)
{
    if(spectrumPowerFiles.empty()) return false;

    Matrix sumPower;
    int validCount = 0;

    for(const std::string& file : spectrumPowerFiles)
    {
        Matrix power;
        if(!loadBinary(file, power)) {
            return false;
        }

        if(validCount == 0) {
            sumPower = power;
        } else {
            if(power.rows() != sumPower.rows() || power.cols() != sumPower.cols()) {
                return false;
            }
            sumPower += power;
        }

        ++validCount;
    }

    if(validCount <= 0) return false;

    Matrix avgPower = sumPower / static_cast<float>(validCount);
    Matrix db = (avgPower.array() + eps).log10().matrix() * 10.0f;

    if(relativeDb && db.size() > 0) {
        db.array() -= db.maxCoeff();
    }

    return saveBinary(outDbFile, db);
}

#include <QTextStream>

void printInfo(const QString& msg)
{
    QTextStream out(stdout);
    out << msg << Qt::endl;
}

void printError(const QString& msg)
{
    QTextStream err(stderr);
    err << msg << Qt::endl;
}

