INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

INCLUDEPATH  += /home/spyros/qq27-poppler/include_win32_qt5/
LIBS         += -L/usr/lib -lpoppler-qt5

HEADERS += \
    #$$PWD/pdfviewer.h \
    #$$PWD/pageselector.h \
    #$$PWD/zoomselector.h
    $$PWD/documentwidget.h \
    $$PWD/pdfviewer.h

SOURCES += \
    #$$PWD/pdfviewer.cpp \
    #$$PWD/pageselector.cpp \
    #$$PWD/zoomselector.cpp
    $$PWD/documentwidget.cpp \
    $$PWD/pdfviewer.cpp

FORMS += \
    #$$PWD/pdfviewer.ui
    $$PWD/pdfviewer.ui
