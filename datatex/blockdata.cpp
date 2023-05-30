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

#include "blockdata.h"


QVector<ParenthesisInfo *> BlockData::parentheses() {
	return m_parentheses;
}
QVector<LatexBlockInfo *> BlockData::latexblocks() {
	return m_latexblocks;
}

void BlockData::insertPar( ParenthesisInfo *info ) {
	int i = 0;
	while (
		i < m_parentheses.size() &&
		info->position > m_parentheses.at(i)->position )
		++i;
	m_parentheses.insert( i, info );
}

void BlockData::insertLat( LatexBlockInfo *info ) {
	int i = 0;
	while (
		i < m_latexblocks.size() &&
		info->position > m_latexblocks.at(i)->position )
		++i;
	m_latexblocks.insert( i, info );
}
