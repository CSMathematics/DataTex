#-------------------------------------------------
#
# Project created by QtCreator 2021-06-25T16:28:39
#
#-------------------------------------------------

QT       += core gui widgets sql pdf
                                \pdfwidgets
QT += gui-private widgets-private xml printsupport concurrent network qml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataTex
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        datatex.cpp

HEADERS += \
        datatex.h

FORMS += \
        datatex.ui

include(datatex/datatex.pri)
include(quazip/quazip.pri)
include(custom_widgets/customwidgets.pri)
include(pdfviewer/qpdfviewer.pri)
include(qtsingleapplication/qtsingleapplication.pri)
#include(SingleApplication/singleapplication.pri)
#include(pdfviewer/pdfviewer.pri)
#include(pdfium/pdfium.pri)

TRANSLATIONS += DataTex_el_GR.ts \

RESOURCES += \
    images.qrc \
    languages.qrc \
    themes/darkstyle/darkstyle.qrc \
    themes/light/lightstyle.qrc \
    themes/linux_breath/linux_breath.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: tar
get.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#INCLUDEPATH  += ./pdfviewer/include_win32_qt5

#INCLUDEPATH += \
#    pdfium/ \
#    pdfium \
#    pdfium/third_party/ \
#    pdfium/third_party/freetype/include \
#    pdfium/third_party/freetype/include/freetype \
#    pdfium/fpdfsdk \
#    pdfium/third_party/zlib_v128

#DEFINES += \
#    OPJ_STATIC \
#    PNG_PREFIX \
#    PNG_USE_READ_MACROS \
#    V8_DEPRECATION_WARNINGS \
#    NOMINMAX \
#    FT2_BUILD_LIBRARY
