QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_PROJECT_DEPTH = 0

SOURCES += \
    aboutdialog.cpp \
    advancedanalysiswidget.cpp \
    axisrange.cpp \
    chartselectwidget.cpp \
    configdialog.cpp \
    curveview.cpp \
    filelistwidget.cpp \
    filetreelistwidget.cpp \
    global.cpp \
    hd5reader.cpp \
    informationwgt.cpp \
    leftnavbar.cpp \
    main.cpp \
    mainwidget.cpp \
    mainwindow.cpp \
    pointtracer.cpp \
    projectdb.cpp \
    projectlistdialog.cpp \
    projectmgr.cpp \
    ratecurveview.cpp \
    reportwidget.cpp \
    statuswidget.cpp \
    systemsettingswidget.cpp \
    titlewdgt.cpp \
    usercenterwidget.cpp \
    waterfallwdgttransf.cpp \
    wellwidget.cpp

HEADERS += \
    aboutdialog.h \
    advancedanalysiswidget.h \
    axisrange.h \
    chartselectwidget.h \
    configdialog.h \
    curveview.h \
    defines.h \
    filelistwidget.h \
    filetreelistwidget.h \
    global.h \
    hd5reader.h \
    informationwgt.h \
    leftnavbar.h \
    mainwidget.h \
    mainwindow.h \
    pointtracer.h \
    projectdb.h \
    projectlistdialog.h \
    projectmgr.h \
    ratecurveview.h \
    reportwidget.h \
    statuswidget.h \
    systemsettingswidget.h \
    titlewdgt.h \
    usercenterwidget.h \
    waterfallwdgttransf.h \
    wellwidget.h

FORMS += \
    aboutdialog.ui \
    advancedanalysiswidget.ui \
    axisrange.ui \
    chartselectwidget.ui \
    configdialog.ui \
    curveview.ui \
    filelistwidget.ui \
    filetreelistwidget.ui \
    informationwgt.ui \
    leftnavbar.ui \
    mainwidget.ui \
    mainwindow.ui \
    projectlistdialog.ui \
    reportwidget.ui \
    statuswidget.ui \
    systemsettingswidget.ui \
    titlewdgt.ui \
    usercenterwidget.ui \
    wellwidget.ui

# Eigen
INCLUDEPATH += $$PWD/include/
# HDF5
INCLUDEPATH += $$PWD/include/hdf5
INCLUDEPATH += $$PWD/include/Eigen


# Default rules for deployment.
include($$PWD/qcustomplot/qcustomplot.pri)
include($$PWD/wellStructure/wellStructure.pri)

CONFIG(release, debug|release){
    TARGET = DasUI
    DESTDIR=$$PWD/bin/Release
    LIBS += -L$$PWD/lib/Release -lhdf5 -lhdf5_cpp
      #TRANSLATIONS += $$PWD/../Bin/Release/translations/das_qt_cn.ts
}

CONFIG(debug, debug|release){
    TARGET = DasUI_d
    DESTDIR=$$PWD/bin/Debug
    LIBS += -L$$PWD/lib/Debug  -lhdf5 -lhdf5_cpp
    #TRANSLATIONS += $$PWD/../Bin/Debug/translations/das_qt_cn.ts
}

# 开启 OpenMP 支持
# 针对 Microsoft Visual C++ (MSVC) 编译器
msvc {
    # /O2: 最大速度优化
    # /arch:AVX2: 启用 AVX2 指令集 (注意：VS2017/2019/2022 默认可能已开启，但显式声明更安全)
    # /openmp: 启用 OpenMP
    QMAKE_CXXFLAGS += /O2 /arch:AVX2 /openmp
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8

    # 如果是 Release 模式，确保不使用调试标志
    release
    {
        QMAKE_CXXFLAGS_RELEASE += /O2 /arch:AVX2 /openmp
        # 移除默认的 /Ob1 等可能限制优化的标志，强制 /O2
        QMAKE_CXXFLAGS_RELEASE -= /Ob1
    }
}

RESOURCES += \
    res.qrc \
    qss.qrc
DISTFILES += res/viewer.rc
