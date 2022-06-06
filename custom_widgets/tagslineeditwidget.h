#ifndef TAGSLINEWIDGET_H
#define TAGSLINEWIDGET_H

#include <QWidget>
#include <QApplication>
#include <QStyleOptionFrame>
#include <qinputcontrol_p.h>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>

#include "tagspresenter.h"

const int vertical_margin = 3;
const int bottommargin = 1;
const int topmargin = 1;

const int horizontal_margin = 3;
const int leftmargin = 1;
const int rightmargin = 1;

class TagsLineEditWidget : public QWidget {
    Q_OBJECT

public:
    explicit TagsLineEditWidget(QWidget* parent, QStringList tags = QStringList());
    ~TagsLineEditWidget();

    // QWidget
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void SetTags(QVector<QString> const& newTags);
    QVector<QString> GetTags();
    QListWidget *mPopup;
    QStringList items;

Q_SIGNALS:
    void ToTagsEdited();

protected:
    void paintEvent(QPaintEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void moveEvent(QMoveEvent *event) override;

private:
    void InitStyleOptionFrame(QStyleOptionFrame* option) const;
    void RepaintWidget();
    void UpdateTextLayout();
    void UpdateCursorBlinking();
    bool IsCursorVisible() const;
    void SetCursorVisible(bool state);
    QVector<QTextLayout::FormatRange> EditetTextFormating() const;
    QRect GetInputWidgetRect();
    void DrawTagsOnWidget(QPainter& p, int startIndex, int lastIndex, int row);
    void adjustPosition();

private:
   QTextLayout *m_textLayout;
   int m_cursorBlinkTimerId;
   bool m_cursorBlinkStatus;
   QInputControl m_inputControl;
   QSharedPointer<TagsPresenter> m_tagsPresenter;
};

class TagsFilterWidget : public QWidget {
    Q_OBJECT

public:
    explicit TagsFilterWidget(QWidget* parent, QStringList tags = QStringList());
    ~TagsFilterWidget();

private:
    QList<QPushButton *> tagButtonList;
    QStringList tagList;

signals:

    QStringList SelectedTags(QStringList tags);
};
#endif // TAGSLINEWIDGET_H
