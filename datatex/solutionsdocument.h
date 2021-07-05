#ifndef SOLUTIONSDOCUMENT_H
#define SOLUTIONSDOCUMENT_H

#include <QDialog>
#include <QtWidgets>
#include <QList>
class MultiList;

namespace Ui {
class SolutionsDocument;
}

class SolutionsDocument : public QDialog
{
    Q_OBJECT

public:
    explicit SolutionsDocument(QWidget *parent = nullptr,QHash<QString,
                               QStringList> SolutionsPerExercise = QHash<QString,QStringList>());
    ~SolutionsDocument();

private slots:
    void on_ExercisesInDocument_cellClicked(int row, int column);
    void SolutionChanged(QString item);

private:
    Ui::SolutionsDocument *ui;

    QHash<QList<int>,QWidget *> CellWidgets;
    QListWidget * s;
};

class MultiList : public QComboBox {
    Q_OBJECT

private:
    QString displayText;
    QStringList itemsChecked;
    QStandardItemModel *model;

public:
    MultiList(QWidget *parent = 0) : QComboBox(parent) {
        setEditable(true);
        displayText = "";
        model = new QStandardItemModel;
        slotUpdateText();

        connect(model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotUpdate()));
    }

    void addItem(const QString &text) {
        int row = model->rowCount();
        QStandardItem* item = new QStandardItem();
        item->setText(text);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);
        model->setItem(row, 0, item);
        this->setModel(model);
    }

    void addItems(const QStringList &texts) {
        for (int i = 0; i < texts.count(); i++) {
            addItem(texts.at(i));
        }
    }

    QStringList getCheckedItems() const {
        QStringList checkedItems;

        for (int i = 0; i < model->rowCount(); i++) {
            if (model->item(i, 0)->checkState() == Qt::Checked) {
                checkedItems << model->item(i, 0)->text();
            }
        }
        return checkedItems;
    }

    void setCheckedItems(const QStringList &items) {
        for (int i = 0; i < items.count(); i++) {
            int index = findText(items.at(i));

            if (index != -1) {
                model->item(index)->setData(Qt::Checked, Qt::CheckStateRole);
            }
        }

        slotUpdate();
    }

public slots:
    void slotUpdateText() {
        lineEdit()->setText(displayText);
    }

    void slotUpdate() {
//        displayText = "";
        itemsChecked.clear();
        for (int i = 0; i < model->rowCount(); i++) {
            if (model->item(i, 0)->checkState() == Qt::Checked) {
                itemsChecked.append(model->item(i, 0)->text());
            }
        }
        displayText = itemsChecked.join(" + ");

        QTimer::singleShot(0, this, SLOT(slotUpdateText()));
    }

signals:
    void listofItems(const QStringList items);
};

#endif // SOLUTIONSDOCUMENT_H
