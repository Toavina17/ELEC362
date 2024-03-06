QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport # Choosing modules (group of classes) needed for the app

CONFIG += c++11 # Setting the standard of C++

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    datainputdialog.cpp \
    dataset.cpp \
    datasetwindow.cpp \
    graphwindow.cpp \
    helpdialog.cpp \
    histogramclass.cpp \
    main.cpp \
    parentwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    aboutdialog.h \
    atmsp.h \
    datainputdialog.h \
    dataset.h \
    datasetwindow.h \
    documentinfo.h \
    graphwindow.h \
    helpdialog.h \
    histogramclass.h \
    parentwindow.h \
    qcustomplot.h

FORMS += \
    aboutdialog.ui \
    datainputdialog.ui \
    datasetwindow.ui \
    graphwindow.ui \
    helpdialog.ui \
    histogramclass.ui \
    parentwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/GSLlib/ -lgsl

INCLUDEPATH += $$PWD/GSLinclude
DEPENDPATH += $$PWD/GSLlib

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/GSLlib/gsl.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/GSLlib/libgsl.a



win32: LIBS += -L$$PWD/GSLlib/ -lgslcblas

INCLUDEPATH += $$PWD/GSLinclude
DEPENDPATH += $$PWD/GSLlib

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/GSLlib/gslcblas.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/GSLlib/libgslcblas.a

RESOURCES += \
    resources.qrc
