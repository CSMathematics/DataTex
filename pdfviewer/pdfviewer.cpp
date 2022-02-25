/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the documentation of Qt. It was originally
** published as part of Qt Quarterly.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
** Qt for Windows(R) Licensees
** As a special exception, Nokia, as the sole copyright holder for Qt
** Designer, grants users of the Qt/Eclipse Integration plug-in the
** right for the Qt/Eclipse Integration to link to functionality
** provided by Qt Designer and its related libraries.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include "pdfviewer.h"
#include "datatex.h"
#include <poppler-qt5.h>
#include <QtCore>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QPrinter>
#include <QtPrintSupport>

PdfViewer::PdfViewer(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    scaleFactors << 0.25 << 0.5 << 0.75 << 1. << 1.25 << 1.5 << 2. << 3. << 4.;
    scaleComboBox = new QComboBox(this);
    scaleComboBox->addItems({"25/%","50%","75%","100%","125%","150%","200%","300%","400%"});
    scaleComboBox->setCurrentIndex(3);
    menuBar()->setVisible(false);
    CurrentPageLine = new QLineEdit(this);
    CurrentPageLine->setMaximumWidth(50);
    CurrentPageLine->setAlignment(Qt::AlignCenter);
    documentWidget = new DocumentWidget();
    scrollArea->setWidget(documentWidget);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(scaleComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(scaleDocument(int)));

    firstPageAction = CreateNewAction(menu_File,firstPageAction,documentWidget,[=](){documentWidget->setPage(1);},QIcon::fromTheme("arrow-left-double"),"&First page");
    connect(firstPageAction,&QAction::triggered,this,[=](){CurrentPage();});
    toolBar->addAction(firstPageAction);

    previousPageAction = CreateNewAction(menu_File,previousPageAction,documentWidget,SLOT(previousPage()),QIcon::fromTheme("arrow-left"),"&Previous page");
    connect(previousPageAction,&QAction::triggered,this,[=](){CurrentPage();});
    toolBar->addAction(previousPageAction);

    toolBar->addWidget(CurrentPageLine);
    CurrentPageLine->setValidator( new QIntValidator(0, 100, this) );
    connect(CurrentPageLine,&QLineEdit::textChanged,this,[=](){setPageManually();});

    nextPageAction = CreateNewAction(menu_File,nextPageAction,documentWidget,SLOT(nextPage()),QIcon::fromTheme("arrow-right"),"&Next page");
    connect(nextPageAction,&QAction::triggered,this,[=](){CurrentPage();});
    toolBar->addAction(nextPageAction);

    lastPageAction = CreateNewAction(menu_File,lastPageAction,documentWidget,SLOT(nextPage()),QIcon::fromTheme("arrow-right-double"),"&Last page");
    connect(lastPageAction,&QAction::triggered,this,[=](){CurrentPage();});
    toolBar->addAction(lastPageAction);

    toolBar->addSeparator();
    zoomOutAction = CreateNewAction(menu_File,zoomOutAction,this,
                    [=](){if(scaleComboBox->currentIndex()>0){scaleComboBox->setCurrentIndex(scaleComboBox->currentIndex()-1);}},
                    QIcon::fromTheme("zoom-out"),"&Zoom out");
    toolBar->addAction(zoomOutAction);

    toolBar->addWidget(scaleComboBox);
    scaleComboBox->setEditable(true);

    zoomInAction = CreateNewAction(menu_File,zoomInAction,this,
                   [=](){if(scaleComboBox->currentIndex()<scaleFactors.count()-1){scaleComboBox->setCurrentIndex(scaleComboBox->currentIndex()+1);}},
                   QIcon::fromTheme("zoom-in"),"&Zoom in");
    toolBar->addAction(zoomInAction);

    toolBar->addSeparator();
    printDocument = CreateNewAction(menu_File,printDocument,this,SLOT(PrintCurrentDocument()),
                   QIcon::fromTheme("document-print"),"&Print this file");
    extPdfViewer = CreateNewAction(menu_File,extPdfViewer,this,[=](){
        QDesktopServices::openUrl(QUrl("file:///"+documentWidget->getDocument()));},
                   QIcon(":/images/pdf.svg"),"&Open in external pdf viewer");
    toolBar->addAction(printDocument);
    toolBar->addAction(extPdfViewer);

//    connect(fitToWidth, &QPushButton::clicked,this, [=](){
//        documentWidget->setScale(documentWidget->width()/2);
//    qDebug()<<documentWidget->width()<<physicalDpiX();
//    });

    selectedTextAction->setChecked(false);

//    const Poppler::Document::PageLayout pageLayout = documentWidget->document()->pageLayout();
//    qDebug()<<pageLayout;


//    const int x = scrollArea->viewport()->x();
//    const int y = scrollArea->viewport()->y();
//    const int width = scrollArea->viewport()->width();
//    const int height = scrollArea->viewport()->height();

//    const QRect currentPageLine(x, y + height * 0.4, width, 2);
//    int currentPage = 0;
//    for (auto it = documentWidget->document()->pageLayout().pageGeometries.cbegin();
//         it != documentWidget->document()->pageLayout().pageGeometries.cend(); ++it) {
//        const QRect pageGeometry = it.value();
//        if (pageGeometry.intersects(currentPageLine)) {
//            currentPage = it.key();
//            break;
//        }
//    }

//    if (currentPage != m_pageNavigation->currentPage()) {
//        m_blockPageScrolling = true;
//        m_pageNavigation->setCurrentPage(currentPage);
//        m_blockPageScrolling = false;
//    }

}

