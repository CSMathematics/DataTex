#include "latextoolbar.h"

LatexToolBar::LatexToolBar(QWidget *parent)
    : QWidget{parent}
{
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    ToolBar = new QToolBar;
    ToolsMenu = new QMenu;
    Save = CreateNewAction(Save,[&](){Save->setEnabled(false);},"Ctrl+S",QIcon::fromTheme("DocumentSave"),tr("Save changes"));
    Undo = CreateNewAction(Undo,[](){},"Ctrl+Z",QIcon::fromTheme("EditUndo"),tr("Undo changes"));
    Copy = CreateNewAction(Copy,[](){},"Ctrl+C",QIcon::fromTheme("EditCopy"),tr("Copy content"));
    Part = CreateNewAction(Part,[&](){SectioningMenu();},"",QIcon::fromTheme("ViewTableOfContentsRtl"),tr("Sectioning"));
    FontSize = CreateNewAction(FontSize,[&](){FontSizeMenu();},"",QIcon::fromTheme("FormatFontSizeLess"),tr("Font size"));
    Math = CreateNewAction(Math,[&](){MathMenu();},"Ctrl+M",QIcon::fromTheme("FormatTextSymbol"),tr("Math"));
    Environments = CreateNewAction(Environments,[](){},"Ctrl+E",QIcon::fromTheme("Environment"),tr("Environments"));
    ListEnvironments = CreateNewAction(ListEnvironments,[](){},"Ctrl+L",QIcon::fromTheme("SortPresence"),tr("Lists"));
    Tabular = CreateNewAction(Tabular,[](){},"Ctrl+T",QIcon::fromTheme("ViewCalendarAgenda"),tr("Tables"));
    FontStyle = CreateNewAction(FontStyle,[&](){FontStyleMenu();},"Ctrl+F",QIcon::fromTheme("FormatTextBold"),tr("Font style"));
    Brackets = CreateNewAction(Brackets,[&](){FontStyleMenu();},"Ctrl+Shift+B",QIcon::fromTheme("parenthesis"),tr("Brackets"));

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
                          QIcon::fromTheme("Part"),tr("Part"));
    QAction* ChapterAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\chapter{<text>}");},"",
        QIcon::fromTheme("Chapter"),tr("Chapter"));
    QAction* SectionAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\section{<text>}");},"",
        QIcon::fromTheme("Section"),tr("Section"));
    QAction* SubSectionAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\subsection{<text>}");},"",
        QIcon::fromTheme("SubSection"),tr("Subsection"));
    QAction* SubsubSectionAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\subsubsection{<text>}");},"",
        QIcon::fromTheme("SubsubSection"),tr("Subsubsection"));
    QAction* ParagraphAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\paragraph{<text>}");},"",
        QIcon::fromTheme("Paragraph"),tr("Paragraph"));
    QAction* SubParagraphAction = CreateNewAction(PartAction,[&](){emit SendToEditor("\\subparagraph{<text>}");},"",
        QIcon::fromTheme("SubParagraph"),tr("Subparagraph"));
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
        QIcon::fromTheme("inlinemath"),tr("Inline mode $  $"));
    QAction* DisplayMathAction = CreateNewAction(DisplayMathAction,[&](){emit SendToEditor("\\[  \\]");},"",
        QIcon::fromTheme("displaymath"),tr("Display mode \\[  \\]"));
    QAction* SubscriptAction = CreateNewAction(SubscriptAction,[&](){emit SendToEditor("_{}");},"",
        QIcon::fromTheme("subscript"),tr("Subscript _\{\}"));
    QAction* SuperscriptAction = CreateNewAction(SuperscriptAction,[&](){emit SendToEditor("^{}");},"",
        QIcon::fromTheme("superscript"),tr("Superscript"));
    QAction* FracAction = CreateNewAction(FracAction,[&](){emit SendToEditor("\\frac{<num>}{<den>}");},"",
        QIcon::fromTheme("frac"),tr("Fraction"));
    QAction* DFracAction = CreateNewAction(DFracAction,[&](){emit SendToEditor("\\dfrac{<num>}{<den>}");},"",
        QIcon::fromTheme("dfrac"),tr("Display fraction"));
    QAction* SqrtAction = CreateNewAction(SqrtAction,[&](){emit SendToEditor("\\sqrt{<text>}");},"",
        QIcon::fromTheme("sqrt"),tr("Square root"));

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
        QIcon::fromTheme("mathrm"),tr("Roman"));
    QAction* MathBold = CreateNewAction(MathBold,[&](){emit SendToEditor("\\mathbf{}");},"",
        QIcon::fromTheme("mathbf"),tr("Bold"));
    QAction* MathItalic = CreateNewAction(MathItalic,[&](){emit SendToEditor("\\mathit{}");},"",
        QIcon::fromTheme("mathit"),tr("Italic"));
    QAction* MathSansSerif = CreateNewAction(MathSansSerif,[&](){emit SendToEditor("\\mathsf{}");},"",
        QIcon::fromTheme("mathsf"),tr("Sans Serif"));
    QAction* MathTypeWriter = CreateNewAction(MathTypeWriter,[&](){emit SendToEditor("\\mathtt{}");},"",
        QIcon::fromTheme("mathtt"),tr("Typewriter"));
    QAction* MathCal = CreateNewAction(MathCal,[&](){emit SendToEditor("\\mathcal{}");},"",
        QIcon::fromTheme("mathcal"),tr("Caligraphic"));
    QAction* MathBB = CreateNewAction(MathBB,[&](){emit SendToEditor("\\mathbb{}");},"",
        QIcon::fromTheme("mathbb"),tr("Blackboard Bold"));
    QAction* MathFraktur = CreateNewAction(MathFraktur,[&](){emit SendToEditor("\\mathfrak{}");},"",
        QIcon::fromTheme("mathfrak"),tr("Fraktur"));
    QAction* MathScript = CreateNewAction(MathScript,[&](){emit SendToEditor("\\mathfrak{}");},"",
        QIcon::fromTheme("mathscr"),tr("Script"));

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
        QIcon::fromTheme("format-text-bold"),tr("Bold"));
    QAction* Italic = CreateNewAction(Italic,[&](){emit SendToEditor("\\textit{<text>}");},"",
        QIcon::fromTheme("format-text-italic"),tr("Italic"));
    QAction* Underline = CreateNewAction(Underline,[&](){emit SendToEditor("\\underline{<text>}");},"",
        QIcon::fromTheme("format-text-underline"),tr("Underline"));
    QAction* Emphasis = CreateNewAction(Emphasis,[&](){emit SendToEditor("\\emph{<text>}");},"",
        QIcon::fromTheme("emph"),tr("Emphasis"));
    QAction* Slanted = CreateNewAction(Slanted,[&](){emit SendToEditor("\\textsl{<text>}");},"",
        QIcon::fromTheme("textsl"),tr("Slanted"));
    QAction* SmallCaps = CreateNewAction(SmallCaps,[&](){emit SendToEditor("\\textsc{<text>}");},"",
        QIcon::fromTheme("textsc"),tr("Small caps"));
    QAction* TypeWriter = CreateNewAction(TypeWriter,[&](){emit SendToEditor("\\texttt{<text>}");},"",
        QIcon::fromTheme("texttt"),tr("Typewriter"));
    QAction* SansSerif = CreateNewAction(SansSerif,[&](){emit SendToEditor("\\textsf{<text>}");},"",
        QIcon::fromTheme("textsf"),tr("Sans serif"));

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
        },"Ctrl+S",QIcon::fromTheme("Sum"),tr("Save changes"));
    Relations = CreateNewAction(Relations,[&](){filterItems("Relations");},"Ctrl+Z",QIcon::fromTheme("Leq"),tr("Relations"));
    Arrows = CreateNewAction(Arrows,[&](){filterItems("Arrows");},"Ctrl+C",QIcon::fromTheme("Arrows"),tr("Arrows"));
    Delimiters = CreateNewAction(Delimiters,[&](){filterItems("Delimiters");},"",QIcon::fromTheme("Delimiters"),tr("Delimiters"));
    Greek = CreateNewAction(Greek,[&](){filterItems("Greek");},"",QIcon::fromTheme("FormatTextSymbol"),tr("Greek"));
    Cyrillic = CreateNewAction(Cyrillic,[&](){filterItems("Cyrillic");},"Ctrl+M",QIcon::fromTheme("Cyrillic"),tr("Cyrillic"));
    MiscMath = CreateNewAction(MiscMath,[&](){filterItems("MiscMath");},"Ctrl+E",QIcon::fromTheme("Miscmath"),tr("MiscMath"));
    MiscText= CreateNewAction(MiscText,[&](){filterItems("MiscText");},"Ctrl+L",QIcon::fromTheme("MiscText"),tr("MiscText"));
    FontAwesome = CreateNewAction(FontAwesome,[&](){filterItems("FontAwesome");},"Ctrl+T",QIcon::fromTheme("FontAwesome"),tr("FontAwesome"));
    Special = CreateNewAction(Special,[&](){filterItems("Special");},"Ctrl+T",QIcon::fromTheme("Special"),tr("Special+"));

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

//    QStringList Files;
//    QDirIterator list(":/images/MathSymbols/" , QStringList() << "*.svg", QDir::Files,QDirIterator::Subdirectories);
//    while (list.hasNext()){
//        Files.append(list.next());
//    }
//    Files.sort();
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
//            qDebug()<< n.toElement().text();
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
    PreambleBasics = CreateNewAction(PreambleBasics,[&](){},QIcon::fromTheme("Sum"),tr("Save changes"));
    Commands = CreateNewAction(Commands,[](){},QIcon::fromTheme("Leq"),tr("Undo changes"));
    Environments = CreateNewAction(Environments,[&](){},QIcon::fromTheme("Arrows"),tr("Copy content"));
    Fonts = CreateNewAction(Fonts,[](){},QIcon::fromTheme("ViewTableOfContentsRtl"),tr("Sectioning"));
    Lists = CreateNewAction(Lists,[](){},QIcon::fromTheme("FormatFontSizeLess"),tr("Font size"));

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
