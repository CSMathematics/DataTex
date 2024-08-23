/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtPDF module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qpdfviewer.h"
#include "ui_qpdfviewer.h"

#include "pageselector.h"
#include "zoomselector.h"

#include <QFileDialog>
#include <QMessageBox>
#include <qpdfbookmarkmodel.h>
#include <qpdfdocument.h>
#include <qpdfpagenavigation.h>
#include <QtMath>
#include <QDesktopServices>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QPrinter>
#include <QtPrintSupport>

const qreal zoomMultiplier = qSqrt(2.0);

Q_LOGGING_CATEGORY(lcExample, "qt.examples.pdfviewer")

QPdfViewer::QPdfViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QPdfViewer)
    , m_zoomSelector(new ZoomSelector(this))
    , m_pageSelector(new PageSelector(this))
    , m_document(new QPdfDocument(this))
{
    ui->setupUi(this);
    statusBar()->hide();
    m_zoomSelector->setMaximumWidth(150);
    ui->mainToolBar->insertWidget(ui->actionZoom_In, m_zoomSelector);

    m_pageSelector->setMaximumWidth(150);
    ui->mainToolBar->addWidget(m_pageSelector);

    m_pageSelector->setPageNavigation(ui->pdfView->pageNavigation());
    connect(m_zoomSelector, &ZoomSelector::zoomModeChanged, ui->pdfView, &QPdfView::setZoomMode);
    connect(m_zoomSelector, &ZoomSelector::zoomFactorChanged, ui->pdfView, &QPdfView::setZoomFactor);
    m_zoomSelector->reset();
    ui->pdfView->setDocument(m_document);
    connect(ui->pdfView, &QPdfView::zoomFactorChanged,
            m_zoomSelector, &ZoomSelector::setZoomFactor);
    ui->pdfView->setPageMode(QPdfView::MultiPage);
    on_actionHideTab_triggered();
}

QPdfViewer::~QPdfViewer()
{
    delete ui;
}

void QPdfViewer::open(const QUrl &docLocation)
{
    if (docLocation.isLocalFile()) {
        m_document->load(docLocation.toLocalFile());
        const auto documentTitle = m_document->metaData(QPdfDocument::Title).toString();
        setWindowTitle(!documentTitle.isEmpty() ? documentTitle : QStringLiteral("PDF Viewer"));
    } else {
        qCDebug(lcExample) << docLocation << "is not a valid local file";
        QMessageBox::critical(this, tr("Failed to open"), tr("%1 is not a valid local file").arg(docLocation.toString()));
    }
    documentPath = docLocation.toLocalFile();
    qCDebug(lcExample) << docLocation;
}

void QPdfViewer::on_actionOpen_triggered()
{
    QUrl toOpen = QFileDialog::getOpenFileUrl(this, tr("Choose a PDF"), QUrl(), "Portable Documents (*.pdf)");
    if (toOpen.isValid())
        open(toOpen);
}

void QPdfViewer::on_actionPdf_triggered()
{
    QDesktopServices::openUrl(QUrl("file://"+documentPath));
}

void QPdfViewer::on_actionPrint_triggered()
{
//    QPrinter printer(QPrinter::HighResolution);
//    printer.setOutputFormat(QPrinter::PdfFormat);
//    printer.setOutputFileName(documentPath);
//    QPrintDialog*dlg = new QPrintDialog(&printer,&ui->pdfView);
//    dlg->setWindowTitle(QObject::tr("Print Document"));

//    if(dlg->exec() == QDialog::Accepted) {
//        parent.print(&printer);
//    }
//    delete dlg;
QPrinter printer;

// TODO : create a custom print dialog, page range sucks, lines range would be better
QPrintDialog dialog(&printer, this);
dialog.setWindowTitle(tr("Print Source Code"));
dialog.setEnabledOptions(QPrintDialog::PrintToFile | QPrintDialog::PrintPageRange);

if ( dialog.exec() == QDialog::Accepted )
{
//    m_document->print(&printer);
}
}

//void QPdfViewer::on_actionAbout_Qt_triggered()
//{
//    QMessageBox::aboutQt(this);
//}

void QPdfViewer::on_actionZoom_In_triggered()
{
    ui->pdfView->setZoomFactor(ui->pdfView->zoomFactor() * zoomMultiplier);
}

void QPdfViewer::on_actionZoom_Out_triggered()
{
    ui->pdfView->setZoomFactor(ui->pdfView->zoomFactor() / zoomMultiplier);
}

void QPdfViewer::on_actionPrevious_Page_triggered()
{
    ui->pdfView->pageNavigation()->goToPreviousPage();
}

void QPdfViewer::on_actionNext_Page_triggered()
{
    ui->pdfView->pageNavigation()->goToNextPage();
}

void QPdfViewer::on_actionContinuous_triggered()
{
    ui->pdfView->setPageMode(ui->actionContinuous->isChecked() ? QPdfView::MultiPage : QPdfView::SinglePage);
}

void QPdfViewer::on_actionHideTab_triggered()
{
    ui->tabWidget->setVisible(false);
}

void QPdfViewer::on_actionShowTab_triggered()
{
    ui->tabWidget->setVisible(true);
}

QAbstractScrollArea *QPdfViewer::getPdfView()
{
    return ui->pdfView;
}

void QPdfViewer::hideToolBar()
{
    ui->mainToolBar->hide();
}
