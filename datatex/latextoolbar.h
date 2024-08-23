#ifndef LATEXTOOLBAR_H
#define LATEXTOOLBAR_H

#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QHBoxLayout>
#include <QDebug>
#include <QDialog>
#include <QDirIterator>
#include <QSortFilterProxyModel>
#include <QAbstractListModel>
#include <QFile>
#include <QtSvg>
#include <QDomElement>
#include <QDomNode>
#include "datatex.h"

class SymbolModel;
class SymbolListModel;
class LatexToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit LatexToolBar(QWidget *parent = nullptr);
    ~LatexToolBar();

    QAction * Save;
    QAction * Undo;
    QAction * Copy;
    QAction * Part;
    QAction * FontSize;
    QAction * Math;
    QAction * Environments;
    QAction * ListEnvironments;
    QAction * Tabular;
    QAction * FontStyle;
    QAction * Brackets;

private:
    QMenu * ToolsMenu;
    QToolBar * ToolBar;
    QToolBar * MathToolBar;
    QAction * CreateNewAction(QAction * Action, std::function<void()> Function, QString ShortCut, QIcon Icon, QString Description);
    void SectioningMenu();
    void FontSizeMenu();
    void MathMenu();
    void FontStyleMenu();

protected:

signals:
    QString SendToEditor(QString text);
};

struct SymbolItem {
    QString command;
    QString category;
    QString iconFile;
};
Q_DECLARE_METATYPE(SymbolItem)


class MathToolBar : public QWidget
{
    Q_OBJECT

public:

    explicit MathToolBar(QWidget *parent = nullptr);
    ~MathToolBar();

    QAction * Operators;
    QAction * Relations;
    QAction * Arrows;
    QAction * Delimiters;
    QAction * Greek;
    QAction * Cyrillic;
    QAction * MiscMath;
    QAction * MiscText;
    QAction * FontAwesome;
    QAction * Special;
    QListWidget * List;
    QStringList SymbolsInfo;
//    bool darkTheme;

private:
    QToolBar * MathBar;
    QAction * CreateNewAction(QAction * Action, std::function<void()> Function, QString ShortCut, QIcon Icon, QString Description);
    void filterItems(QString filter);

protected:

signals:
    QString SendToEditor(QString text);
};

class PreambleToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit PreambleToolBar(QWidget *parent = nullptr);
    ~PreambleToolBar();

    QAction * PreambleBasics;
    QAction * Commands;
    QAction * Environments;
    QAction * Fonts;
    QAction * Lists;

private:
    QToolBar * PreambleBar;
    QAction * CreateNewAction(QAction * Action, std::function<void()> Function, QIcon Icon, QString Description);

protected:

signals:
    QString SendToEditor(QString text);
};
#endif // LATEXTOOLBAR_H
