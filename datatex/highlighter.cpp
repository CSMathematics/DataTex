/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QScrollBar>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QRectF>
#include <QTextStream>
#include <QPushButton>
#include <QDebug>
#include "highlighter.h"

QList<DifferenceButton *> LatexTextBrowser::buttonlist;
QList<QTextCursor> LatexTextBrowser::cursorList;
QList<int> LatexTextBrowser::FileLines;
QList<int> LatexTextBrowser::DataLines;
int LatexTextBrowser::currentWidth;
bool LatexTextBrowser::buttonsInPlace;

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        QStringLiteral("\\bchar\\b"), QStringLiteral("\\busepackage\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(QColor(200, 50, 40 ));
    rule.pattern = QRegularExpression(QStringLiteral("\\\\\\b\[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor(170,170,170));

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression(QStringLiteral("^%.+"));
    commentEndExpression = QRegularExpression(QStringLiteral("%.+"));

    packageNameFormat.setFontWeight(QFont::Bold);
    packageNameFormat.setForeground(QColor(0,170,0));
    rule.pattern = QRegularExpression(QStringLiteral("\\b{[A-Za-z0-9_]+}"));
    rule.format = packageNameFormat;
    highlightingRules.append(rule);

    mathFormat.setFontWeight(QFont::Bold);
    mathFormat.setForeground(QColor(140,190,0));
    rule.pattern = QRegularExpression(QStringLiteral("^\\\\\\[[\\s\\S]*[\\d\\D]*[\\w\\W]*_*\n*\\\\*]*\\\\\\]$"));
    rule.format = mathFormat;
    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

LatexTextBrowser::LatexTextBrowser(QWidget *parent) :
    QTextEdit(parent)
{
    Highlighter * h = new Highlighter();
    h->setDocument(LatexTextBrowser::document());
    setReadOnly(true);
    QFont font;
    font.setFamily("DejaVu Sans Mono");
    font.setFixedPitch(true);
    font.setPointSize(10);
    setFont(font);

    // Line numbers
    lineNumberArea = new LineNumberArea(this);
    ///
    connect(this->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumberArea(int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumberArea()));
    ///
    updateLineNumberAreaWidth(0);
    buttonsInPlace = false;
}

LatexTextBrowser::~LatexTextBrowser()
{
//    for (int i=0;i<buttonlist.count();i++) {
//        delete buttonlist[i];
//    }
}

void LatexTextBrowser::ShowDifferences(LatexTextBrowser *widget1, LatexTextBrowser *widget2)
{
//    buttonlist.clear();
    FileLines.clear();
    DataLines.clear();
    cursorList.clear();
    int FileLineNumber =-1;
    int DataLineNumber =-1;
    QString ContentInFile = widget1->toPlainText();
    QString ContentInDatabase = widget2->toPlainText();
    QTextStream FileStream(&ContentInFile);
    QTextStream DataStream(&ContentInDatabase);
    while(!DataStream.atEnd() || !FileStream.atEnd()){
        FileLineNumber++;
        DataLineNumber++;
        QString fileline = FileStream.readLine();
        QString dataline = DataStream.readLine();
        if(dataline != fileline && !ContentInDatabase.contains(fileline)){
            FileLines.append(FileLineNumber);
        }
        if(dataline != fileline && !ContentInFile.contains(dataline)){
            DataLines.append(DataLineNumber);
        }
    }
    clearFormat(widget1);
    clearFormat(widget2);
    for(int i = 0;i<FileLines.count();i++) {
        QTextBlock fileblock = widget1->document()->findBlockByNumber(FileLines[i]);
        QTextCursor filecursor(fileblock);
        cursorList.append(filecursor);
        QTextBlockFormat fileblockFormat = filecursor.blockFormat();
        fileblockFormat.setBackground(QColor(220,235,235));
        filecursor.setBlockFormat(fileblockFormat);
    }
    for(int i = 0;i<DataLines.count();i++) {
        QTextBlock datablock = widget2->document()->findBlockByNumber(DataLines[i]);
        QTextCursor datacursor(datablock);
        QTextBlockFormat datablockFormat = datacursor.blockFormat();
        datablockFormat.setBackground(QColor(210,245,200));
        datacursor.setBlockFormat(datablockFormat);
    }
}

void LatexTextBrowser::ShowButtons(LatexTextBrowser * widget1)
{
//    Button for each difference ----------
    for (int i=0;i<FileLines.count();i++) {
        if(FileLines.count()>0){
            DifferenceButton * b = new DifferenceButton(widget1);
            buttonlist.append(b);
            buttonlist[i]->setGeometry(QRect(50,widget1->cursorRect(cursorList[i]).y(),30,
                                       widget1->document()->documentLayout()->blockBoundingRect(cursorList[i].block()).height()));
            buttonsInPlace = true;
            connect(widget1->verticalScrollBar(),&QScrollBar::valueChanged,widget1,[=](){
                buttonlist[i]->move(50,widget1->cursorRect(cursorList[i]).y()+widget1->document()->documentMargin());
                widget1->updateGeometry();
            });
            connect(widget1,&LatexTextBrowser::cursorPositionChanged,widget1,[=](){
                buttonlist[i]->move(/*widget1->geometry().width()-1.7*buttonlist[i]->width()*/50,widget1->cursorRect(cursorList[i]).y());
                widget1->updateGeometry();
            });
        }
    }
}

void LatexTextBrowser::clearFormat(LatexTextBrowser * widget)
{
    QTextCursor cursor(widget->textCursor());
    cursor.select(QTextCursor::Document);
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.clearBackground();
    cursor.setBlockFormat(blockFormat);
}

LineNumberArea::LineNumberArea(LatexTextBrowser *editor) : QWidget(editor) {
    codeEditor = editor;
}

QSize LineNumberArea::sizeHint() const {
    return QSize(((LatexTextBrowser *)codeEditor)->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    ((LatexTextBrowser *)codeEditor)->lineNumberAreaPaintEvent(event);
}

int LatexTextBrowser::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, this->document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 15 +  fontMetrics().horizontalAdvance(QLatin1Char('9')) * (digits);

    return space;
}

void LatexTextBrowser::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void LatexTextBrowser::updateLineNumberArea(QRectF /*rect_f*/)
{
    LatexTextBrowser::updateLineNumberArea();
}
void LatexTextBrowser::updateLineNumberArea(int /*slider_pos*/)
{
    LatexTextBrowser::updateLineNumberArea();
}
void LatexTextBrowser::updateLineNumberArea()
{
    /*
     * When the signal is emitted, the sliderPosition has been adjusted according to the action,
     * but the value has not yet been propagated (meaning the valueChanged() signal was not yet emitted),
     * and the visual display has not been updated. In slots connected to this signal you can thus safely
     * adjust any action by calling setSliderPosition() yourself, based on both the action and the
     * slider's value.
     */
    // Make sure the sliderPosition triggers one last time the valueChanged() signal with the actual value !!!!
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    // Since "QTextEdit" does not have an "updateRequest(...)" signal, we chose
    // to grab the imformations from "sliderPosition()" and "contentsRect()".
    // See the necessary connections used (Class constructor implementation part).

    QRect rect =  this->contentsRect();
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    updateLineNumberAreaWidth(0);
    //----------
    int dy = this->verticalScrollBar()->sliderPosition();
    if (dy > -1) {
        lineNumberArea->scroll(0, dy);
    }

    int first_block_id = getFirstVisibleBlockId();
    if (first_block_id == 0 || this->textCursor().block().blockNumber() == first_block_id-1){
        this->verticalScrollBar()->setSliderPosition(dy-this->document()->documentMargin());
    }
}


void LatexTextBrowser::resizeEvent(QResizeEvent *e)
{
    QTextEdit::resizeEvent(e);
    QRect cr = this->contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
    int buttons = buttonlist.count();
    if(buttons>0 && buttonsInPlace){
        for (int i = 0;i<buttons;i++) {
            buttonlist[i]->setGeometry(
                QRect(cr.right()-1.6*buttonlist[i]->width(),
                this->cursorRect(cursorList[i]).y(),30,
                this->document()->documentLayout()->blockBoundingRect(cursorList[i].block()).height()));
        }
        qDebug()<<"Width = "<<currentWidth<<"Cursor y = "<<this->cursorRect(cursorList[0]).y();
    }
    currentWidth = this->width();
}


int LatexTextBrowser::getFirstVisibleBlockId()
{
    // Detect the first block for which bounding rect - once translated
    // in absolute coordinated - is contained by the editor's text area

    // Costly way of doing but since "blockBoundingGeometry(...)" doesn't
    // exists for "QTextEdit"...

    QTextCursor curs = QTextCursor(this->document());
    curs.movePosition(QTextCursor::Start);
    for(int i=0; i < this->document()->blockCount(); ++i)
    {
        QTextBlock block = curs.block();

        QRect r1 = this->viewport()->geometry();
        QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                    this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                        this->verticalScrollBar()->sliderPosition()
                        ) ).toRect();

        if (r1.contains(r2, true)) { return i; }

        curs.movePosition(QTextCursor::NextBlock);
    }

    return 0;
}

void LatexTextBrowser::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(230,230,230));
    int blockNumber = this->getFirstVisibleBlockId();

    QTextBlock block = this->document()->findBlockByNumber(blockNumber);
    QTextBlock prev_block = (blockNumber > 0) ? this->document()->findBlockByNumber(blockNumber-1) : block;
    int translate_y = (blockNumber > 0) ? -this->verticalScrollBar()->sliderPosition() : 0;

    int top = this->viewport()->geometry().top();

    // Adjust text position according to the previous "non entirely visible" block
    // if applicable. Also takes in consideration the document's margin offset.
    int additional_margin;
    if (blockNumber == 0)
        // Simply adjust to document's margin
        additional_margin = (int) this->document()->documentMargin() -1 - this->verticalScrollBar()->sliderPosition();
    else
        // Getting the height of the visible part of the previous "non entirely visible" block
        additional_margin = (int) this->document()->documentLayout()->blockBoundingRect(prev_block)
                .translated(0, translate_y).intersected(this->viewport()->geometry()).height();

    // Shift the starting point
    top += additional_margin;

    int bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();

    QColor col_1(0,0,0);      // Current line (custom green)
    QColor col_0(120, 120, 120);    // Other lines  (custom darkgrey)
    QFont Bfont;
    Bfont.setBold(true);
    QFont font;
    font.setBold(false);

    // Draw the numbers (displaying the current line number in green)
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(120, 120, 120));
            painter.setPen((this->textCursor().blockNumber() == blockNumber) ? col_1 : col_0);
            painter.setFont((this->textCursor().blockNumber() == blockNumber) ? Bfont : font );
            painter.drawText(-5, top,
                             lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();
        ++blockNumber;
    }
}

