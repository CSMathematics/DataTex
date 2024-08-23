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

#ifndef BLOCKDATA_H
#define BLOCKDATA_H

#include <QTextBlock>
#include <QTextBlockUserData>

struct ParenthesisInfo {
	QChar character;
	int position;
}; 

struct LatexBlockInfo {
	QChar character;
	int position;
};

class BlockData : public QTextBlockUserData
{
public:
    BlockData() {};
    static BlockData *data(const QTextBlock &block) { return static_cast<BlockData *>(block.userData()); }
    QList<int> code;
    QList<bool> misspelled;
    QVector<ParenthesisInfo *> parentheses();
    QVector<LatexBlockInfo *> latexblocks();
    void insertPar( ParenthesisInfo *info );
    void insertLat( LatexBlockInfo *info );
private:
	QVector<ParenthesisInfo *> m_parentheses;
	QVector<LatexBlockInfo *> m_latexblocks;
};

#endif
