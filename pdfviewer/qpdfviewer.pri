INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD


HEADERS += \
    $$PWD/qpdfviewer.h \
    $$PWD/searchresultdelegate.h \
    $$PWD/zoomselector.h


SOURCES += \
    $$PWD/qpdfviewer.cpp \
    $$PWD/searchresultdelegate.cpp \
    $$PWD/zoomselector.cpp


FORMS += \
    $$PWD/qpdfviewer.ui

RESOURCES += \
    $$PWD/resources.qrc

DISTFILES += \
    $$PWD/doc/src/pdfviewer.qdoc \
    $$PWD/images/document-open.svgz \
    $$PWD/images/go-down-search.svgz \
    $$PWD/images/go-next-view-page.svgz \
    $$PWD/images/go-next-view.svgz \
    $$PWD/images/go-previous-view-page.svgz \
    $$PWD/images/go-previous-view.svgz \
    $$PWD/images/go-up-search.svgz \
    $$PWD/images/zoom-fit-best.svgz \
    $$PWD/images/zoom-fit-width.svgz \
    $$PWD/images/zoom-in.svgz \
    $$PWD/images/zoom-original.svgz \
    $$PWD/images/zoom-out.svgz \
    $$PWD/images/zoom-previous.svgz

