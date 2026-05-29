QT -= gui
QT += core

QMAKE_PROJECT_DEPTH = 0

CONFIG += c++17 console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = DasDownsampleTool

SOURCES += \
    defines.cpp \
    hdfspectrumprocessor.cpp \
    iir/Biquad.cpp \
    iir/Butterworth.cpp \
    iir/Cascade.cpp \
    iir/ChebyshevI.cpp \
    iir/ChebyshevII.cpp \
    iir/Custom.cpp \
    iir/PoleFilter.cpp \
    iir/RBJ.cpp \
    processdownsample.cpp \
    processlfdas.cpp \
    src/main.cpp

HEADERS += \
    defines.h \
    hdfspectrumprocessor.h \
    iir/Biquad.h \
    iir/Butterworth.h \
    iir/Cascade.h \
    iir/ChebyshevI.h \
    iir/ChebyshevII.h \
    iir/Common.h \
    iir/Custom.h \
    iir/Layout.h \
    iir/MathSupplement.h \
    iir/PoleFilter.h \
    iir/RBJ.h \
    iir/State.h \
    iir/Types.h \
    processdownsample.h \
    processlfdas.h

INCLUDEPATH += $$PWD/../include/
INCLUDEPATH += $$PWD/../include/hdf5

CONFIG(release, debug|release) {
    DESTDIR = $$PWD/../bin/release
    LIBS += -L$$PWD/../lib/release -lhdf5 -lhdf5_cpp -lhdf5_hl -lhdf5_hl_cpp -lfftw3 -lfftw3f
}

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../bin/debug
    LIBS += -L$$PWD/../lib/debug -lhdf5 -lhdf5_cpp -lhdf5_hl -lhdf5_hl_cpp -lfftw3 -lfftw3f
}
