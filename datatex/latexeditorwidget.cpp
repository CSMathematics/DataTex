#include <QScrollBar>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QRectF>
#include <QTextStream>
#include <QPushButton>
#include <QDebug>
#include "latexhighlighter.h"
#include "latexeditorwidget.h"
#include "datatex.h"
#include "sqlfunctions.h"
#include <QDebug>
#include "qpdfviewer.h"


QList<QTextCursor> LatexTextBrowser::cursorList;
QList<int> LatexTextBrowser::FileLines;
QList<int> LatexTextBrowser::DataLines;
int LatexTextBrowser::currentWidth;
bool LatexTextBrowser::buttonsInPlace;

LatexTextBrowser::LatexTextBrowser(QWidget *parent) :
    QTextEdit(parent)
{
    LatexHighlighter * h = new LatexHighlighter();
    h->setDocument(LatexTextBrowser::document());
    setReadOnly(true);
    QSettings settings;
    QFont font;// = settings.value("Application_Settings/EditorFont").value<QFont>();
    QString fontfamily = settings.value("Application_Settings/EditorFont").toString();
    int fontsize = settings.value("Application_Settings/EditorFontSize").toInt();
    qDebug()<<fontfamily;
    font.setFamily(fontfamily);
    font.setFixedPitch(true);
    font.setPointSize(fontsize);
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
    QString ContentInFile = widget1->toPlainText();
    QString ContentInDatabase = widget2->toPlainText();
    QStringList lines1 = ContentInFile.split("\n", QString::SkipEmptyParts);
    QStringList lines2 = ContentInDatabase.split("\n", QString::SkipEmptyParts);

    for (int i = 0; i < lines1.size(); ++i) {
        bool foundMatch = false;
        QString line1 = lines1[i];
        for (int j = 0; j < lines2.size(); ++j) {
            if (line1 == lines2[j]) {
                foundMatch = true;
                break;
            }
        }
        if (!foundMatch) {
            FileLines.append(i);
        }
    }
    for (int i = 0; i < lines2.size(); ++i) {
        bool foundMatch = false;
        QString line2 = lines2[i];
        for (int j = 0; j < lines1.size(); ++j) {
            if (line2 == lines1[j]) {
                foundMatch = true;
                break;
            }
        }
        if (!foundMatch) {
            DataLines.append(i);
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

    int space = 25 +  fontMetrics().horizontalAdvance(QLatin1Char('9')) * (digits);

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
    QFile autoComplete(":/complete/completion.txt");
    autoComplete.open(QIODevice::ReadOnly);
    QStringList commands;
    QTextStream stream(&autoComplete);
    while(!stream.atEnd()){
        QString line = stream.readLine();
        commands.append(line.remove("\n"));
    }
    autoComplete.close();
    QCompleter *Completer = new QCompleter(this);
    Completer->setWidget(this);
    Completer->setCompletionMode(QCompleter::PopupCompletion);
    Completer->setCaseSensitivity(Qt::CaseInsensitive);
    //    QObject::connect(Completer, QOverload<const QString &>::of(&QCompleter::activated),
    //                     this, &QTextEdit::insertCompletion);
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

LatexTextWidget::LatexTextWidget(QWidget * parent,bool useMath,bool usePreamble) :
    QWidget(parent),
    toolBar(new LatexToolBar(this)),
    editor(new LatexTextEdit(this))
{
    UseMathBar = useMath;
    UsePreambleBar = usePreamble;
    mathBar = nullptr;
    preambleBar = nullptr;
    glayout = new QGridLayout(this);
    glayout->setContentsMargins(0, 0, 0, 0);
    glayout->setSpacing(0);
    setup();
    connect(editor, &QTextEdit::textChanged, this, [=](){
        toolBar->Save->setEnabled(true);
        toolBar->Undo->setEnabled(true);
    });

    connect(toolBar->Save,&QAction::triggered,editor,[&](){
        QString FileContent = editor->toPlainText();
        QString DatabaseFilePath = editor->documentTitle();
        QFile file(DatabaseFilePath);
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        QTextStream ContentStream(&file);
        QString PreviousContent = ContentStream.readAll();
        file.resize(0);
        ContentStream << FileContent;
        file.close();

        SaveContentToDatabase(QFileInfo(DatabaseFilePath).baseName(),FileContent);
        if(PreviousContent != FileContent){
            QSqlQuery needsUpdate(DataTex::CurrentDocumentDataBase);
            needsUpdate.exec(QString("UPDATE Documents SET NeedsUpdate = 1 WHERE Id IN (SELECT Document_Id FROM Files_per_Document WHERE File_Id = \"%1\")").arg(QFileInfo(DatabaseFilePath).baseName()));
            QSqlQuery editEntry(DataTex::CurrentTexFilesDataBase);
            editEntry.exec(QString("INSERT INTO Edit_History (File_Id,Date_Time,Modification,FileContent,Metadata)"
                                   "VALUES ('%1','%2','Content modified',\"%3\",'%4')")
                               .arg(QFileInfo(DatabaseFilePath).baseName(),QDateTime::currentDateTime().toString("dd/M/yyyy hh:mm"),FileContent,{}));
        }
        DBFileInfo fileInfo = DBFileInfo(QFileInfo(DatabaseFilePath).baseName(),DataTex::CurrentTexFilesDataBase);
        DBFileInfo::WriteDBFile(fileInfo);
    });
    connect(toolBar->Copy,&QAction::triggered,editor,[&](){
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(editor->toPlainText());
    });

    connect(toolBar,&LatexToolBar::SendToEditor,editor,[&](QString command){
        editor->insertPlainText(command);
    });
    connect(mathBar,&MathToolBar::SendToEditor,editor,[&](QString command){
        editor->insertPlainText(command);
    });

//    connect(editor, &QTextEdit::copyAvailable, toolBar->Copy, &QAction::setEnabled);

    editor->document()->toPlainText();
}

void LatexTextWidget::setup()
{
    if(UseMathBar){
        mathBar = new MathToolBar(this);
        glayout->addWidget(mathBar,1,0);
        QSplitter * splitter = new QSplitter(this);
        glayout->addWidget(splitter,1,1);
        splitter->addWidget(mathBar->List);
        splitter->addWidget(editor);
        splitter->setSizes(QList<int>({45, 300}));
    }
    else{
        glayout->addWidget(editor,1,0,1,2);
    }
    if(UsePreambleBar){
        preambleBar = new PreambleToolBar(this);
        QHBoxLayout *hbox = new QHBoxLayout(this);
        QSpacerItem *spacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
        glayout->addItem(hbox,0,0,1,2);
        hbox->addWidget(preambleBar);
        hbox->addWidget(toolBar);
        hbox->addItem(spacer);
    }
    else{
        glayout->addWidget(toolBar,0,0,1,2);
    }
}

void LatexTextWidget::SaveContentToDatabase(QString fileName, QString content)
{
    QSqlQuery WriteContent(DataTex::CurrentTexFilesDataBase);
    WriteContent.prepare("UPDATE Database_Files SET FileContent = :content WHERE Id = :file");
    WriteContent.bindValue(":file",QFileInfo(fileName).baseName());
    WriteContent.bindValue(":content",content);
    WriteContent.exec();

    QSqlQuery WriteContent_2(DataTex::CurrentDocumentDataBase);
    WriteContent_2.prepare("UPDATE Documents SET Content = :content WHERE Id = :file");
    WriteContent_2.bindValue(":file",QFileInfo(fileName).baseName());
    WriteContent_2.bindValue(":content",content);
    WriteContent_2.exec();
}

FileToolBar::FileToolBar(LatexTextWidget *editor,bool exclusiveTexPdf) : QToolBar(editor)
{
    setIconSize(QSize(16,16));
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);


    ShowTex = new QAction(QIcon(":/images/latex-config.svg"),"", this);
    ShowTex->setCheckable(true);
    ShowTex->setChecked(true);
    ShowTex->setVisible(exclusiveTexPdf);
    connect(ShowTex,&QAction::triggered,editor,[=](){emit showTexFile();});

    ShowPdf = new QAction(QIcon(":/images/pdf2.svg"),"", this);
    ShowPdf->setCheckable(true);
    connect(ShowPdf,&QAction::triggered,editor,[=](){emit showPdfFile();});

    ShowMetadata = new QAction(QIcon(":/images/help-about.svg"),"", this);

    BuildFile = new QAction(QIcon(":/images/media-playback-start.svg"),"", this);
    connect(BuildFile,&QAction::triggered,editor,[=](){emit Build();});
    OpenFolder = new QAction(QIcon(":/images/document-open-data.svg"),"", this);
    connect(OpenFolder,&QAction::triggered,editor,[=](){
        QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(editor->editor->documentTitle()).absolutePath()));});
    ShowTopBar = new QAction(QIcon(":/images/ToolBar_Icons/topbar.svg"),"", this);
    ShowTopBar->setCheckable(true);
    ShowTopBar->setChecked(true);
    connect(ShowTopBar,&QAction::triggered,editor,[=](bool checked){editor->toolBar->setVisible(checked);});
    ShowSideBar = new QAction(QIcon(":/images/ToolBar_Icons/sidebar.svg"),"", this);
    ShowSideBar->setCheckable(true);
    ShowSideBar->setChecked(true);
    connect(ShowSideBar,&QAction::triggered,editor,[=](bool checked){editor->mathBar->setVisible(checked);});
    addAction(ShowTex);
    addAction(ShowPdf);
    addAction(ShowMetadata);
    addSeparator();
    addAction(BuildFile);
    addAction(OpenFolder);
    addSeparator();
    addAction(ShowTopBar);
    addAction(ShowSideBar);
    if(exclusiveTexPdf){
        QActionGroup *ActGroup = new QActionGroup(this);
        ActGroup->addAction(ShowTex);
        ActGroup->addAction(ShowPdf);
        ActGroup->setExclusive(true);
        ActGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);
    }
    setOrientation(Qt::Horizontal);
}

FileToolBar::~FileToolBar()
{
    delete OpenFolder;
    delete ShowTex;
    delete ShowPdf;
    delete BuildFile;
    delete ShowMetadata;
    delete ShowTopBar;
    delete ShowSideBar;
}
