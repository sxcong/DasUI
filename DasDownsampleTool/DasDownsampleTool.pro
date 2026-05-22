QT -= gui
QT += core

CONFIG += c++17 console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = DasDownsampleTool

SOURCES += \
    src/application/batch_file_processor.cpp \
    src/application/das_downsample_service.cpp \
    src/application/fk_service.cpp \
    src/application/lfdas_service.cpp \
    src/infrastructure/hdf5_das_reader.cpp \
    src/infrastructure/binary_das_writer.cpp \
    src/infrastructure/binary_das_reader.cpp \
    src/main.cpp

HEADERS += \
    src/application/batch_file_processor.h \
    src/application/das_downsample_service.h \
    src/application/fk_service.h \
    src/core/das_matrix.h \
    src/core/downsample_config.h \
    src/core/hdf5_path_utils.h \
    src/core/idataset_reader.h \
    src/core/idataset_writer.h \
    src/infrastructure/hdf5_das_reader.h \
    src/infrastructure/hdf5_das_writer.h \
    src/infrastructure/binary_das_reader.h \
    src/application/lfdas_service.h

INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/../include/
INCLUDEPATH += $$PWD/../include/hdf5

CONFIG(release, debug|release) {
    DESTDIR = $$PWD/../bin/release
    LIBS += -L$$PWD/../lib/release -lhdf5 -lhdf5_cpp -lhdf5_hl -lhdf5_hl_cpp -lfftw3
}

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../bin/debug
    LIBS += -L$$PWD/../lib/debug -lhdf5 -lhdf5_cpp -lhdf5_hl -lhdf5_hl_cpp -lfftw3
}
