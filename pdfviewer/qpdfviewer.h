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

#ifndef QPDFVIEWER_H
#define QPDFVIEWER_H

#include <QLoggingCategory>
#include <QMainWindow>
#include <QAbstractScrollArea>
//#include "datatex.h"

Q_DECLARE_LOGGING_CATEGORY(lcExample)

QT_BEGIN_NAMESPACE
namespace Ui {
class QPdfViewer;
}

class QPdfDocument;
class QPdfView;
QT_END_NAMESPACE

class PageSelector;
class ZoomSelector;

class QPdfViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit QPdfViewer(QWidget *parent = nullptr);
    ~QPdfViewer();

public slots:
    void open(const QUrl &docLocation);
    QAbstractScrollArea *getPdfView();
    void hideToolBar();

private slots:
    // action handlers
    void on_actionOpen_triggered();
    void on_actionPdf_triggered();
    void on_actionPrint_triggered();
//    void on_actionAbout_Qt_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionPrevious_Page_triggered();
    void on_actionNext_Page_triggered();
    void on_actionContinuous_triggered();
    void on_actionHideTab_triggered();
    void on_actionShowTab_triggered();

private:
    Ui::QPdfViewer *ui;
    ZoomSelector *m_zoomSelector;
    PageSelector *m_pageSelector;
    QPdfDocument *m_document;
    QString documentPath;
};

#endif // QPDFVIEWER_H
