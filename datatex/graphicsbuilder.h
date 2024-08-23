#ifndef GRAPHICSBUILDER_H
#define GRAPHICSBUILDER_H

#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include "sqlfunctions.h"
#include "qpdfviewer.h"

namespace Ui {
class GraphicsBuilder;
}

class GraphicsBuilder : public QDialog
{
    Q_OBJECT

public:
    explicit GraphicsBuilder(QWidget *parent = nullptr);
    ~GraphicsBuilder();

private:
    Ui::GraphicsBuilder *ui;
    QPdfViewer * GraphicsView;

private slots:
    void autoCompile();
};

#endif // GRAPHICSBUILDER_H