void PdfViewer::setCurrentDocument(QString pdfFile)
{
    documentWidget->setDocument(pdfFile);
    lastPath = pdfFile;
    scaleComboBox->setEnabled(true);
    CurrentPage();
}

QAction * PdfViewer::CreateNewAction(QMenu * Menu, QAction * Action, QWidget * widget, const char *Function, QIcon Icon, const char * Description)
{
    Action = new QAction(Icon,tr(Description), this);
    Menu->addAction(Action);
    connect(Action, SIGNAL(triggered()), widget, Function);
    return Action;
}

QAction * PdfViewer::CreateNewAction(QMenu * Menu, QAction * Action, QWidget * widget, std::function<void()> Function, QIcon Icon, const char * Description)
{
    Action = new QAction(Icon,tr(Description), this);
    Menu->addAction(Action);
    QObject::connect(Action, &QAction::triggered, widget, Function);
    return Action;
}

void PdfViewer::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open PDF File"),
        lastPath, tr("PDF files (*.pdf)"));

    if (path.isEmpty())
        return;

    if (documentWidget->setDocument(path)) {
        lastPath = path;
        scaleComboBox->setEnabled(true);
    } else
        QMessageBox::warning(this, tr("PDF Viewer - Failed to Open File"),
                             tr("The specified file could not be opened."));
}

void PdfViewer::scaleDocument(int index)
{
    documentWidget->setScale(scaleFactors[index]);
}

void PdfViewer::CurrentPage()
{
    CurrentPageLine->setText(QString::number(documentWidget->getPage()+1)+"/"+QString::number(documentWidget->document()->numPages()));
}

void PdfViewer::setPageManually()
{
    QStringList values = CurrentPageLine->text().split("/");
    documentWidget->setPage(values.at(0).toInt());
}

//DocumentLayout PdfViewer::calculateDocumentLayout() const
//{
//    // The DocumentLayout describes a virtual layout where all pages are positioned inside
//    //    - For SinglePage mode, this is just an area as large as the current page surrounded
//    //      by the m_documentMargins.
//    //    - For MultiPage mode, this is the area that is covered by all pages which are placed
//    //      below each other, with m_pageSpacing inbetween and surrounded by m_documentMargins

//    DocumentLayout documentLayout;

//    if (!m_document || m_document->status() != QPdfDocument::Ready)
//        return documentLayout;

//    QHash<int, QRect> pageGeometries;

//    const int pageCount = m_document->pageCount();

//    int totalWidth = 0;

//    const int startPage = (m_pageMode == QPdfView::SinglePage ? m_pageNavigation->currentPage() : 0);
//    const int endPage = (m_pageMode == QPdfView::SinglePage ? m_pageNavigation->currentPage() + 1 : pageCount);

//    // calculate page sizes
//    for (int page = 0; page < 41; ++page) {
//        QSize pageSize;
//        if (m_zoomMode == QPdfView::CustomZoom) {
//            pageSize = QSizeF(m_document->pageSize(page) * m_screenResolution * m_zoomFactor).toSize();
//        } else if (m_zoomMode == QPdfView::FitToWidth) {
//            pageSize = QSizeF(m_document->pageSize(page) * m_screenResolution).toSize();
//            const qreal factor = (qreal(m_viewport.width() - m_documentMargins.left() - m_documentMargins.right()) / qreal(pageSize.width()));
//            pageSize *= factor;
//        } else if (m_zoomMode == QPdfView::FitInView) {
//            const QSize viewportSize(m_viewport.size() + QSize(-m_documentMargins.left() - m_documentMargins.right(), -m_pageSpacing));

//            pageSize = QSizeF(m_document->pageSize(page) * m_screenResolution).toSize();
//            pageSize = pageSize.scaled(viewportSize, Qt::KeepAspectRatio);
//        }

//        totalWidth = qMax(totalWidth, pageSize.width());

//        pageGeometries[page] = QRect(QPoint(0, 0), pageSize);
//    }

//    totalWidth += m_documentMargins.left() + m_documentMargins.right();

//    int pageY = m_documentMargins.top();

//    // calculate page positions
//    for (int page = startPage; page < endPage; ++page) {
//        const QSize pageSize = pageGeometries[page].size();

//        // center horizontal inside the viewport
//        const int pageX = (qMax(totalWidth, m_viewport.width()) - pageSize.width()) / 2;

//        pageGeometries[page].moveTopLeft(QPoint(pageX, pageY));

//        pageY += pageSize.height() + m_pageSpacing;
//    }

//    pageY += m_documentMargins.bottom();

//    documentLayout.pageGeometries = pageGeometries;

//    // calculate overall document size
//    documentLayout.documentSize = QSize(totalWidth, pageY);

//    return documentLayout;
//}

void PdfViewer::PrintCurrentDocument()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, documentWidget);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(lastPath);
    printer.setFullPage(false);
    dialog.setWindowTitle(tr("Print Document"));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));
    printer.setFullPage(true);
    if (dialog.exec() == QDialog::Accepted)
    {
        QPainter painter;
        painter.begin(&printer);
        documentWidget->render(&painter);
        painter.end();
    }
}
