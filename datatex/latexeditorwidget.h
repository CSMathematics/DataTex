#ifndef LATEXEDITORWIDGET_H
#define LATEXEDITORWIDGET_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QTextEdit>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtWidgets/QTextEdit>
#else
#include <QtGui/QTextEdit>
#endif
#include <QPushButton>
#include <QSplitter>
#include <QSqlQuery>
#include "latextoolbar.h"
#include "qpdfviewer.h"
// #include "qpdfview.h"
#include <QPdfView>
#include "filecommands.h"


QT_BEGIN_NAMESPACE
class QTextDocument;
class LatexTextWidget;
class LatexToolBar;
class MathToolBar;
class PreambleToolBar;
class FileToolBar;
class LineNumberArea;
QT_END_NAMESPACE

class LatexTextBrowser : public QTextEdit
{
    Q_OBJECT

public:
    explicit LatexTextBrowser(QWidget *parent = 0);
    ~LatexTextBrowser();

    int getFirstVisibleBlockId();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    static QList<int> FileLines;
    static QList<int> DataLines;
    static QList<QTextCursor> cursorList;
signals:

public slots:

    void resizeEvent(QResizeEvent *e);
    static void ShowDifferences(LatexTextBrowser * widget1,LatexTextBrowser * widget2);
    static void clearFormat(LatexTextBrowser * widget);

private slots:

    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF /*rect_f*/);
    void updateLineNumberArea(int /*slider_pos*/);
    void updateLineNumberArea();

private:

    LineNumberArea *lineNumberArea;
    static int currentWidth;
    static bool buttonsInPlace;
};

class LatexTextEdit : public LatexTextBrowser
{
    Q_OBJECT

public:
    LatexTextEdit(QWidget *parent = 0);

private slots:

    void SetEditorReadOnly(LatexTextEdit * widget);
};

class LineNumberArea : public QWidget
{
    Q_OBJECT

public:
    LineNumberArea(LatexTextBrowser *editor);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    LatexTextBrowser *codeEditor;
};

class FileToolBar : public QToolBar
{
    Q_OBJECT

public:
    FileToolBar(LatexTextWidget *editor, bool exclusiveTexPdf = true);
    ~FileToolBar();
    QAction *OpenFolder;
    QAction *ShowTex;
    QAction *ShowPdf;
    QAction *BuildFile;
    QAction *ShowMetadata;
    QAction *ShowTopBar;
    QAction *ShowSideBar;

signals:
    void showTexFile();
    void showPdfFile();
    void Build();
};

class LatexTextWidget : public QWidget
{
    Q_OBJECT

public:
    bool UseMathBar,UsePreambleBar;
    LatexTextWidget(QWidget *editor, bool useMath = true, bool usePreamble = false);
    LatexToolBar * toolBar;
    MathToolBar * mathBar;
    PreambleToolBar * preambleBar;
    LatexTextEdit * editor;

private:
    QGridLayout * glayout;
    void setup();
    void SaveContentToDatabase(QString fileName, QString content);
};

#endif // LATEXEDITORWIDGET_H
