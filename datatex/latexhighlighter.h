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


#ifndef LATEXHIGHLIGHTER_H
#define LATEXHIGHLIGHTER_H


#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>
#include <QColor>
#include <QTextBlockUserData>
#include "latexeditorwidget.h"
#include "hunspell/hunspell.hxx"

class QTextDocument;
class LatexEditor;

class LatexHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    LatexHighlighter(QTextDocument *parent = 0,bool spelling=false, QString ignoredWords=""/*,Hunspell *spellChecker=0*/);
    ~LatexHighlighter();
    QColor ColorStandard, ColorComment, ColorMath, ColorCommand, ColorKeyword, ColorVerbatim, ColorTodo, ColorKeywordGraphic, ColorNumberGraphic;
    QStringList KeyWords, KeyWordsGraphic, KeyWordsGraphicBis;
    QTextCharFormat spellingErrorFormat;
    QTextCharFormat mathFormat;
    QTextCharFormat commandFormat;
    QTextCharFormat keywordFormat;
    QTextCharFormat inMathFormat;
    QStringList alwaysignoredwordList;
public slots:
    void SetAlwaysIgnoredWords(QString ignoredWords);
    void setColors(QList<QColor> colors);
    void setSpellChecker(Hunspell * checker);
    void activateInlineSpell(bool enable);
    void SetEditor(LatexTextBrowser *ed);
    void setModeGraphic(bool m);
private :
    QString spell_dic, spell_encoding;
    QStringList ignoredwordList, hardignoredwordList;
    //Hunspell * pChecker;
    bool checkSpelling;
    bool isWordSeparator(QChar c) const;
    bool isSpace(QChar c) const;
    bool isGraphic;
protected:
    LatexTextBrowser *editor;
    void highlightBlock(const QString &text);
};


#endif
