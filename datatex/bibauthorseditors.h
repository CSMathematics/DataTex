#ifndef BIBAUTHORSEDITORS_H
#define BIBAUTHORSEDITORS_H

#include <QDialog>

namespace Ui {
class BibAuthorsEditors;
}

class BibAuthorsEditors : public QDialog
{
    Q_OBJECT

public:
    explicit BibAuthorsEditors(QWidget *parent = nullptr);
    ~BibAuthorsEditors();

private:
    Ui::BibAuthorsEditors *ui;
};

#endif // BIBAUTHORSEDITORS_H
