#include "latextoolbar.h"

LatexToolBar::LatexToolBar(QWidget *parent)
    : QWidget{parent}
{
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    ToolBar = new QToolBar;
    ToolsMenu = new QMenu;
    Save = CreateNewAction(Save,[&](){Save->setEnabled(false);},"Ctrl+S",QIcon(":/images/document-save.svg"),tr("Save changes"));
    Undo = CreateNewAction(Undo,[](){},"Ctrl+Z",QIcon(":/images/edit-undo.svg"),tr("Undo changes"));
    Copy = CreateNewAction(Copy,[](){},"Ctrl+C",QIcon(":/images/edit-copy.svg"),tr("Copy content"));
    Part = CreateNewAction(Part,[&](){SectioningMenu();},"",QIcon(":/images/view-table-of-contents-rtl.svg"),tr("Sectioning"));
    FontSize = CreateNewAction(FontSize,[&](){FontSizeMenu();},"",QIcon(":/images/format-font-size-less.svg"),tr("Font size"));
    Math = CreateNewAction(Math,[&](){MathMenu();},"Ctrl+M",QIcon(":/images/format-text-symbol.svg"),tr("Math"));
    Environments = CreateNewAction(Environments,[](){},"Ctrl+E",QIcon(":/images/environment.svg"),tr("Environments"));
    ListEnvironments = CreateNewAction(ListEnvironments,[](){},"Ctrl+L",QIcon(":/images/sort-presence.svg"),tr("Lists"));
    Tabular = CreateNewAction(Tabular,[](){},"Ctrl+T",QIcon(":/images/view-calendar-agenda.svg"),tr("Tables"));
    FontStyle = CreateNewAction(FontStyle,[&](){FontStyleMenu();},"Ctrl+F",QIcon(":/images/format-text-bold.svg"),tr("Font style"));
    Brackets = CreateNewAction(Brackets,[&](){FontStyleMenu();},"Ctrl+Shift+B",QIcon(":/images/ToolBar_Icons/parenthesis.svg"),tr("Brackets"));

    ToolBar->addAction(Save);
    ToolBar->addAction(Undo);
    ToolBar->addAction(Copy);
    ToolBar->addSeparator();
    ToolBar->addAction(Part);
    ToolBar->addAction(FontSize);
    ToolBar->addAction(FontStyle);
    ToolBar->addSeparator();
    ToolBar->addAction(Math);
    ToolBar->addAction(Environments);
    ToolBar->addAction(ListEnvironments);
    ToolBar->addAction(Tabular);
    ToolBar->addAction(Brackets);


    ToolBar->setIconSize(QSize(16,16));
    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(ToolBar);
    ToolBar->setOrientation(Qt::Horizontal);

//#ifndef QT_NO_CLIPBOARD
//    Copy->setEnabled(false);
//#endif // !QT_NO_CLIPBOARD
}

LatexToolBar::~LatexToolBar()
{
    delete ToolsMenu;
    delete ToolBar;
    delete Save;
    delete Undo;
    delete Copy;
    delete Part;
    delete FontSize;
    delete Math;
    delete Environments;
    delete ListEnvironments;
    delete Tabular;
    delete FontStyle;
    delete Brackets;
}

QAction * LatexToolBar::CreateNewAction(QAction * Action, std::function<void()> Function, QString ShortCut, QIcon Icon, QString Description)
{
    Action = new QAction(Icon,Description, this);
    Action->setShortcut(QString(ShortCut));
    QObject::connect(Action, &QAction::triggered, this, Function);
    return Action;
}

