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

#ifndef THEME_H
#define THEME_H

#include <QString>
#include <QColor>

namespace Theme {
const QColor darkbackgroundColor=QColor( "#ffffff" );
const QColor lightbackgroundColor=QColor( "#7F7F7F" );

const QColor grayColor=QColor("#BDBFBF" );

const QString viewportLightStyleSheet= QString("QWidget {background-color:#7F7F7F;}");

const QString viewportDarkStyleSheet= QString("QWidget {background-color:#ffffff;}");

const QString listwidgetStyleSheet= QString("QListView {background-color:#ffffff; border: none; color:#BDBFBF; outline:1; show-decoration-selected:1 ;spacing:2px;}"
                  "QListView::item:selected {background-color: #7F7F7F; color:white;}"
                  "QListView::item:hover {background-color:#7F7F7F; color:white;}"
                     );


const QString toolbarlightStyleSheet= QString("QWidget {background-color:#7F7F7F;}");

const QString comboboxStyleSheet= QString("QComboBox {selection-background-color: #ffffff;border-style: solid;border: 1px solid #BDBFBF;border-radius: 2px;padding: 1px; color:#BDBFBF;font-weight:bold;}"
                  "QComboBox:hover {border: 1px solid #BDBFBF;color: #ffffff;}"
                  "QComboBox QAbstractItemView {background-color: #ffffff;border-radius: 2px;border: 1px solid #76797C;    selection-background-color: #7F7F7F;color:#BDBFBF;}"
                  "QComboBox::drop-down{subcontrol-origin: padding;subcontrol-position: top right;width:15px;border-left-width: 0px;border-left-color: darkgray;border-left-style: solid;border-top-right-radius: 3px;border-bottom-right-radius: 3px;}"
                  "QComboBox::down-arrow{image: url(:/images/down_arrow_disabled.png);}"
                  "QComboBox::down-arrow:on, QComboBox::down-arrow:hover,QComboBox::down-arrow:focus{image: url(:/images/down_arrow.png);}"
                     );
const QString lineeditStyleSheet= QString("QLineEdit {background-color: #ffffff;border-style: solid;border: 1px solid #BDBFBF;border-radius: 2px;padding: 1px; color:#BDBFBF;font-weight:bold;}");


const QString treeviewStyleSheet= QString("QTreeView {background-color:#ffffff; border: none; color:#BDBFBF; outline:0; show-decoration-selected:0 ;}"
                "QTreeView::item:selected {background-color: #7F7F7F; color:white;}"
                "QTreeView::item:hover {background-color:#7F7F7F; color:white;}"
                "QTreeView:branch:selected, QTreeView:branch:hover{background: url(:/images/empty.png);background-color:#ffffff;}"
                "QTreeView::branch:has-siblings:!adjoins-item {border-image: url(:/images/empty.png);}"
                "QTreeView::branch:has-siblings:adjoins-item {border-image: url(:/images/empty.png);}"
                "QTreeView::branch:!has-children:!has-siblings:adjoins-item {border-image: url(:/images/empty.png);}"
                "QTreeView::branch:has-children:!has-siblings:closed,QTreeView::branch:closed:has-children:has-siblings {    image: url(:/images/branch_closed.png);}"
                "QTreeView::branch:open:has-children:!has-siblings,QTreeView::branch:open:has-children:has-siblings  {   image: url(:/images/branch_open.png);}"
                "QTreeView::branch:has-children:!has-siblings:closed:hover,QTreeView::branch:closed:has-children:has-siblings:hover {image: url(:/images/branch_closed-on.png);}"
                "QTreeView::branch:open:has-children:!has-siblings:hover,QTreeView::branch:open:has-children:has-siblings:hover{image: url(:/images/branch_open-on.png);}"
                     );
const QString labelStyleSheet= QString("QLabel {padding:0 0 0 0; margin:0px;  font-weight:bold;}");

const QString comboboxLightStyleSheet= QString("QComboBox {selection-background-color: #ffffff;border-style: solid;border: 1px solid #BDBFBF;border-radius: 2px;padding: 1px; color:#BDBFBF;font-weight:bold;}"
                  "QComboBox:hover {border: 1px solid #BDBFBF;color: #ffffff;}"
                  "QComboBox:on {selection-background-color: #ffffff;color: #BDBFBF;}"
                  //"QComboBox QAbstractItemView {background-color: #ffffff;border-radius: 2px;border: 1px solid #76797C;    selection-background-color: #18465d;}"
                  "QComboBox::drop-down{subcontrol-origin: padding;subcontrol-position: top right;width:15px;border-left-width: 0px;border-left-color: darkgray;border-left-style: solid;border-top-right-radius: 3px;border-bottom-right-radius: 3px;}"
                  "QComboBox::down-arrow{image: url(:/images/down_arrow_disabled.png);}"
                  "QComboBox::down-arrow:on, QComboBox::down-arrow:hover,QComboBox::down-arrow:focus{image: url(:/images/down_arrow.png);}"
                     );
const QString comboboxDarkStyleSheet= QString("QComboBox {selection-background-color: #7F7F7F;border-style: solid;border: 1px solid #BDBFBF;border-radius: 2px;padding: 1px; color:#BDBFBF;font-weight:bold;}"
                  "QComboBox:hover {border: 1px solid #BDBFBF;color: #ffffff;}"
                  "QComboBox:on {selection-background-color: #7F7F7F;color: #BDBFBF;}"
                  //"QComboBox QAbstractItemView {background-color: #ffffff;border-radius: 2px;border: 1px solid #76797C;    selection-background-color: #18465d;}"
                  "QComboBox::drop-down{subcontrol-origin: padding;subcontrol-position: top right;width:15px;border-left-width: 0px;border-left-color: darkgray;border-left-style: solid;border-top-right-radius: 3px;border-bottom-right-radius: 3px;}"
                  "QComboBox::down-arrow{image: url(:/images/down_arrow_disabled.png);}"
                  "QComboBox::down-arrow:on, QComboBox::down-arrow:hover,QComboBox::down-arrow:focus{image: url(:/images/down_arrow.png);}"
                     );
const QString menuDarkStyleSheet=QString("QMenu {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6B6B6B, stop: 1 #424242); "
		"border-left: 1px solid qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #BCBCBC, stop: 1 #4C4C4C);"
		"border-right: 1px solid qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #BCBCBC, stop: 1 #4C4C4C);"
		"border-top: 1px solid #BCBCBC;"
		"border-bottom: 1px solid #4C4C4C;"
		"padding-top:5px;padding-bottom:5px;}"
		"QMenu::separator {height:0px;border-top: 1px solid #292929; border-bottom:1px solid #737373; margin-left:-1px; margin-right:-1px;}"
		"QMenu::item {color:#CFD1D1;padding: 5px 25px 5px 32px;}"
		"QMenu::item::selected {background-color:#242424;border-top: 1px solid #151515; border-bottom:1px solid #737373;}"
		"QMenu::icon {padding-left:15px;}");
const QString comboboxSearchStyleSheet= QString("QComboBox {background-color:#ffffff;selection-background-color: #BDBFBF;selection-color: #7F7F7F;border: 0px ;padding: 1px; color:#BDBFBF;}"
                  "QComboBox QAbstractItemView {background-color: #ffffff;border-radius: 2px;border: 1px solid #76797C;selection-background-color: #BDBFBF;selection-color: #7F7F7F;color:#BDBFBF;}"
                  "QComboBox::drop-down{subcontrol-origin: padding;subcontrol-position: top right;width:15px;border-left-width: 0px;border-left-color: darkgray;border-left-style: solid;border-top-right-radius: 3px;border-bottom-right-radius: 3px;}"
                  "QComboBox::down-arrow{image: url(:/images/down_arrow_disabled.png);}"
                  "QComboBox::down-arrow:on, QComboBox::down-arrow:hover,QComboBox::down-arrow:focus{image: url(:/images/down_arrow.png);}"
                     );
const QString lineeditSearchStyleSheet= QString("QLineEdit {background-color: #ffffff;border-style: solid;border: 1px solid #BDBFBF;border-radius: 2px;padding: 2px; color:#BDBFBF;selection-background-color: #BDBFBF;selection-color: #7F7F7F;}");

const QString buttonSearchStyleSheet= QString("QPushButton{background-color: #ffffff;border-style: solid;border: 1px solid #BDBFBF;border-radius: 2px;padding: 2px; color:#BDBFBF;}"
"QPushButton:hover {border: 1px solid #BDBFBF;color: #ffffff;}");

const QString spinboxSearchStyleSheet= QString("QSpinBox {background-color:#ffffff;selection-background-color: #BDBFBF;selection-color: #7F7F7F; color:#BDBFBF;border-style: solid;border: 1px solid #BDBFBF;border-radius: 2px;padding: 2px;margin:2px;}"
"QSpinBox::up-button {subcontrol-origin: border;subcontrol-position: top right;width: 16px; border-width: 1px;}"
"QSpinBox::up-arrow {image: url(:/images/up_arrow_disabled.png);}"
"QSpinBox::down-button {subcontrol-origin: border; subcontrol-position: bottom right;width: 16px;border-width: 1px;}"
"QSpinBox::down-arrow {image: url(:/images/down_arrow_disabled.png);}"
"QSpinBox::down-arrow:on, QSpinBox::down-arrow:hover,QSpinBox::down-arrow:focus{image: url(:/images/down_arrow.png);}"
"QSpinBox::up-arrow:on, QSpinBox::up-arrow:hover,QSpinBox::up-arrow:focus{image: url(:/images/up_arrow.png);}"
                  );
}
#endif // THEME_H