LatexTextEdit::LatexTextEdit(QWidget *parent) :
    LatexTextBrowser(parent)
{
    setReadOnly(false);
    connect(this, &QTextEdit::cursorPositionChanged, this, [=](){
        SetEditorReadOnly(this);
    });
}

void LatexTextEdit::SetEditorReadOnly(LatexTextEdit * widget)
{
    QString CurrentLineText = widget->textCursor().block().text();
    if(CurrentLineText.startsWith("%# Database File") || CurrentLineText.startsWith("%# End of file")
        || CurrentLineText.startsWith("%# Database Document :") || CurrentLineText.startsWith("%@ Database source:")
        || CurrentLineText.startsWith("%@ Document type:")){
        widget->setReadOnly(true);
    }
    else{
        widget->setReadOnly(false);
    }
}

DifferenceButton::DifferenceButton(LatexTextBrowser * editor) :
    QPushButton(editor)
{
    setIcon(QIcon(":/images/go-next.svg"));
    setMaximumWidth(30);
    setFlat(true);
    setStyleSheet("QPushButton {background-color: rgb(220,235,235);"
                  "border: 0px solid black;"
                  "border-radius: 0px;color:rgb(220,235,235);}"
                  "QPushButton:pressed {"
                  "background-color:rgb(255,255,255);"
                  "color: rgb(220,235,235);}");
}
