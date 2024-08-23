/***************************************************************************
 *   copyright       : (C) 2003-2022 by Pascal Brachet                     *
 *   https://www.xm1math.net/texmaker/                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   @license GPL-2.0+ <https://spdx.org/licenses/GPL-2.0+.html>           *
 ***************************************************************************/


#include <QtGui>
#include <QRegularExpression>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QTextCodec>
#else
#include <QtCore5Compat/QTextCodec>
#endif

#include "latexhighlighter.h"
#include "blockdata.h"

LatexHighlighter::LatexHighlighter(QTextDocument *parent,bool spelling,QString ignoredWords/*,Hunspell *spellChecker*/)
    : QSyntaxHighlighter(parent)
{
    isGraphic=false;
    QSettings settings;
    bool isThemeDark = settings.value("Application_Settings/IsThemeDark").toBool();
    if(isThemeDark){
        ColorStandard = QColor(242, 242, 242);
        ColorComment = QColor(177, 177, 177);
        ColorMath = QColor(51, 204, 51);
        ColorCommand=QColor(255, 14, 0);
        ColorKeyword=QColor(0, 159, 255);
        ColorVerbatim = QColor(239, 119, 0);
        ColorTodo=QColor(255, 0, 0);
        ColorKeywordGraphic=QColor(126, 206, 255);
        ColorNumberGraphic=QColor(234, 0, 255);
    }
    else{
        ColorStandard = QColor(0,0,0);
        ColorComment = QColor(146, 146, 146);
        ColorMath = QColor(0x00,0x90, 0x00);
        ColorCommand=QColor(0xAA, 0x00, 0x00);
        ColorKeyword=QColor(0x00, 0xAA, 0xCC);
        ColorVerbatim = QColor(154, 77, 0);
        ColorTodo=QColor(0xFF,0x00,0x00);
        ColorKeywordGraphic=QColor(0x00,0x77,0x89);
        ColorNumberGraphic=QColor(0x68,0x08,0x78);
    }
    KeyWords= QString("section{,subsection{,subsubsection{,chapter{,part{,paragraph{,subparagraph{,section*{,subsection*{,subsubsection*{,chapter*{,part*{,paragraph*{,subparagraph*{,label{,includegraphics{,includegraphics[,includegraphics*{,includegraphics*[,include{,input{,begin{,end{").split(",");
    KeyWordsGraphic=QString("void bool bool3 int real pair triple string").split(" ");
    KeyWordsGraphicBis=QString("and controls tension atleast curl if else while for do return break continue struct typedef new access import unravel from include quote static public private restricted this explicit true false null cycle newframe operator").split(" ");
    spellingErrorFormat.setFontUnderline(true);
    spellingErrorFormat.setUnderlineColor(QColor(Qt::red));
    //spellingErrorFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    checkSpelling=spelling;
    //pChecker = spellChecker;
    //if (pChecker) spell_encoding=QString(pChecker->get_dic_encoding());
    if (!ignoredWords.isEmpty()) alwaysignoredwordList=ignoredWords.split(",");
    else alwaysignoredwordList.clear();
    ignoredwordList=alwaysignoredwordList;
    QFile wordsfile(":/spell/spellignore.txt");
    QString line;
    if (wordsfile.open(QFile::ReadOnly))
    {
        while (!wordsfile.atEnd())
        {
            line = wordsfile.readLine();
            if (!line.isEmpty()) hardignoredwordList.append(line.trimmed());
        }
    }
    mathFormat.setFontWeight(QFont::Bold);
    mathFormat.setForeground(ColorMath);
    commandFormat.setFontWeight(QFont::Bold);
    commandFormat.setForeground(ColorCommand);
    keywordFormat.setFontWeight(QFont::Bold);
    keywordFormat.setForeground(ColorKeyword);
    inMathFormat.setFontWeight(QFont::Bold);
    inMathFormat.setForeground(QColor("#77b300"));
}

LatexHighlighter::~LatexHighlighter(){
    //delete pChecker;
}

void LatexHighlighter::SetEditor(LatexTextBrowser *ed)
{
    editor=ed;
}

void LatexHighlighter::SetAlwaysIgnoredWords(QString ignoredWords)
{
    if (!ignoredWords.isEmpty()) alwaysignoredwordList=ignoredWords.split(",");
    else alwaysignoredwordList.clear();
    ignoredwordList=alwaysignoredwordList;
}


void LatexHighlighter::setColors(QList<QColor> colors)
{
    ColorStandard=colors.at(0);
    ColorComment=colors.at(1);
    ColorMath=colors.at(2);
    ColorCommand=colors.at(3);
    ColorKeyword=colors.at(4);
    ColorVerbatim=colors.at(5);
    ColorTodo=colors.at(6);
    ColorKeywordGraphic=colors.at(7);
    ColorNumberGraphic=colors.at(8);
    rehighlight();
}

