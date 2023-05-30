#ifndef TAGSPRESENTER_H
#define TAGSPRESENTER_H
#include <QVector>
#include <QPen>
#include <QTimer>
#include <QtMath>
#include <QPainterPath>
#include <QPainter>
#include <QStyleHints>
#include <QTextLayout>
#include <QStyleOptionFrame>
#include <qinputcontrol_p.h>

#include "tag.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
#define FONT_METRICS_WIDTH(fmt, ...) fmt.width(__VA_ARGS__)
#else
#define FONT_METRICS_WIDTH(fmt, ...) fmt.horizontalAdvance(__VA_ARGS__)
#endif

const int top_text_margin = 1;
const int bottom_text_margin = 1;
const int left_text_margin = 1;
const int right_text_margin = 1;

const int tag_horisontal_spacing = 3;
const int tag_inner_left_padding = 3;
const int tag_inner_right_padding = 4;
const int tag_inner_top_padding = 3;
const int tag_inner_bottom_padding = 3;
const int tag_cross_width = 7;
const int tag_cross_spacing = 5;
const int tag_cross_rightspacing = 3;

class TagsPresenter
{
public:
    TagsPresenter(const QFontMetrics &viewWidgetFontMetrics,const QTextLayout *m_editedTextLayout);
    ~TagsPresenter();
public://СЃРµС‚РµСЂС‹ РіРµС‚РµСЂС‹
    QVector<QString> GetTags() const;
    void SetTags(const QVector<QString> &newTags);
public://методы работы с коллекцией
    int GetTagsCount() const;
    void AppendNewEmptyTag();
    void RemoveTagAtIndex(int index);
    void InsertEmptyTagAtIndex(int index);
    bool IsEditedTextHasDuplicate();

public://cursor pos
    int GetCursorPosition() const;
    void SetCursorPosition(int position);
public:
    int GetCurrentEditIndex() const;
    void EditPreviousTag();
    void EditNextTag();
    void SetTagEditableAtIndex(int index);
private:
    void setEditingIndex(int index);
public:// text on  tags
    const QString& GetTagTextByIndex(int index);
    QString& UpdateTextInEdittedTag(const QString &text);
    QString& InsertCharectersInEdittedTagText(int startPositionInString, const QString& charecters);
    const QString& GetCurrentEdittedTagText() const;
    void RemoveCharecterInEditedTag();
private:
    QString& RemoveCharectersInEdittedTagText(int startPos, int count);
public://tags positions
    void CalculateAllTagsRects();
    bool IsPointInCrossRectArea(int index, QPoint const& point) const;
    const QRect& GetTagRectByIndex(int index) const;
    const QRect GetEditedTranslatedTagRect()const;
    const QRect GetTranslatedTagRectByIndex(int index) const;
    const QRect&  GetCurrentEdittedTagRect() const;
    inline QRect GetCrossButtonRect(QRect const& r) const;
    int GetVericalScrollValue() const;
    void SetInputWidgetRect(const QRect &inputWidgetRect);
    int GetAllTagsHeight() const;
    void CalculateVecticalScroll(QRect const& editedTagRect);
private:
    void CalculateTagsRects(QPoint& leftTopPoint, const QRect &widgetSizes, QVector<Tag> &Tags, int beginTagIndex, int lastTagIndex);
    void CalculateTagOnEdit(QPoint& leftTopPoint, const QRect &widgetSizes);
    QRect& SetCurrentEdittedTagRect(const QRect &rect);
private:
    QFontMetrics m_viewWidgetFontMetrics;
    const QTextLayout *m_textLayout;
    QVector<Tag> m_tags;
    int m_currentEditIndex;
    int m_cursorPosition;
    int m_vecticalScrollValue;
    QRect m_inputWidgetRect;
};

#endif // TAGSPRESENTER_H
