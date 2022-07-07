INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

#  INCLUDEPATH  += :/pdfviewer/include_win32_qt5/
#  LIBS         += -L/usr/lib -lpoppler-qt5

HEADERS += \
    $$PWD/qpdfview.h \
    $$PWD/qpdfview_p.h \
  $$PWD/qpdfviewer.h \
  $$PWD/pageselector.h \
  $$PWD/zoomselector.h \
#      $$PWD/documentwidget.h \
#      $$PWD/pdfviewer.h

SOURCES += \
    $$PWD/qpdfview.cpp \
  $$PWD/qpdfviewer.cpp \
  $$PWD/pageselector.cpp \
  $$PWD/zoomselector.cpp \
#      $$PWD/documentwidget.cpp \
#      $$PWD/pdfviewer.cpp

FORMS += \
  $$PWD/qpdfviewer.ui \
#      $$PWD/pdfviewer.ui