void LatexHighlighter::highlightBlock(const QString &text)
{
    QRegularExpression rxSweave("<<(.*)>>=");
    QStringList types;
    types << QLatin1String("article") << QLatin1String("book")
          << QLatin1String("booklet") << QLatin1String("inbook")
          << QLatin1String("incollection") << QLatin1String("inproceedings")
          << QLatin1String("manual") << QLatin1String("mastersthesis")
          << QLatin1String("misc") << QLatin1String("phdthesis")
          << QLatin1String("proceedings") << QLatin1String("techreport")
          << QLatin1String("unpublished") << QLatin1String("periodical")
          << QLatin1String("conference") << QLatin1String("mvbook")
          << QLatin1String("collection") << QLatin1String("mvcollection")
          << QLatin1String("online") << QLatin1String("mvproceedings")
          << QLatin1String("inproceedings") << QLatin1String("report")
          << QLatin1String("thesis") << QLatin1String("electronic") << QLatin1String("patent");
    QRegularExpression rxBib("@("+types.join("|")+")\\s*\\{\\s*(.*),", QRegularExpression::CaseInsensitiveOption | QRegularExpression::InvertedGreedinessOption);


    const int StateStandard = 0;
    const int StateComment = 1;
    const int StateMath = 2;
    const int StateCommand=3;
    const int StateVerbatim =4;
    const int StateVerbatimCommand =5;
    const int StateSweave =6;
    const int StateSweaveCommand =7;
    const int StateGraphic =8;
    const int StateGraphicCommand =9;
    const int StateGraphicMath =10;
    const int StateBib =11;
    const int StateGraphicComment =12;
    const int StateGraphicAsy =13;
    const int StateGraphicAsyCommand =14;
    const int StateGraphicAsyMath =15;
    const int StateGraphicAsyComment =16;

    int i = 0;
    int state = previousBlockState();
    //int oldstate=state;
    QChar last, next ,ch,tmp, verbflag;
    verbflag=' ';
    QString buffer;


    BlockData *blockData = new BlockData;
    int leftPos = text.indexOf( '{' );
    while ( leftPos != -1 )
    {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = '{';
        info->position = leftPos;

        blockData->insertPar( info );
        leftPos = text.indexOf( '{', leftPos+1 );
    }

    int rightPos = text.indexOf('}');
    while ( rightPos != -1 )
    {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = '}';
        info->position = rightPos;

        blockData->insertPar( info );
        rightPos = text.indexOf( '}', rightPos+1 );
    }

    leftPos = text.indexOf( "\\begin{" );
    while ( leftPos != -1 )
    {
        LatexBlockInfo *info = new LatexBlockInfo;
        info->character = 'b';
        info->position = leftPos;

        blockData->insertLat( info );
        leftPos = text.indexOf("\\begin{", leftPos+1 );
    }

    rightPos = text.indexOf("\\end{");
    while ( rightPos != -1 )
    {
        LatexBlockInfo *info = new LatexBlockInfo;
        info->character = 'e';
        info->position = rightPos;

        blockData->insertLat( info );
        rightPos = text.indexOf("\\end{", rightPos+1 );
    }

    setCurrentBlockUserData(blockData);


/////////////////////

/////////////////
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QRegExp rxverb("verb\\*?([^\\*])");
    QRegExp rxlst("lstinline(.)");
#else
    QRegularExpression rxverb(QRegularExpression::anchoredPattern("verb\\*?([^\\*])"));
    QRegularExpression rxlst(QRegularExpression::anchoredPattern("lstinline(.)"));
#endif
    QTextCharFormat structFormat;
    structFormat.setFontWeight(QFont::Bold);
    structFormat.setForeground(ColorKeyword);

    QTextCharFormat asyFormat;
    asyFormat.setFontWeight(QFont::Bold);
    asyFormat.setForeground(ColorCommand);

    if (!isGraphic)
    {
        if (state<0) state=0;
        i=0;
        int k;
        blockData->code.clear();
        blockData->misspelled.clear();
        for (int j=0; j < text.length(); j++) {blockData->code.append(0);blockData->misspelled.append(false);}
        while (i < text.length())
        {
            ch = text.at( i );
            buffer += ch;
            if ( i < text.length()-1 ) next = text.at( i+1 );
            switch (state) {

            case StateStandard: {
                tmp=text.at( i );
                if (tmp=='\\') {
                    if (next=='[' || next=='(')
                    {
                        setFormat( i, 1,mathFormat );
                        blockData->code[i]=1;
                        state=StateMath;
                        i++;
                        if ( i < text.length())
                        {
                            setFormat( i, 1,mathFormat);
                            blockData->code[i]=1;
                        }
                    }
                    else
                    {
                        setFormat( i, 1,commandFormat );
                        blockData->code[i]=1;
                        state=StateCommand;
                    }
                    buffer = QString();
                } else
                    if (tmp=='$') {
                        setFormat( i, 1,mathFormat);
                        blockData->code[i]=1;
                        state=StateMath;
                        if (next=='$')
                        {
                            i++;
                            if ( i < text.length())
                            {
                                setFormat( i, 1,mathFormat);
                                blockData->code[i]=1;
                            }
                        }
                        buffer = QString();
                    } else
                        if (tmp== '%' ){
                            setFormat( i, 1,ColorComment);
                            state=StateComment;
                            blockData->code[i]=1;
                            buffer = QString();
                        } else
                            if (tmp== '{' ){
                                blockData->code[i]=1;
                                setFormat( i, 1,ColorStandard);
                                state=StateStandard;
                            } else
                                if (tmp== '}' ){
                                    blockData->code[i]=1;
                                    setFormat( i, 1,ColorStandard);
                                    state=StateStandard;

                                    int poslab=buffer.indexOf("label{");
                                    if (poslab!=-1)
                                    {
                                        for (k=i-buffer.length()+poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                    }
                                    poslab=buffer.indexOf("cite{");
                                    if (poslab!=-1)
                                    {
                                        for (k=i-buffer.length()+poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                    }
                                    poslab=buffer.indexOf("begin{verbatim}");
                                    if(poslab != -1) {state=StateVerbatim;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{verbatim*}");
                                    if(poslab != -1) {state=StateVerbatim;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{lstlisting}");
                                    if(poslab != -1) {state=StateVerbatim;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{gnuplot}");
                                    if(poslab != -1) {state=StateVerbatim;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{asy}");
                                    if(poslab != -1) {state=StateGraphicAsy;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{tikzpicture}");
                                    if(poslab != -1) {state=StateGraphic;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{pspicture}");
                                    if(poslab != -1) {state=StateGraphic;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{pspicture*}");
                                    if(poslab != -1) {state=StateGraphic;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{equation}") ;
                                    if(poslab != -1) {state=StateMath;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{equation*}") ;
                                    if(poslab != -1) {state=StateMath;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{align}");
                                    if(poslab != -1) {state=StateMath;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{align*}");
                                    if(poslab != -1) {state=StateMath;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{multline}");
                                    if(poslab != -1) {state=StateMath;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{multline*}");
                                    if(poslab != -1) {state=StateMath;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{gather}") ;
                                    if(poslab != -1) {state=StateMath;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    poslab=buffer.indexOf("begin{gather*}") ;
                                    if(poslab != -1) {state=StateMath;for (k=poslab; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}}
                                    buffer = QString();
                                } else
                                    if (tmp== '<' ){
                                        blockData->code[i]=1;
                                        setFormat( i, 1,ColorStandard);
                                        state=StateStandard;
                                    } else
                                        if (tmp== '>' ){
                                            blockData->code[i]=1;
                                            setFormat( i, 1,ColorStandard);
                                            state=StateStandard;
                                        } else
                                            if (tmp== ',' ){
                                                blockData->code[i]=1;
                                                setFormat( i, 1,ColorStandard);
                                                state=StateStandard;
                                                int posbib=buffer.indexOf(rxBib);
                                                if( posbib!= -1)
                                                {
                                                    int posbib2=buffer.lastIndexOf("{");
                                                    if ((posbib2 !=-1) && (posbib2>posbib)) setFormat(posbib,posbib2-posbib,structFormat);
                                                    for (k=posbib; k <i ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                                    state=StateBib;
                                                    buffer = QString();
                                                }
                                            } else
                                                if (tmp== '=' ){
                                                    blockData->code[i]=1;
                                                    setFormat( i, 1,ColorStandard);
                                                    if(buffer.indexOf(rxSweave) != -1) {state=StateSweave;buffer = QString();}
                                                } else
                                                    if (tmp== '(' ){
                                                        blockData->code[i]=1;
                                                        setFormat( i, 1,ColorStandard);
                                                        state=StateStandard;
                                                        buffer = QString();
                                                    } else
                                                        if (tmp== ')' ){
                                                            blockData->code[i]=1;
                                                            setFormat( i, 1,ColorStandard);
                                                            state=StateStandard;
                                                            buffer = QString();
                                                        } else
                                                            if (isWordSeparator(tmp)){
                                                                blockData->code[i]=1;
                                                                setFormat( i, 1,ColorStandard);
                                                                //buffer = QString();
                                                            } else
                                                            {
                                                                setFormat( i, 1,ColorStandard);
                                                                state=StateStandard;
                                                                //buffer = QString();
                                                            }
                //	buffer = QString();
            } break;

            case StateComment: {
                setFormat( i, 1,ColorComment);
                blockData->code[i]=1;
                state=StateComment;
                buffer = QString();
            } break;

            case StateMath: {
                tmp=text.at( i );
                if (tmp== '$') {
                    setFormat( i, 1,mathFormat);
                    blockData->code[i]=1;
                    state=StateStandard;
                    if (next=='$')
                    {
                        i++;
                        if ( i < text.length())
                        {
                            setFormat( i, 1,mathFormat);
                            blockData->code[i]=1;
                        }
                    }
                    buffer = QString();
                } else if (tmp== '\\') {
                    if (next==']' || next==')')
                    {
                        setFormat( i, 1,mathFormat);
                        blockData->code[i]=1;
                        state=StateStandard;
                        i++;
                        if ( i < text.length())
                        {
                            setFormat( i, 1,mathFormat);
                            blockData->code[i]=1;
                        }
                        buffer = QString();
                    }
                    else if (next=='$')
                    {
                        setFormat( i, 1,mathFormat);
                        blockData->code[i]=1;
                        state=StateMath;
                        i++;
                        if ( i < text.length())
                        {
                            setFormat( i, 1,mathFormat/*QColor("#838307")*/);
                            blockData->code[i]=1;
                        }
                    }
                    else
                    {
                        setFormat( i, 1,inMathFormat);
                        blockData->code[i]=1;
                        state=StateMath;
                    }
                } else
                    if (tmp== '{' ){
                        setFormat( i, 1,inMathFormat);
                        blockData->code[i]=1;
                        state=StateMath;
                    } else
                        if (tmp== '}' ){
                            setFormat( i, 1,inMathFormat);
                            blockData->code[i]=1;
                            state=StateMath;
                            int pos=buffer.indexOf("\\end{equation}");
                            if( pos!= -1)
                            {

                                state=StateStandard;
                                setFormat(pos,4,keywordFormat);
                                setFormat(pos+4,10,ColorStandard);
                                for (k=i-13; k <i-9 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                            }
                            pos=buffer.indexOf("\\end{equation*}");
                            if( pos!= -1)
                            {
                                state=StateStandard;
                                setFormat(pos,4,keywordFormat);
                                setFormat(pos+4,11,ColorStandard);
                                for (k=i-14; k <i-10 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                            }
                            pos=buffer.indexOf("\\end{align}");
                            if( pos!= -1)
                            {
                                state=StateStandard;
                                setFormat(pos,4,keywordFormat);
                                setFormat(pos+4,7,ColorStandard);
                                for (k=i-10; k <i-6 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                            }
                            pos=buffer.indexOf("\\end{align*}");
                            if( pos!= -1)
                            {
                                state=StateStandard;
                                setFormat(pos,4,keywordFormat);
                                setFormat(pos+4,8,ColorStandard);
                                for (k=i-11; k <i-7 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                            }
                            pos=buffer.indexOf("\\end{multline}");
                            if( pos!= -1)
                            {
                                state=StateStandard;
                                setFormat(pos,4,keywordFormat);
                                setFormat(pos+4,10,ColorStandard);
                                for (k=i-13; k <i-9 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                            }
                            pos=buffer.indexOf("\\end{multline*}");
                            if( pos!= -1)
                            {
                                state=StateStandard;
                                setFormat(pos,4,keywordFormat);
                                setFormat(pos+4,11,ColorStandard);
                                for (k=i-14; k <i-10 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                            }
                            pos=buffer.indexOf("\\end{gather}");
                            if( pos!= -1)
                            {
                                state=StateStandard;
                                setFormat(pos,4,keywordFormat);
                                setFormat(pos+4,8,ColorStandard);
                                for (k=i-11; k <i-7 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                            }
                            pos=buffer.indexOf("\\end{gather*}");
                            if( pos!= -1)
                            {
                                state=StateStandard;
                                setFormat(pos,4,keywordFormat);
                                setFormat(pos+4,9,ColorStandard);
                                for (k=i-12; k <i-8 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                            }
                            buffer = QString();
                        } else
                        {
                            setFormat( i, 1,inMathFormat);
                            blockData->code[i]=1;
                            state=StateMath;
                        }

            } break;
            case StateGraphicMath: {
                tmp=text.at( i );
                if (tmp== '$') {
                    setFormat( i, 1,mathFormat);
                    blockData->code[i]=1;
                    state=StateGraphic;
                    if (next=='$')
                    {
                        i++;
                        if ( i < text.length())
                        {
                            setFormat( i, 1,mathFormat);
                            blockData->code[i]=1;
                        }
                    }
                } else if (tmp== '\\') {
                    if (next==']')
                    {
                        setFormat( i, 1,mathFormat);
                        blockData->code[i]=1;
                        state=StateGraphic;
                        i++;
                        if ( i < text.length())
                        {
                            setFormat( i, 1,mathFormat);
                            blockData->code[i]=1;
                        }
                    }
                    else
                    {
                        setFormat( i, 1,mathFormat);
                        blockData->code[i]=1;
                        state=StateGraphicMath;
                    }
                } else
                {
                    setFormat( i, 1,mathFormat);
                    blockData->code[i]=1;
                    state=StateGraphicMath;
                }
                buffer = QString();
            } break;
            case StateGraphicAsyMath: {
                tmp=text.at( i );
                if (tmp== '"') {
                    setFormat( i, 1,mathFormat);
                    blockData->code[i]=1;
                    state=StateGraphicAsy;
                    // 		if (tmp== '$') {
                    // 			setFormat( i, 1,mathFormat);
                    // 			blockData->code[i]=1;
                    // 			state=StateGraphicAsy;
                    // 			if (next=='$')
                    // 				{
                    // 				i++;
                    // 				if ( i < text.length())
                    // 					{
                    // 					setFormat( i, 1,mathFormat);
                    // 					blockData->code[i]=1;
                    // 					}
                    // 				}
                } else if (tmp== '\\') {
                    if (next==']')
                    {
                        setFormat( i, 1,mathFormat);
                        blockData->code[i]=1;
                        state=StateGraphicAsy;
                        i++;
                        if ( i < text.length())
                        {
                            setFormat( i, 1,mathFormat);
                            blockData->code[i]=1;
                        }
                    }
                    else
                    {
                        setFormat( i, 1,mathFormat);
                        blockData->code[i]=1;
                        state=StateGraphicAsyMath;
                    }
                } else
                {
                    setFormat( i, 1,mathFormat);
                    blockData->code[i]=1;
                    state=StateGraphicAsyMath;
                }
                buffer = QString();
            } break;
            case StateCommand:{
                tmp=text.at( i );
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                if (rxverb.exactMatch(buffer))
#else
                QRegularExpressionMatch rxverbMatch=rxverb.match(buffer);
                QRegularExpressionMatch rxlstMatch=rxlst.match(buffer);
                if (rxverbMatch.hasMatch())
#endif
                {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                    verbflag=rxverb.cap(1).at(0);
#else
                    verbflag=rxverbMatch.captured(1).at(0);
#endif
                    blockData->code[i]=1;
                    setFormat( i, 1,commandFormat );
                    state=StateVerbatim;
                } else
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                    if (rxlst.exactMatch(buffer))
#else
                    if (rxlstMatch.hasMatch())
#endif
                    {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                        if (rxlst.exactMatch(buffer))
#else
                        verbflag=rxlstMatch.captured(1).at(0);
#endif
                            blockData->code[i]=1;
                        setFormat( i, 1,commandFormat );
                        state=StateVerbatim;
                    } else
                        if (tmp=='$') {
                            if (last=='\\')
                            {
                                setFormat( i, 1,commandFormat );
                                blockData->code[i]=1;
                                state=StateStandard;
                            }
                            else
                            {
                                setFormat( i, 1,mathFormat);
                                blockData->code[i]=1;
                                state=StateMath;
                            }
                        } else
                            if (tmp=='%') {
                                if (last=='\\')
                                {
                                    setFormat( i, 1,ColorStandard);
                                    state=StateStandard;
                                }
                                else
                                {
                                    setFormat( i, 1,ColorComment);
                                    blockData->code[i]=1;
                                    state=StateComment;
                                }
                            } else
                                if (tmp== ' ') {
                                    setFormat( i, 1,ColorStandard);
                                    state=StateStandard;
                                } else
                                    if (tmp== '\t') {
                                        setFormat( i, 1,ColorStandard);
                                        state=StateStandard;
                                    }  else
                                        if (tmp=='(' || tmp=='[' || tmp=='{' || tmp==')' || tmp==']' || tmp=='}') {
                                            blockData->code[i]=1;
                                            setFormat( i, 1,ColorStandard);
                                            state=StateStandard;
                                            if ( buffer.length() > 0 )
                                            {
                                                for ( QStringList::Iterator it = KeyWords.begin(); it != KeyWords.end(); ++it )
                                                {
                                                    if (( *it ).indexOf( buffer )!=-1)
                                                    {
                                                        if (*it!="begin{" && *it!="end{") setFormat( i - buffer.length(), buffer.length(),structFormat);
                                                        else setFormat( i - buffer.length(), buffer.length(),keywordFormat);
                                                        blockData->code[i]=1;
                                                    }
                                                }
                                            }
                                        } else
                                            if (tmp=='\\' || tmp==',' || tmp==';' /*|| tmp=='\''*/ || tmp=='\"' || tmp=='`' || tmp=='^' || tmp=='~') {
                                                blockData->code[i]=1;
                                                if (last=='\\')
                                                {
                                                    setFormat( i, 1,commandFormat );
                                                    blockData->code[i]=1;
                                                    state=StateStandard;
                                                }
                                                else
                                                {
                                                    setFormat( i, 1,commandFormat );
                                                    blockData->code[i]=1;
                                                    state=StateCommand;
                                                }
                                            } else
                                            {
                                                setFormat( i, 1,commandFormat );
                                                blockData->code[i]=1;
                                                state=StateCommand;
                                            }
            } break;
            case StateVerbatim: {
                tmp=text.at( i );
                if (tmp==verbflag && verbflag!=' ')
                {
                    setFormat( i, 1,commandFormat );
                    blockData->code[i]=1;
                    state=StateStandard;
                    verbflag=' ';
                } else
                    if (tmp=='\\') {
                        if (next=='[')
                        {
                            setFormat( i, 1,ColorVerbatim );
                            blockData->code[i]=1;
                            i++;
                            if ( i < text.length())
                            {
                                setFormat( i, 1,ColorVerbatim);
                                blockData->code[i]=1;
                            }
                        }
                        else
                        {
                            setFormat( i, 1,ColorVerbatim );
                            blockData->code[i]=1;
                            state=StateVerbatimCommand;
                        }
                    } else
                        if (tmp=='$') {
                            setFormat( i, 1,ColorVerbatim);
                            blockData->code[i]=1;
                            if (next=='$')
                            {
                                i++;
                                if ( i < text.length())
                                {
                                    setFormat( i, 1,ColorVerbatim);
                                    blockData->code[i]=1;
                                }
                            }
                            buffer = QString();
                        } else
                            if (tmp== '%' ){
                                setFormat( i, 1,ColorVerbatim);
                                blockData->code[i]=1;
                                buffer = QString();
                            } else
                                if (tmp== '{' ){
                                    blockData->code[i]=1;
                                    setFormat( i, 1,ColorVerbatim);
                                } else
                                    if (tmp== '}' ){
                                        blockData->code[i]=1;
                                        setFormat( i, 1,ColorVerbatim);
                                        state=StateVerbatim;
                                        int pos=buffer.indexOf("\\end{verbatim}");
                                        if( pos!= -1)
                                        {
                                            state=StateStandard;
                                            setFormat(pos,4,keywordFormat);
                                            setFormat(pos+4,10,ColorStandard);
                                            for (k=i-13; k <i-9 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                        }
                                        pos=buffer.indexOf("\\end{verbatim*}");
                                        if( pos!= -1)
                                        {
                                            state=StateStandard;
                                            setFormat(pos,4,keywordFormat);
                                            setFormat(pos+4,11,ColorStandard);
                                            for (k=i-14; k <i-10 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                        }
                                        pos=buffer.indexOf("\\end{lstlisting}");
                                        if( pos!= -1)
                                        {
                                            state=StateStandard;
                                            setFormat(pos,4,keywordFormat);
                                            setFormat(pos+4,12,ColorStandard);
                                            for (k=i-15; k <i-11 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                        }
                                        pos=buffer.indexOf("\\end{gnuplot}");
                                        if( pos!= -1)
                                        {
                                            state=StateStandard;
                                            setFormat(pos,4,keywordFormat);
                                            setFormat(pos+4,9,ColorStandard);
                                            for (k=i-12; k <i-8 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                        }
                                        buffer = QString();
                                    } else
                                        if (tmp== '(' ){
                                            blockData->code[i]=1;
                                            setFormat( i, 1,ColorVerbatim);
                                            buffer = QString();
                                        } else
                                            if (tmp== ')' ){
                                                blockData->code[i]=1;
                                                setFormat( i, 1,ColorVerbatim);
                                                buffer = QString();
                                            } else
                                                if (isWordSeparator(tmp)){
                                                    blockData->code[i]=1;
                                                    setFormat( i, 1,ColorVerbatim);
                                                    buffer = QString();
                                                } else
                                                {
                                                    setFormat( i, 1,ColorVerbatim);
                                                    //buffer = QString();
                                                }
            } break;
            case StateVerbatimCommand:{
                tmp=text.at( i );
                if (tmp==verbflag && verbflag!=' ')
                {
                    setFormat( i, 1,commandFormat );
                    blockData->code[i]=1;
                    state=StateStandard;
                    verbflag=' ';
                } else
                    if (tmp=='$') {
                        setFormat( i, 1,ColorVerbatim);
                        blockData->code[i]=1;
                        state=StateVerbatim;
                    } else
                        if (tmp=='%') {
                            if (last=='\\')
                            {
                                setFormat( i, 1,ColorVerbatim);
                                state=StateVerbatim;
                            }
                            else
                            {
                                setFormat( i, 1,ColorVerbatim);
                                blockData->code[i]=1;
                                state=StateVerbatim;
                            }
                        } else
                            if (tmp== ' ') {
                                setFormat( i, 1,ColorVerbatim);
                                state=StateVerbatim;
                            }  else
                                if (tmp=='(' || tmp=='[' || tmp=='{' || tmp==')' || tmp==']' || tmp=='}') {
                                    blockData->code[i]=1;
                                    setFormat( i, 1,ColorVerbatim);
                                    state=StateVerbatim;
                                } else
                                    if (tmp=='\\' || tmp==',' || tmp==';' || /*tmp=='\'' ||*/ tmp=='\"' || tmp=='`' || tmp=='^' || tmp=='~') {
                                        blockData->code[i]=1;
                                        setFormat( i, 1,ColorVerbatim);
                                        state=StateVerbatim;
                                    } else
                                    {
                                        setFormat( i, 1,ColorVerbatim);
                                        blockData->code[i]=1;
                                        state=StateVerbatimCommand;
                                    }
            } break;
            case StateGraphic: {
                tmp=text.at( i );
                if (tmp=='\\') {
                    if (next=='[')
                    {
                        setFormat( i, 1,ColorVerbatim );
                        blockData->code[i]=1;
                        i++;
                        if ( i < text.length())
                        {
                            setFormat( i, 1,ColorVerbatim);
                            blockData->code[i]=1;
                        }
                    }
                    else
                    {
                        setFormat( i, 1,ColorVerbatim );
                        blockData->code[i]=1;
                        state=StateGraphicCommand;
                    }
                } else
                    if (tmp=='$') {
                        setFormat( i, 1,mathFormat);
                        blockData->code[i]=1;
                        state=StateGraphicMath;
                        if (next=='$')
                        {
                            i++;
                            if ( i < text.length())
                            {
                                setFormat( i, 1,mathFormat);
                                blockData->code[i]=1;
                            }
                        }
                        buffer = QString();
                    } else
                        if (tmp== '/' && next=='/'){
                            setFormat( i, 1,ColorComment);
                            state=StateGraphicComment;
                            blockData->code[i]=1;
                            buffer = QString();
                        } else
                            if (tmp== '%'){
                                setFormat( i, 1,ColorComment);
                                state=StateGraphicComment;
                                //setFormat( i, 1,ColorVerbatim);
                                blockData->code[i]=1;
                                buffer = QString();
                            } else
                                if (tmp== '{' ){
                                    blockData->code[i]=1;
                                    setFormat( i, 1,ColorVerbatim);
                                } else
                                    if (tmp== '}' ){
                                        blockData->code[i]=1;
                                        setFormat( i, 1,ColorVerbatim);
                                        state=StateGraphic;
                                        int pos=buffer.indexOf("\\end{asy}");
                                        if( pos!= -1)
                                        {
                                            state=StateStandard;
                                            setFormat(pos,4,keywordFormat);
                                            setFormat(pos+4,5,ColorStandard);
                                            for (k=i-8; k <i-4 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                        }
                                        pos=buffer.indexOf("\\end{tikzpicture}");
                                        if( pos!= -1)
                                        {
                                            state=StateStandard;
                                            setFormat(pos,4,keywordFormat);
                                            setFormat(pos+4,13,ColorStandard);
                                            for (k=i-16; k <i-12 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                        }
                                        pos=buffer.indexOf("\\end{pspicture}");
                                        if( pos!= -1)
                                        {
                                            state=StateStandard;
                                            setFormat(pos,4,keywordFormat);
                                            setFormat(pos+4,11,ColorStandard);
                                            for (k=i-14; k <i-10 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                        }
                                        pos=buffer.indexOf("\\end{pspicture*}");
                                        if( pos!= -1)
                                        {
                                            state=StateStandard;
                                            setFormat(pos,4,keywordFormat);
                                            setFormat(pos+4,12,ColorStandard);
                                            for (k=i-15; k <i-11 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                        }
                                        buffer = QString();
                                    } else
                                        if (tmp== '(' ){
                                            blockData->code[i]=1;
                                            setFormat( i, 1,ColorVerbatim);
                                            buffer = QString();
                                        } else
                                            if (tmp== ')' ){
                                                blockData->code[i]=1;
                                                setFormat( i, 1,ColorVerbatim);
                                                buffer = QString();
                                            } else
                                                if (isWordSeparator(tmp) && tmp!='"' && tmp!='.' && tmp!='-'){
                                                    blockData->code[i]=1;
                                                    setFormat( i, 1,ColorVerbatim);
                                                    buffer = QString();
                                                } else
                                                {
                                                    setFormat( i, 1,ColorVerbatim);
                                                    //buffer = QString();
                                                }
            } break;
            case StateGraphicAsy: {
                tmp=text.at( i );
                if (tmp=='\\') {
                    if (next=='[')
                    {
                        setFormat( i, 1,ColorVerbatim );
                        blockData->code[i]=1;
                        i++;
                        if ( i < text.length())
                        {
                            setFormat( i, 1,ColorVerbatim);
                            blockData->code[i]=1;
                        }
                    }
                    else
                    {
                        setFormat( i, 1,ColorVerbatim );
                        blockData->code[i]=1;
                        state=StateGraphicAsyCommand;
                    }
                } else
                    if (tmp=='"') {
                        setFormat( i, 1,mathFormat);
                        blockData->code[i]=1;
                        state=StateGraphicAsyMath;
                        buffer = QString();
                        // 		if (tmp=='$') {
                        // 			setFormat( i, 1,mathFormat);
                        // 			blockData->code[i]=1;
                        // 			state=StateGraphicAsyMath;
                        // 			if (next=='$')
                        // 				{
                        // 				i++;
                        // 				if ( i < text.length())
                        // 					{
                        // 					setFormat( i, 1,mathFormat);
                        // 					blockData->code[i]=1;
                        // 					}
                        // 				}
                        // 			buffer = QString();
                    } else
                        if (tmp== '/' && next=='/'){
                            setFormat( i, 1,ColorComment);
                            state=StateGraphicAsyComment;
                            blockData->code[i]=1;
                            buffer = QString();
                        } else
                            if (tmp== '{' ){
                                blockData->code[i]=1;
                                setFormat( i, 1,ColorVerbatim);
                            } else
                                if (tmp== '}' ){
                                    blockData->code[i]=1;
                                    setFormat( i, 1,ColorVerbatim);
                                    state=StateGraphicAsy;
                                    int pos=buffer.indexOf("\\end{asy}");
                                    if( pos!= -1)
                                    {
                                        state=StateStandard;
                                        setFormat(pos,4,keywordFormat);
                                        setFormat(pos+4,5,ColorStandard);
                                        for (k=i-8; k <i-4 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                    }
                                    pos=buffer.indexOf("\\end{tikzpicture}");
                                    if( pos!= -1)
                                    {
                                        state=StateStandard;
                                        setFormat(pos,4,keywordFormat);
                                        setFormat(pos+4,13,ColorStandard);
                                        for (k=i-16; k <i-12 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                    }
                                    pos=buffer.indexOf("\\end{pspicture}");
                                    if( pos!= -1)
                                    {
                                        state=StateStandard;
                                        setFormat(pos,4,keywordFormat);
                                        setFormat(pos+4,11,ColorStandard);
                                        for (k=i-14; k <i-10 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                    }
                                    pos=buffer.indexOf("\\end{pspicture*}");
                                    if( pos!= -1)
                                    {
                                        state=StateStandard;
                                        setFormat(pos,4,keywordFormat);
                                        setFormat(pos+4,12,ColorStandard);
                                        for (k=i-15; k <i-11 ; k++) {if (k>0 && k<text.length()) blockData->code[k]=1;}
                                    }
                                    buffer = QString();
                                } else
                                    if (tmp== '(' ){
                                        blockData->code[i]=1;
                                        setFormat( i, 1,ColorVerbatim);
                                        buffer = QString();
                                    } else
                                        if (tmp== ')' ){
                                            blockData->code[i]=1;
                                            setFormat( i, 1,ColorVerbatim);
                                            buffer = QString();
                                        } else
                                            if (isWordSeparator(tmp) && tmp!='"' && tmp!='.' && tmp!='-'){
                                                blockData->code[i]=1;
                                                setFormat( i, 1,ColorVerbatim);
                                                buffer = QString();
                                            } else
                                            {
                                                setFormat( i, 1,ColorVerbatim);
                                                //buffer = QString();
                                            }
            } break;
            case StateGraphicComment: {
                setFormat( i, 1,ColorComment);
                blockData->code[i]=1;
                state=StateGraphicComment;
                buffer = QString();
            } break;
            case StateGraphicAsyComment: {
                setFormat( i, 1,ColorComment);
                blockData->code[i]=1;
                state=StateGraphicAsyComment;
                buffer = QString();
            } break;
            case StateGraphicCommand:{
                tmp=text.at( i );
                if (tmp=='$') {
                    setFormat( i, 1,ColorVerbatim);
                    blockData->code[i]=1;
                    state=StateGraphic;
                } else
                    if (tmp=='%') {
                        if (last=='\\')
                        {
                            setFormat( i, 1,ColorVerbatim);
                            state=StateGraphic;
                        }
                        else
                        {
                            setFormat( i, 1,ColorVerbatim);
                            blockData->code[i]=1;
                            state=StateGraphic;
                        }
                    } else
                        if (tmp== ' ') {
                            setFormat( i, 1,ColorVerbatim);
                            state=StateGraphic;
                        }  else
                            if (tmp=='(' || tmp=='[' || tmp=='{' || tmp==')' || tmp==']' || tmp=='}') {
                                blockData->code[i]=1;
                                setFormat( i, 1,ColorVerbatim);
                                state=StateGraphic;
                            } else
                                if (tmp=='\\' || tmp==',' || tmp==';' || /*tmp=='\'' ||*/ tmp=='\"' || tmp=='`' || tmp=='^' || tmp=='~') {
                                    blockData->code[i]=1;
                                    setFormat( i, 1,ColorVerbatim);
                                    state=StateGraphic;
                                } else
                                {
                                    setFormat( i, 1,ColorVerbatim);
                                    //blockData->code[i]=1;
                                    state=StateGraphicCommand;
                                }
            } break;
            case StateGraphicAsyCommand:{
                tmp=text.at( i );
                if (tmp=='$') {
                    setFormat( i, 1,ColorVerbatim);
                    blockData->code[i]=1;
                    state=StateGraphicAsy;
                } else
                    if (tmp=='%') {
                        if (last=='\\')
                        {
                            setFormat( i, 1,ColorVerbatim);
                            state=StateGraphicAsy;
                        }
                        else
                        {
                            setFormat( i, 1,ColorVerbatim);
                            blockData->code[i]=1;
                            state=StateGraphicAsy;
                        }
                    } else
                        if (tmp== ' ') {
                            setFormat( i, 1,ColorVerbatim);
                            state=StateGraphicAsy;
                        }  else
                            if (tmp=='(' || tmp=='[' || tmp=='{' || tmp==')' || tmp==']' || tmp=='}') {
                                blockData->code[i]=1;
                                setFormat( i, 1,ColorVerbatim);
                                state=StateGraphicAsy;
                            } else
                                if (tmp=='\\' || tmp==',' || tmp==';' || /*tmp=='\'' ||*/ tmp=='\"' || tmp=='`' || tmp=='^' || tmp=='~') {
                                    blockData->code[i]=1;
                                    setFormat( i, 1,ColorVerbatim);
                                    state=StateGraphicAsy;
                                } else
                                {
                                    setFormat( i, 1,ColorVerbatim);
                                    //blockData->code[i]=1;
                                    state=StateGraphicAsyCommand;
                                }
            } break;
                /****************************/
            case StateBib: {
                tmp=text.at( i );
                /*		if (tmp== '\"' ){
            blockData->code[i]=1;
            setFormat( i, 1,ColorVerbatim);
            state=StateBibCommand;
        } else*/
                if (tmp== '}' ){
                    blockData->code[i]=1;
                    setFormat( i, 1,ColorVerbatim);
                    if (next=='\n' || i == text.length()-1)
                    {
                        state=StateStandard;
                        buffer = QString();
                    }
                } else
                {
                    blockData->code[i]=1;
                    setFormat( i, 1,ColorVerbatim);
                    state=StateBib;
                    //buffer = QString();
                }
            } break;
                /*       case StateBibCommand: {
         qDebug() << "bibcommand" << buffer;
               tmp=text.at( i );
        if (tmp== '\"' ){
            blockData->code[i]=1;
            setFormat( i, 1,ColorVerbatim);
            state=StateBib;
        } else
        if (tmp== '}' ){
            blockData->code[i]=1;
            setFormat( i, 1,ColorVerbatim);
            if (next!=',' || i == text.length()-1)
              {
              state=StateBib;
              }
            else
            {
             state=StateBibCommand;
            }
        } else
         {
            setFormat( i, 1,ColorVerbatim);
            state=StateBibCommand;
        }
       } break;*/
                /***************************/
            case StateSweave: {
                tmp=text.at( i );
                if (tmp=='\\') {
                    if (next=='[')
                    {
                        setFormat( i, 1,ColorVerbatim );
                        blockData->code[i]=1;
                        i++;
                        if ( i < text.length())
                        {
                            setFormat( i, 1,ColorVerbatim);
                            blockData->code[i]=1;
                        }
                    }
                    else
                    {
                        setFormat( i, 1,ColorVerbatim );
                        blockData->code[i]=1;
                        state=StateSweaveCommand;
                    }
                } else
                    if (tmp=='$') {
                        setFormat( i, 1,ColorVerbatim);
                        blockData->code[i]=1;
                        if (next=='$')
                        {
                            i++;
                            if ( i < text.length())
                            {
                                setFormat( i, 1,ColorVerbatim);
                                blockData->code[i]=1;
                            }
                        }
                        buffer = QString();
                    } else
                        if (tmp== '%' ){
                            setFormat( i, 1,ColorVerbatim);
                            blockData->code[i]=1;
                            buffer = QString();
                        } else
                            if (tmp== '{' ){
                                blockData->code[i]=1;
                                setFormat( i, 1,ColorVerbatim);
                            } else
                                if (tmp== '}' ){
                                    blockData->code[i]=1;
                                    setFormat( i, 1,ColorVerbatim);
                                    buffer = QString();
                                } else
                                    if (tmp== '@' ){
                                        blockData->code[i]=1;
                                        setFormat( i, 1,ColorVerbatim);
                                        state=StateStandard;
                                        buffer = QString();
                                    } else
                                        if (tmp== '(' ){
                                            blockData->code[i]=1;
                                            setFormat( i, 1,ColorVerbatim);
                                            buffer = QString();
                                        } else
                                            if (tmp== ')' ){
                                                blockData->code[i]=1;
                                                setFormat( i, 1,ColorVerbatim);
                                                buffer = QString();
                                            } else
                                                if (isWordSeparator(tmp)){
                                                    blockData->code[i]=1;
                                                    setFormat( i, 1,ColorVerbatim);
                                                    buffer = QString();
                                                } else
                                                {
                                                    setFormat( i, 1,ColorVerbatim);
                                                    //buffer = QString();
                                                }
            } break;
            case StateSweaveCommand:{
                tmp=text.at( i );
                if (tmp=='$') {
                    setFormat( i, 1,ColorVerbatim);
                    blockData->code[i]=1;
                    state=StateSweave;
                } else
                    if (tmp=='%') {
                        if (last=='\\')
                        {
                            setFormat( i, 1,ColorVerbatim);
                            state=StateSweave;
                        }
                        else
                        {
                            setFormat( i, 1,ColorVerbatim);
                            blockData->code[i]=1;
                            state=StateSweave;
                        }
                    } else
                        if (tmp== ' ') {
                            setFormat( i, 1,ColorVerbatim);
                            state=StateSweave;
                        }  else
                            if (tmp=='(' || tmp=='[' || tmp=='{' || tmp==')' || tmp==']' || tmp=='}') {
                                blockData->code[i]=1;
                                setFormat( i, 1,ColorVerbatim);
                                state=StateSweave;
                            } else
                                if (tmp=='\\' || tmp==',' || tmp==';' /*|| tmp=='\''*/ || tmp=='\"' || tmp=='`' || tmp=='^' || tmp=='~') {
                                    blockData->code[i]=1;
                                    setFormat( i, 1,ColorVerbatim);
                                    state=StateSweave;
                                } else
                                {
                                    setFormat( i, 1,ColorVerbatim);
                                    blockData->code[i]=1;
                                    state=StateSweaveCommand;
                                }
            } break;
            }
            last = ch;
            i++;
        }
        if ( state == StateComment )
        {
            setCurrentBlockState(StateStandard);
        }
        else if ( state == StateMath )
        {
            setCurrentBlockState(StateMath) ;
        }
        else if ( state == StateGraphicMath )
        {
            setCurrentBlockState(StateGraphicMath) ;
        }
        else if ( state == StateVerbatim )
        {
            setCurrentBlockState(StateVerbatim) ;
        }
        else if ( state == StateVerbatimCommand )
        {
            setCurrentBlockState(StateVerbatimCommand) ;
        }
        else if ( state == StateGraphicComment )
        {
            setCurrentBlockState(StateGraphic) ;
        }
        else if ( state == StateGraphic )
        {
            setCurrentBlockState(StateGraphic) ;
        }
        else if ( state == StateGraphicCommand )
        {
            setCurrentBlockState(StateGraphicCommand) ;
        }
        else if ( state == StateGraphicAsyComment )
        {
            setCurrentBlockState(StateGraphicAsy) ;
        }
        else if ( state == StateGraphicAsy )
        {
            setCurrentBlockState(StateGraphicAsy) ;
        }
        else if ( state == StateGraphicAsyCommand )
        {
            setCurrentBlockState(StateGraphicAsyCommand) ;
        }
        else if ( state == StateSweave )
        {
            setCurrentBlockState(StateSweave) ;
        }
        else if ( state == StateSweaveCommand )
        {
            setCurrentBlockState(StateSweaveCommand) ;
        }
        else if ( state == StateBib )
        {
            setCurrentBlockState(StateBib) ;
        }
        /*else if ( state == StateBibCommand )
       {
       setCurrentBlockState(StateBibCommand) ;
       } */
        else
        {
            setCurrentBlockState(StateStandard) ;
            state=StateStandard;
        }
        /////////////////////

        //if ((oldstate == StateStandard) && (state == StateStandard))
        //{
        i=currentBlock().blockNumber();
        //editor->checkStructUpdate(document(),currentBlock().position(),text,i);
        //const QList<StructItem>& structure = editor->getStructItems();
        //for (int j = 0; j < structure.count(); j++)
        //{
        //if ((structure.at(j).cursor.block().blockNumber()==currentBlock().blockNumber()) && (structure.at(j).type!=0))
        //  {
        //  int offwidth=0;
        //  QTextCursor curs = editor->document()->find("{",structure.at(j).cursor.position());
        //  if (!curs.isNull()  && (curs.position()<currentBlock().length()+currentBlock().position()))
        //    {
        //    offwidth=curs.position()-structure.at(j).cursor.position()+structure.at(j).item.length()+1;
        //    }
        //    if (offwidth>0) setFormat(structure.at(j).cursor.position()-currentBlock().position(),offwidth,structFormat);
        //  }
        //}
        //}
        ////////////////////

        if (text.isEmpty()) return;
        QBrush brushstandard(ColorStandard);
        QBrush brushverbatim(ColorVerbatim);
        QBrush brushkeyword(ColorKeyword);
        QBrush brushmath(ColorMath);
        QBrush brushcomment(ColorComment);
        QTextCharFormat todoFormat;
        todoFormat.setFontWeight(QFont::Bold);
        todoFormat.setForeground(ColorTodo);
        todoFormat.setBackground(Qt::yellow);
        if (state == StateComment)
        {
            i=0;
            while (i < text.length())
            {
                buffer = QString();
                ch = text.at( i );
                while (!isSpace(ch) || ch=='%')
                {
                    buffer += ch;
                    i++;
                    if (i < text.length()) ch = text.at( i );
                    else break;
                }
                if ((buffer.length() > 0) && (format(i - buffer.length()).foreground()==brushcomment))
                {

                    if (buffer=="%TODO")
                    {
                        setFormat( i - buffer.length()+1, buffer.length()-1,todoFormat);
                    }

                }
                i++;
            }
        }

        if (state == StateGraphic || state == StateGraphicCommand || state == StateGraphicComment || state == StateGraphicAsy || state == StateGraphicAsyCommand || state == StateGraphicAsyComment)
        {
            QRegularExpression number("[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");
            //int index = number.indexIn(text);
            int index=number.match(text).capturedStart();
            while (index >= 0) {
                //int length = number.matchedLength();
                int length=number.match(text).capturedLength();
                if (blockData->code[index]!=1) setFormat(index, length, ColorNumberGraphic);
                //index = number.indexIn(text, index + length);
                index=number.match(text,index+length).capturedStart();
            }
            if (state == StateGraphicAsy || state == StateGraphicAsyCommand || state == StateGraphicAsyComment)
            {
                QRegularExpression expression("\\b[A-Za-z0-9_]+(?=\\()");
                //index = expression.indexIn(text);
                index=expression.match(text).capturedStart();
                while (index >= 0) {
                    //int length = expression.matchedLength();
                    int length=expression.match(text).capturedLength();
                    if (blockData->code[index]!=1) setFormat(index, length, ColorKeyword);
                    //index = expression.indexIn(text, index + length);
                    index=expression.match(text,index+length).capturedStart();
                }
                i=0;
                while (i < text.length())
                {
                    buffer = QString();
                    ch = text.at( i );
                    while ((blockData->code[i]!=1 || ch=='\\') && (!isSpace(ch)))
                    {
                        buffer += ch;
                        i++;
                        if (i < text.length()) ch = text.at( i );
                        else break;
                    }
                    if ((buffer.length() > 0) && (format(i - buffer.length()).foreground()==brushverbatim))
                    {
                        for ( QStringList::Iterator it = KeyWordsGraphic.begin(); it != KeyWordsGraphic.end(); ++it )
                        {
                            if (*it==buffer )
                            {
                                setFormat( i - buffer.length(), buffer.length(),ColorKeywordGraphic);
                            }
                        }
                        for ( QStringList::Iterator it = KeyWordsGraphicBis.begin(); it != KeyWordsGraphicBis.end(); ++it )
                        {
                            if (*it==buffer )
                            {
                                setFormat( i - buffer.length(), buffer.length(),asyFormat);
                            }
                        }
                    }

                    i++;
                }
            }
            else if ((text.indexOf("pspicture") == -1) && (text.indexOf("tikzpicture") == -1))
            {
                QRegularExpression expression("\\\\[A-Za-z]+");
                //index = expression.indexIn(text);
                index=expression.match(text).capturedStart();
                while (index >= 0) {
                    //int length = expression.matchedLength();
                    int length=expression.match(text).capturedLength();
                    if ((format(index).foreground()!=brushmath) && (format(index).foreground()!=brushcomment)) setFormat(index, length, ColorKeywordGraphic);
                    //index = expression.indexIn(text, index + length);
                    index=expression.match(text,index+length).capturedStart();
                }
            }

        }

        if (/*pChecker &&*/ state!=StateGraphic && state!=StateGraphicCommand && state!=StateGraphicComment && state!=StateGraphicAsy && state!=StateGraphicAsyCommand && state!=StateGraphicAsyComment)
        {
            i=0;
            int check;
            int offset ;
            QTextCodec *codec = QTextCodec::codecForName(spell_encoding.toLatin1());
            if(!codec) codec = QTextCodec::codecForLocale();
            QByteArray encodedString;

            while (i < text.length())
            {
                buffer = QString();
                ch = text.at( i );
                offset=0;
                while ((blockData->code[i]!=1) && (!isSpace(ch)))
                {
                    buffer += ch;
                    i++;
                    if (i < text.length()) ch = text.at( i );
                    else break;
                }
                while (buffer.startsWith('\''))
                {
                    buffer=buffer.right(buffer.size()-1);
                }
                while (buffer.endsWith('\''))
                {
                    buffer.chop(1);
                    offset++;
                }
                if ( (buffer.length() > 1) && (!ignoredwordList.contains(buffer)) && (!hardignoredwordList.contains(buffer)))
                {
                    if (format(i - buffer.length()-offset).foreground()==brushverbatim) spellingErrorFormat.setForeground(brushverbatim);
                    else if (format(i - buffer.length()-offset).foreground()==brushkeyword) spellingErrorFormat.setForeground(brushkeyword);
                    else spellingErrorFormat.setForeground(brushstandard);
                    if (format(i - buffer.length()-offset).fontWeight()==QFont::Bold) {spellingErrorFormat.setFontWeight(QFont::Bold);spellingErrorFormat.setFontUnderline(true);}
                    else spellingErrorFormat.setFontWeight(QFont::Normal);
                    encodedString = codec->fromUnicode(buffer);
                    //		      check = pChecker->spell(encodedString.data());
                    if (!check)
                    {
                        if (checkSpelling) setFormat(i - buffer.length()-offset, buffer.length(), spellingErrorFormat);
                        for (int k=i - buffer.length()-offset;  k< i-offset; k++) blockData->misspelled[k]=true;
                    }
                }
                i++;
            }
        }


    }
    //****************************************************************
    else
    {
        if (state<0) state=8;
        i=0;
        blockData->code.clear();
        blockData->misspelled.clear();
        for (int j=0; j < text.length(); j++) {blockData->code.append(0);blockData->misspelled.append(false);}
        while (i < text.length())
        {
            ch = text.at( i );
            buffer += ch;
            if ( i < text.length()-1 ) next = text.at( i+1 );

            switch (state) {


            case StateGraphicMath: {
                tmp=text.at( i );
                if (tmp== '"') {
                    setFormat( i, 1,mathFormat);
                    blockData->code[i]=1;
                    state=StateGraphic;
                    // 		if (tmp== '$') {
                    // 			setFormat( i, 1,mathFormat);
                    // 			blockData->code[i]=1;
                    // 			state=StateGraphic;
                    // 			if (next=='$')
                    // 				{
                    // 				i++;
                    // 				if ( i < text.length())
                    // 					{
                    // 					setFormat( i, 1,mathFormat);
                    // 					blockData->code[i]=1;
                    // 					}
                    // 				}
                } else if (tmp== '\\') {
                    if (next==']')
                    {
                        setFormat( i, 1,mathFormat);
                        blockData->code[i]=1;
                        state=StateGraphic;
                        i++;
                        if ( i < text.length())
                        {
                            setFormat( i, 1,mathFormat);
                            blockData->code[i]=1;
                        }
                    }
                    else
                    {
                        setFormat( i, 1,mathFormat);
                        blockData->code[i]=1;
                        state=StateGraphicMath;
                    }
                } else
                {
                    setFormat( i, 1,mathFormat);
                    blockData->code[i]=1;
                    state=StateGraphicMath;
                }
                buffer = QString();
            } break;

            case StateGraphic: {
                tmp=text.at( i );
                if (tmp=='\\') {
                    if (next=='[')
                    {
                        setFormat( i, 1,ColorStandard );
                        blockData->code[i]=1;
                        i++;
                        if ( i < text.length())
                        {
                            setFormat( i, 1,ColorStandard);
                            blockData->code[i]=1;
                        }
                    }
                    else
                    {
                        setFormat( i, 1,ColorStandard );
                        blockData->code[i]=1;
                        state=StateGraphicCommand;
                    }
                } else
                    if (tmp=='"') {
                        setFormat( i, 1,mathFormat);
                        blockData->code[i]=1;
                        state=StateGraphicMath;
                        buffer = QString();
                        // 		if (tmp=='$') {
                        // 			setFormat( i, 1,mathFormat);
                        // 			blockData->code[i]=1;
                        // 			state=StateGraphicMath;
                        // 			if (next=='$')
                        // 				{
                        // 				i++;
                        // 				if ( i < text.length())
                        // 					{
                        // 					setFormat( i, 1,mathFormat);
                        // 					blockData->code[i]=1;
                        // 					}
                        // 				}
                        // 			buffer = QString();
                    } else
                        if (tmp== '/' && next=='/'){
                            setFormat( i, 1,ColorComment);
                            state=StateGraphicComment;
                            blockData->code[i]=1;
                            buffer = QString();
                            // 		} else
                            // 		if (tmp== '%' ){
                            // 			setFormat( i, 1,ColorComment);
                            // 			state=StateGraphicComment;
                            // 			blockData->code[i]=1;
                            // 			buffer = QString();
                        } else
                            if (tmp== '{' ){
                                blockData->code[i]=1;
                                setFormat( i, 1,ColorStandard);
                            } else
                                if (tmp== '}' ){
                                    blockData->code[i]=1;
                                    setFormat( i, 1,ColorStandard);
                                    state=StateGraphic;
                                    buffer = QString();
                                } else
                                    if (tmp== '(' ){
                                        blockData->code[i]=1;
                                        setFormat( i, 1,ColorStandard);
                                        buffer = QString();
                                    } else
                                        if (tmp== ')' ){
                                            blockData->code[i]=1;
                                            setFormat( i, 1,ColorStandard);
                                            buffer = QString();
                                        } else
                                            if (isWordSeparator(tmp) && tmp!='"' && tmp!='.'  && tmp!='-'){
                                                blockData->code[i]=1;
                                                setFormat( i, 1,ColorStandard);
                                                buffer = QString();
                                            } else
                                            {
                                                setFormat( i, 1,ColorStandard);
                                                //buffer = QString();
                                            }
            } break;

            case StateGraphicComment: {
                setFormat( i, 1,ColorComment);
                blockData->code[i]=1;
                state=StateGraphicComment;
                buffer = QString();
            } break;

            case StateGraphicCommand:{
                tmp=text.at( i );
                if (tmp=='$') {
                    setFormat( i, 1,ColorStandard);
                    blockData->code[i]=1;
                    state=StateGraphic;
                } else
                    if (tmp=='%') {
                        if (last=='\\')
                        {
                            setFormat( i, 1,ColorStandard);
                            state=StateGraphic;
                        }
                        else
                        {
                            setFormat( i, 1,ColorStandard);
                            blockData->code[i]=1;
                            state=StateGraphic;
                        }
                    } else
                        if (tmp== ' ') {
                            setFormat( i, 1,ColorStandard);
                            state=StateGraphic;
                        }  else
                            if (tmp=='(' || tmp=='[' || tmp=='{' || tmp==')' || tmp==']' || tmp=='}') {
                                blockData->code[i]=1;
                                setFormat( i, 1,ColorStandard);
                                state=StateGraphic;
                            } else
                                if (tmp=='\\' || tmp==',' || tmp==';' || /*tmp=='\'' ||*/ tmp=='\"' || tmp=='`' || tmp=='^' || tmp=='~') {
                                    blockData->code[i]=1;
                                    setFormat( i, 1,ColorStandard);
                                    state=StateGraphic;
                                } else
                                {
                                    setFormat( i, 1,ColorStandard);
                                    //blockData->code[i]=1;
                                    state=StateGraphicCommand;
                                }
            } break;

            }
            last = ch;
            i++;
        }
        if ( state == StateGraphicMath )
        {
            setCurrentBlockState(StateGraphicMath) ;
        }
        else if ( state == StateGraphicComment )
        {
            setCurrentBlockState(StateGraphic) ;
        }
        else if ( state == StateGraphic )
        {
            setCurrentBlockState(StateGraphic) ;
        }
        else if ( state == StateGraphicCommand )
        {
            setCurrentBlockState(StateGraphicCommand) ;
        }
        else
        {
            setCurrentBlockState(StateGraphic) ;
            state=StateGraphic;
        }

        if (text.isEmpty()) return;
        if (state == StateGraphic || state == StateGraphicCommand || state == StateGraphicComment)
        {
            QRegularExpression number("[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");
            //int index = number.indexIn(text);
            int index=number.match(text).capturedStart();
            while (index >= 0) {
                //int length = number.matchedLength();
                int length=number.match(text).capturedLength();
                if (blockData->code[index]!=1) setFormat(index, length, ColorNumberGraphic);
                //index = number.indexIn(text, index + length);
                index=number.match(text,index+length).capturedStart();
            }
            QRegularExpression expression("\\b[A-Za-z0-9_]+(?=\\()");
            //index = expression.indexIn(text);
            index=expression.match(text).capturedStart();
            while (index >= 0) {
                //int length = expression.matchedLength();
                int length=expression.match(text).capturedLength();
                if (blockData->code[index]!=1) setFormat(index, length, ColorKeyword);
                //index = expression.indexIn(text, index + length);
                index=expression.match(text,index+length).capturedStart();
            }

            i=0;
            while (i < text.length())
            {
                buffer = QString();
                ch = text.at( i );
                while ((blockData->code[i]!=1 || ch=='\\') && (!isSpace(ch)))
                {
                    buffer += ch;
                    i++;
                    if (i < text.length()) ch = text.at( i );
                    else break;
                }
                if ((buffer.length() > 0) )
                {
                    for ( QStringList::Iterator it = KeyWordsGraphic.begin(); it != KeyWordsGraphic.end(); ++it )
                    {
                        if (*it==buffer )
                        {
                            setFormat( i - buffer.length(), buffer.length(),ColorKeywordGraphic);
                        }
                    }
                    for ( QStringList::Iterator it = KeyWordsGraphicBis.begin(); it != KeyWordsGraphicBis.end(); ++it )
                    {
                        if (*it==buffer )
                        {
                            setFormat( i - buffer.length(), buffer.length(),asyFormat);
                        }
                    }
                }
                i++;
            }

        }
    }
    //*****************************************************************



}

bool LatexHighlighter::isWordSeparator(QChar c) const
{
    switch (c.toLatin1()) {
    case '.':
    case ',':
    case '?':
    case '!':
    case ':':
    case ';':
    case '-':
    case '<':
    case '>':
    case '[':
    case ']':
    case '(':
    case ')':
    case '{':
    case '}':
    case '=':
    case '/':
    case '+':
    case '%':
    case '&':
    case '^':
    case '`':
        //    case '*':
        //    case '\'':
    case '"':
    case '~':
        return true;
    default:
        return false;
    }
}

bool LatexHighlighter::isSpace(QChar c) const
{
    return c == QLatin1Char(' ')
           || c == QChar::Nbsp
           || c == QChar::LineSeparator
           || c == QLatin1Char('\t')
        ;
}

void LatexHighlighter::setSpellChecker(Hunspell * checker)
{
    //pChecker = checker;
    //if (pChecker) spell_encoding=QString(pChecker->get_dic_encoding());
}

void LatexHighlighter::activateInlineSpell(bool enable)
{
    checkSpelling=enable;
}

void LatexHighlighter::setModeGraphic(bool m)
{
    isGraphic=m;
}