void LatexToolBar::SectioningMenu()
{
    QMenu * SectionMenu = new QMenu(this);
    QAction* PartAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\part{<text>}");},"",
                          QIcon(":/images/ToolBar_Icons/Part.svg"),tr("Part"));
    QAction* ChapterAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\chapter{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/Chapter.svg"),tr("Chapter"));
    QAction* SectionAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\section{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/Section.svg"),tr("Section"));
    QAction* SubSectionAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\subsection{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/SubSection.svg"),tr("Subsection"));
    QAction* SubsubSectionAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\subsubsection{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/SubsubSection.svg"),tr("Subsubsection"));
    QAction* ParagraphAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\paragraph{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/Paragraph.svg"),tr("Paragraph"));
    QAction* SubParagraphAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\subparagraph{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/SubParagraph.svg"),tr("Subparagraph"));
    SectionMenu->addAction(PartAction);
    SectionMenu->addAction(ChapterAction);
    SectionMenu->addAction(SectionAction);
    SectionMenu->addAction(SubSectionAction);
    SectionMenu->addAction(SubsubSectionAction);
    SectionMenu->addAction(ParagraphAction);
    SectionMenu->addAction(SubParagraphAction);
    QPoint p = mapFromGlobal(QCursor::pos());
    p = this->mapToGlobal(p);
    SectionMenu->exec(p);
}

void LatexToolBar::FontSizeMenu()
{
    QMenu * FontSizeMenu = new QMenu(this);
    QAction* TinyAction = CreateNewAction(TinyAction,[&](){emit SendToEditor("\\tiny{<text>}");},"",
        QIcon(""),tr("Tiny"));
    QAction* ScriptAction = CreateNewAction(ScriptAction,[&](){emit SendToEditor("\\scriptsize{<text>}");},"",
        QIcon(""),tr("Script size"));
    QAction* FootnoteAction = CreateNewAction(FootnoteAction,[&](){emit SendToEditor("\\footnotesize{<text>}");},"",
        QIcon(""),tr("Footnote size"));
    QAction* SmallAction = CreateNewAction(SmallAction,[&](){emit SendToEditor("\\small{<text>}");},"",
        QIcon(""),tr("Small"));
    QAction* NormalAction = CreateNewAction(NormalAction,[&](){emit SendToEditor("\\normalsize{<text>}");},"",
        QIcon(""),tr("Normal"));
    QAction* largeAction = CreateNewAction(largeAction,[&](){emit SendToEditor("\\large{<text>}");},"",
        QIcon(""),tr("large"));
    QAction* LargeAction = CreateNewAction(LargeAction,[&](){emit SendToEditor("\\Large{<text>}");},"",
        QIcon(""),tr("Large"));
    QAction* LARGEAction = CreateNewAction(LARGEAction,[&](){emit SendToEditor("\\LARGE{<text>}");},"",
        QIcon(""),tr("LARGE"));
    QAction* hugeAction = CreateNewAction(hugeAction,[&](){emit SendToEditor("\\huge{<text>}");},"",
        QIcon(""),tr("huge"));
    QAction* HugeAction = CreateNewAction(HugeAction,[&](){emit SendToEditor("\\Huge{<text>}");},"",
        QIcon(""),tr("Huge"));
    FontSizeMenu->addAction(TinyAction);
    FontSizeMenu->addAction(ScriptAction);
    FontSizeMenu->addAction(FootnoteAction);
    FontSizeMenu->addAction(SmallAction);
    FontSizeMenu->addAction(NormalAction);
    FontSizeMenu->addAction(largeAction);
    FontSizeMenu->addAction(LargeAction);
    FontSizeMenu->addAction(LARGEAction);
    FontSizeMenu->addAction(hugeAction);
    FontSizeMenu->addAction(HugeAction);
    QPoint p = mapFromGlobal(QCursor::pos());
    p = this->mapToGlobal(p);
    FontSizeMenu->exec(p);
}

void LatexToolBar::MathMenu()
{
    QMenu * MainMathMenu = new QMenu(this);
    QAction* InlineMathAction = CreateNewAction(InlineMathAction,[&](){emit SendToEditor("$  $");},"",
        QIcon(":/images/ToolBar_Icons/inlinemath.svg"),tr("Inline mode $  $"));
    QAction* DisplayMathAction = CreateNewAction(DisplayMathAction,[&](){emit SendToEditor("\\[  \\]");},"",
        QIcon(":/images/ToolBar_Icons/displaymath.svg"),tr("Display mode \\[  \\]"));
    QAction* SubscriptAction = CreateNewAction(SubscriptAction,[&](){emit SendToEditor("_{}");},"",
        QIcon(":/images/ToolBar_Icons/subscript.svg"),tr("Subscript _\{\}"));
    QAction* SuperscriptAction = CreateNewAction(SuperscriptAction,[&](){emit SendToEditor("^{}");},"",
        QIcon(":/images/ToolBar_Icons/superscript.svg"),tr("Superscript"));
    QAction* FracAction = CreateNewAction(FracAction,[&](){emit SendToEditor("\\frac{<num>}{<den>}");},"",
        QIcon(":/images/ToolBar_Icons/frac.svg"),tr("Fraction"));
    QAction* DFracAction = CreateNewAction(DFracAction,[&](){emit SendToEditor("\\dfrac{<num>}{<den>}");},"",
        QIcon(":/images/ToolBar_Icons/dfrac.svg"),tr("Display fraction"));
    QAction* SqrtAction = CreateNewAction(SqrtAction,[&](){emit SendToEditor("\\sqrt{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/sqrt.svg"),tr("Square root"));

    QAction* Equation = CreateNewAction(Equation,[&](){emit SendToEditor("\\begin{equation}\ntext\n\\end{equation}");},"",
        QIcon(""),tr("\\begin{equation}"));
    QAction* Equation_star = CreateNewAction(Equation_star,[&](){emit SendToEditor("\\begin{equation*}\ntext\n\\end{equation*}");},"",
        QIcon(""),tr("\\begin{equation*}"));
    QAction* Align = CreateNewAction(Align,[&](){emit SendToEditor("\\begin{align}\ntext\n\\end{align}");},"",
        QIcon(""),tr("\\begin{align}"));
    QAction* Align_star = CreateNewAction(Align_star,[&](){emit SendToEditor("\\begin{align*}\ntext\n\\end{align*}");},"",
        QIcon(""),tr("\\begin{align*}"));
    QAction* Gather = CreateNewAction(Gather,[&](){emit SendToEditor("\\begin{gather}\ntext\n\\end{gather}");},"",
        QIcon(""),tr("\\begin{gather}"));
    QAction* Gather_star = CreateNewAction(Gather_star,[&](){emit SendToEditor("\\begin{gather*}\ntext\n\\end{gather*}");},"",
        QIcon(""),tr("\\begin{gather*}"));
    QAction* AlignAt = CreateNewAction(AlignAt,[&](){emit SendToEditor("\\begin{alignat}{cols}\ntext\n\\end{alignat}");},"",
        QIcon(""),tr("\\begin{alignat}"));
    QAction* AlignAt_star = CreateNewAction(AlignAt_star,[&](){emit SendToEditor("\\begin{alignat*}{cols}\ntext\n\\end{alignat*}");},"",
        QIcon(""),tr("\\begin{alignat*}"));
    QAction* Multiline = CreateNewAction(Multiline,[&](){emit SendToEditor("\\begin{multline}\ntext\n\\end{multline}");},"",
        QIcon(""),tr("\\begin{multline}"));
    QAction* Multiline_star = CreateNewAction(Multiline_star,[&](){emit SendToEditor("\\begin{multline*}\ntext\n\\end{multline*}");},"",
        QIcon(""),tr("\\begin{multline*}"));

    QAction* MathRoman = CreateNewAction(MathRoman,[&](){emit SendToEditor("\\mathrm{}");},"",
        QIcon(":/images/ToolBar_Icons/mathrm.svg"),tr("Roman"));
    QAction* MathBold = CreateNewAction(MathBold,[&](){emit SendToEditor("\\mathbf{}");},"",
        QIcon(":/images/ToolBar_Icons/mathbf.svg"),tr("Bold"));
    QAction* MathItalic = CreateNewAction(MathItalic,[&](){emit SendToEditor("\\mathit{}");},"",
        QIcon(":/images/ToolBar_Icons/mathit.svg"),tr("Italic"));
    QAction* MathSansSerif = CreateNewAction(MathSansSerif,[&](){emit SendToEditor("\\mathsf{}");},"",
        QIcon(":/images/ToolBar_Icons/mathsf.svg"),tr("Sans Serif"));
    QAction* MathTypeWriter = CreateNewAction(MathTypeWriter,[&](){emit SendToEditor("\\mathtt{}");},"",
        QIcon(":/images/ToolBar_Icons/mathtt.svg"),tr("Typewriter"));
    QAction* MathCal = CreateNewAction(MathCal,[&](){emit SendToEditor("\\mathcal{}");},"",
        QIcon(":/images/ToolBar_Icons/mathcal.svg"),tr("Caligraphic"));
    QAction* MathBB = CreateNewAction(MathBB,[&](){emit SendToEditor("\\mathbb{}");},"",
        QIcon(":/images/ToolBar_Icons/mathbb.svg"),tr("Blackboard Bold"));
    QAction* MathFraktur = CreateNewAction(MathFraktur,[&](){emit SendToEditor("\\mathfrak{}");},"",
        QIcon(":/images/ToolBar_Icons/mathfrak.svg"),tr("Fraktur"));
    QAction* MathScript = CreateNewAction(MathScript,[&](){emit SendToEditor("\\mathfrak{}");},"",
        QIcon(":/images/ToolBar_Icons/mathscr.svg"),tr("Script"));

//    QMenu * basicMath = new QMenu(this);
    QMenu * eqEnvironments = new QMenu(tr("Equations"),this);
    QMenu * mathFontStyles = new QMenu(tr("Font styles"),this);

    MainMathMenu->addAction(InlineMathAction);
    MainMathMenu->addAction(DisplayMathAction);
    MainMathMenu->addAction(SubscriptAction);
    MainMathMenu->addAction(SuperscriptAction);
    MainMathMenu->addAction(FracAction);
    MainMathMenu->addAction(DFracAction);
    MainMathMenu->addAction(SqrtAction);

    MainMathMenu->addMenu(eqEnvironments);
    eqEnvironments->addAction(Equation);
    eqEnvironments->addAction(Equation_star);
    eqEnvironments->addAction(Align);
    eqEnvironments->addAction(Align_star);
    eqEnvironments->addAction(Gather);
    eqEnvironments->addAction(Gather_star);
    eqEnvironments->addAction(AlignAt);
    eqEnvironments->addAction(AlignAt_star);
    eqEnvironments->addAction(Multiline);
    eqEnvironments->addAction(Multiline_star);

    MainMathMenu->addMenu(mathFontStyles);
    mathFontStyles->addAction(MathRoman);
    mathFontStyles->addAction(MathBold);
    mathFontStyles->addAction(MathItalic);
    mathFontStyles->addAction(MathSansSerif);
    mathFontStyles->addAction(MathTypeWriter);
    mathFontStyles->addAction(MathCal);
    mathFontStyles->addAction(MathBB);
    mathFontStyles->addAction(MathFraktur);
    mathFontStyles->addAction(MathScript);

    QPoint p = mapFromGlobal(QCursor::pos());
    p = this->mapToGlobal(p);
    MainMathMenu->exec(p);
}

void LatexToolBar::FontStyleMenu()
{
    QMenu * FontStyleMenu = new QMenu(this);
    QAction* Bold = CreateNewAction(Bold,[&](){emit SendToEditor("\\textbf{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/format-text-bold.svg"),tr("Bold"));
    QAction* Italic = CreateNewAction(Italic,[&](){emit SendToEditor("\\textit{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/format-text-italic.svg"),tr("Italic"));
    QAction* Underline = CreateNewAction(Underline,[&](){emit SendToEditor("\\underline{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/format-text-underline.svg"),tr("Underline"));
    QAction* Emphasis = CreateNewAction(Emphasis,[&](){emit SendToEditor("\\emph{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/emph.svg"),tr("Emphasis"));
    QAction* Slanted = CreateNewAction(Slanted,[&](){emit SendToEditor("\\textsl{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/textsl.svg"),tr("Slanted"));
    QAction* SmallCaps = CreateNewAction(SmallCaps,[&](){emit SendToEditor("\\textsc{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/textsc.svg"),tr("Small caps"));
    QAction* TypeWriter = CreateNewAction(TypeWriter,[&](){emit SendToEditor("\\texttt{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/texttt.svg"),tr("Typewriter"));
    QAction* SansSerif = CreateNewAction(SansSerif,[&](){emit SendToEditor("\\textsf{<text>}");},"",
        QIcon(":/images/ToolBar_Icons/textsf.svg"),tr("Sans serif"));

    FontStyleMenu->addAction(Bold);
    FontStyleMenu->addAction(Italic);
    FontStyleMenu->addAction(Underline);
    FontStyleMenu->addAction(Emphasis);
    FontStyleMenu->addAction(Slanted);
    FontStyleMenu->addAction(SmallCaps);
    FontStyleMenu->addAction(TypeWriter);
    FontStyleMenu->addAction(SansSerif);
    QPoint p = mapFromGlobal(QCursor::pos());
    p = this->mapToGlobal(p);
    FontStyleMenu->exec(p);
}


MathToolBar::MathToolBar(QWidget *parent)
    : QWidget{parent}
{
    MathBar = new QToolBar;
    Operators = CreateNewAction(Operators,[&](){
            List->setVisible(true);
            filterItems("Operators");
        },"Ctrl+S",QIcon(":/images/sum.svg"),tr("Save changes"));
    Relations = CreateNewAction(Relations,[&](){filterItems("Relations");},"Ctrl+Z",QIcon(":/images/leq.svg"),tr("Relations"));
    Arrows = CreateNewAction(Arrows,[&](){filterItems("Arrows");},"Ctrl+C",QIcon(":/images/Arrows.svg"),tr("Arrows"));
    Delimiters = CreateNewAction(Delimiters,[&](){filterItems("Delimiters");},"",QIcon(":/images/delimiters.svg"),tr("Delimiters"));
    Greek = CreateNewAction(Greek,[&](){filterItems("Greek");},"",QIcon(":/images/format-text-symbol.svg"),tr("Greek"));
    Cyrillic = CreateNewAction(Cyrillic,[&](){filterItems("Cyrillic");},"Ctrl+M",QIcon(":/images/cyrillic.svg"),tr("Cyrillic"));
    MiscMath = CreateNewAction(MiscMath,[&](){filterItems("MiscMath");},"Ctrl+E",QIcon(":/images/miscmath.svg"),tr("MiscMath"));
    MiscText= CreateNewAction(MiscText,[&](){filterItems("MiscText");},"Ctrl+L",QIcon(":/images/misc-text.svg"),tr("MiscText"));
    FontAwesome = CreateNewAction(FontAwesome,[&](){filterItems("FontAwesome");},"Ctrl+T",QIcon(":/images/font-awesome.svg"),tr("FontAwesome"));
    Special = CreateNewAction(Special,[&](){filterItems("Special");},"Ctrl+T",QIcon(":/images/special.svg"),tr("Special+"));

    MathBar->addAction(Operators);
    MathBar->addAction(Relations);
    MathBar->addAction(Arrows);
    MathBar->addAction(Delimiters);
    MathBar->addAction(Greek);
    MathBar->addAction(Cyrillic);
    MathBar->addAction(MiscMath);
    MathBar->addAction(MiscText);
    MathBar->addAction(FontAwesome);
    MathBar->addAction(Special);

    QActionGroup *ActGroup = new QActionGroup(MathBar);
    ActGroup->addAction(Operators);
    ActGroup->addAction(Relations);
    ActGroup->addAction(Arrows);
    ActGroup->addAction(Delimiters);
    ActGroup->addAction(Greek);
    ActGroup->addAction(Cyrillic);
    ActGroup->addAction(MiscMath);
    ActGroup->addAction(MiscText);
    ActGroup->addAction(FontAwesome);
    ActGroup->addAction(Special);
    ActGroup->setExclusive(true);
    ActGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);

    connect(ActGroup,&QActionGroup::triggered,this,[&](QAction * action){
            List->setVisible(action->isChecked());
    });

    QSettings settings;
    bool darkTheme = settings.value("Application_Settings/Theme").toString() == "darkstyle";


    List = new QListWidget(this);
    List->setViewMode(QListView::IconMode);
    List->setWrapping(true);
    List->setResizeMode(QListView::Adjust);
    List->setIconSize(QSize(20,20));
    List->setGridSize(QSize(30,30));
    List->setStyleSheet("QListView::icon { padding-right: 0px; } QListView { padding: 5px; }");
    List->setVerticalScrollMode(QListWidget::ScrollPerPixel);
    List->setUniformItemSizes(true);
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(List);
    layout->setMargin(0);
    List->setVisible(false);


    MathBar->setIconSize(QSize(16,16));
    QHBoxLayout * hlayout = new QHBoxLayout(this);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(MathBar);
    hlayout->addWidget(List);
    MathBar->setOrientation(Qt::Vertical);

    //#ifndef QT_NO_CLIPBOARD
    //    Copy->setEnabled(false);
    //#endif // !QT_NO_CLIPBOARD

    QStringList Files;
    QDirIterator list(":/images/MathSymbols/" , QStringList() << "*.svg", QDir::Files,QDirIterator::Subdirectories);
    while (list.hasNext()){
        Files.append(list.next());
    }
    Files.sort();
    foreach (QString svgPath, /*Files*/DataTex::SVG_IconPaths) {
        SymbolItem symbol;
        symbol.iconFile = svgPath;
        QFile file(symbol.iconFile);

        QDomDocument IconSVG;
        file.open(QIODevice::ReadOnly);
        IconSVG.setContent(&file);
        file.close();
        QDomElement root = IconSVG.documentElement();
        QDomNodeList nl = root.elementsByTagName("title");
        if (!nl.isEmpty()) {
            QDomNode n = nl.at(0);
            symbol.command =  n.toElement().text();
            qDebug()<< n.toElement().text();
        }
        symbol.category = QFileInfo(symbol.iconFile).absolutePath().split(QDir::separator()).last();
//        qDebug()<<symbol.command;
        QSvgRenderer svgRender(symbol.iconFile);
        QSize svgSize=svgRender.defaultSize()*4;
        QImage img(svgSize.width(), svgSize.height(), QImage::Format_ARGB32);
        QPainter p(&img);
        img.fill(0x000000000);
        svgRender.render(&p);
        if(darkTheme){
            img.invertPixels(QImage::InvertRgb);//dark
        }

        SymbolsInfo.append(symbol.iconFile);
        QListWidgetItem *item = new QListWidgetItem(QIcon(QPixmap::fromImage(img)),"",List);
        List->addItem(item);
        item->setData(Qt::UserRole,QVariant::fromValue(symbol));
    }
//    List->sortItems();
    connect(List,&QListWidget::itemClicked, this,[=](QListWidgetItem * item){
        QString command = item->data(Qt::UserRole).value<SymbolItem>().command;
        emit SendToEditor(command);
    });
}

MathToolBar::~MathToolBar()
{
    delete MathBar;
    delete Operators;
    delete Relations;
    delete Arrows;
    delete Delimiters;
    delete Greek;
    delete Cyrillic;
    delete MiscMath;
    delete MiscText;
    delete FontAwesome;
    delete Special;
}

void MathToolBar::filterItems(QString filter)
{
    for (int row = 0; row < List->count(); ++row)
    {
        List->item(row)->setHidden(!List->item(row)->data(Qt::UserRole).value<SymbolItem>().category.contains(filter, Qt::CaseInsensitive)); // you can leave out the case insensitivity if you do not want it
    }
}

QAction * MathToolBar::CreateNewAction(QAction * Action, std::function<void()> Function, QString ShortCut, QIcon Icon, QString Description)
{
    Action = new QAction(Icon,Description, this);
    Action->setShortcut(QString(ShortCut));
    QObject::connect(Action, &QAction::triggered, this, Function);
    Action->setCheckable(true);
    return Action;
}

PreambleToolBar::PreambleToolBar(QWidget *parent)
    : QWidget{parent}
{
    PreambleBar = new QToolBar;
    PreambleBasics = CreateNewAction(PreambleBasics,[&](){},QIcon(":/images/MathSymbols/Operators/sum.svg"),tr("Save changes"));
    Commands = CreateNewAction(Commands,[](){},QIcon(":/images/leq.svg"),tr("Undo changes"));
    Environments = CreateNewAction(Environments,[&](){},QIcon(":/images/MathSymbols/Arrows/Leftrightarrow.svg"),tr("Copy content"));
    Fonts = CreateNewAction(Fonts,[](){},QIcon(":/images/view-table-of-contents-rtl.svg"),tr("Sectioning"));
    Lists = CreateNewAction(Lists,[](){},QIcon(":/images/format-font-size-less.svg"),tr("Font size"));

    PreambleBar->addAction(PreambleBasics);
    PreambleBar->addAction(Commands);
    PreambleBar->addAction(Environments);
    PreambleBar->addAction(Fonts);
    PreambleBar->addAction(Lists);


    PreambleBar->setIconSize(QSize(16,16));
    QHBoxLayout * hlayout = new QHBoxLayout(this);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->addWidget(PreambleBar);
//    hlayout->addWidget(List);
    PreambleBar->setOrientation(Qt::Horizontal);

}

PreambleToolBar::~PreambleToolBar()
{
    delete PreambleBar;
    delete PreambleBasics;
    delete Commands;
    delete Environments;
    delete Fonts;
    delete Lists;
}

QAction * PreambleToolBar::CreateNewAction(QAction * Action, std::function<void()> Function,QIcon Icon, QString Description)
{
    Action = new QAction(Icon,Description, this);
    QObject::connect(Action, &QAction::triggered, this, Function);
    return Action;
}
