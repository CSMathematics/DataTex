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

#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include <QLabel>
#include <QRectF>
#include <poppler-qt5.h>
#include <QWidget>
#include <QRubberBand>

class QRubberBand;

class DocumentWidget : public QLabel
{
    Q_OBJECT

public:
    DocumentWidget(QWidget *parent = nullptr);
    ~DocumentWidget();
    Poppler::Document *document();
    QMatrix matrix() const;
    qreal scale() const;

public slots:
    bool setDocument(const QString &filePath);
    void setPage(int page = -1);
    void previousPage();
    void nextPage();
    void setScale(qreal scale);
    int getPage();
    QString getDocument();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void pageChanged(int currentPage);

private:
    void showPage(int page = -1);

    Poppler::Document *doc;
    int currentPage;
    QPoint dragPosition;
    QRubberBand *rubberBand;
    qreal scaleFactor;
    QString file;
};

#endif
