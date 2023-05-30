#include "tagspresenter.h"
#include <QtDebug>

TagsPresenter::TagsPresenter(const QFontMetrics &viewWidgetFontMetrics, const QTextLayout *m_editedTextLayout)
    : m_viewWidgetFontMetrics(viewWidgetFontMetrics)
    , m_textLayout(m_editedTextLayout)
    , m_tags({Tag()})
    , m_currentEditIndex(0)
    , m_cursorPosition(0)
    , m_vecticalScrollValue(0)
{
}

TagsPresenter::~TagsPresenter()
{
}

QVector<QString> TagsPresenter::GetTags() const
{
    QVector<QString> tagList;
    for (const Tag &item : m_tags)
    {
        tagList.push_back(item.text);
    }
    return tagList;
}

void TagsPresenter::SetTags(const QVector<QString> &newTags)
{
    m_tags.clear();
    for (int i=0; i<m_tags.count(); ++i)
    {
        m_tags.append(Tag{newTags.at(i), QRect()});
    }
    m_currentEditIndex = 0;
    SetCursorPosition(0);
    CalculateAllTagsRects();
}

int TagsPresenter::GetTagsCount() const
{
    return m_tags.count();
}

QRect TagsPresenter::GetCrossButtonRect(const QRect &r) const
{
    QRect crossRect(QPoint(0, 0), QSize(tag_cross_width, tag_cross_width));
    crossRect.moveCenter(QPoint(r.right() - tag_cross_width-tag_cross_rightspacing, r.center().y()));
    return crossRect;
}

int TagsPresenter::GetVericalScrollValue() const
{
    return m_vecticalScrollValue;
}

void TagsPresenter::SetInputWidgetRect(const QRect &inputWidgetRect)
{
    m_inputWidgetRect=inputWidgetRect;
}

bool TagsPresenter::IsPointInCrossRectArea(int index, const QPoint &point) const
{
    QRect CrossButtonRect(GetCrossButtonRect(m_tags.at(index).rect));
    CrossButtonRect.adjust(-2, 0, 0, 0);
    CrossButtonRect.translate(0, -m_vecticalScrollValue);
    if(CrossButtonRect.contains(point))
    {
        if(index!=m_currentEditIndex)
        {
            return true;
        }
    }
    return false;
}

const QString &TagsPresenter::GetTagTextByIndex(int index)
{
    Q_ASSERT(index>=0 && index<=m_tags.count());
    return m_tags.at(index).text;
}

void TagsPresenter::CalculateAllTagsRects()
{
    QRect widgetSizes = m_inputWidgetRect;
    QPoint leftTopPoint = widgetSizes.topLeft();

    CalculateTagsRects(leftTopPoint, widgetSizes, m_tags, 0 , m_currentEditIndex);//до эдита
    CalculateTagOnEdit(leftTopPoint, widgetSizes);//эдит
    CalculateTagsRects(leftTopPoint, widgetSizes, m_tags, m_currentEditIndex+1, m_tags.count());//после
}

void TagsPresenter::CalculateTagsRects(QPoint &leftTopPoint,const QRect &widgetSizes, QVector<Tag> &Tags, int beginTagIndex, int lastTagIndex)
{
    Q_ASSERT(beginTagIndex>=0 && beginTagIndex<=lastTagIndex);
    const int fontMetricsHeight= m_viewWidgetFontMetrics.height();//высота шрифта
    for (int i=beginTagIndex; i<lastTagIndex; ++i)
    {
        const int textMetricsWidth = m_viewWidgetFontMetrics.horizontalAdvance(Tags.at(i).text);//ширина шрифта
        if (leftTopPoint.x()+textMetricsWidth+tag_inner_left_padding+2*tag_inner_right_padding+ tag_cross_spacing + tag_cross_width>widgetSizes.width())//смотрим можем ли поместить все наше добро с отступами, если да то
        {
            leftTopPoint.setX(0);//перенос точки в ноль на след строке
            leftTopPoint.setY(leftTopPoint.y()+tag_inner_bottom_padding+fontMetricsHeight);
        }
        QRect newTagRect(leftTopPoint, QSize(textMetricsWidth, fontMetricsHeight));//прямоугольник тэга
        newTagRect.adjust(tag_inner_left_padding,
                          tag_inner_top_padding,
                          3*tag_inner_right_padding + tag_cross_spacing + tag_cross_width,
                          tag_inner_bottom_padding);//Добавляет соответственно к существующим координатам прямоугольника.
        //Пояснение:!
        //dx1=tag_inner_left_padding - отступ слева что бы место было
        //dy1=tag_inner_top_padding - отступ сверху
        //dx2=2*tag_inner_right_padding + tag_cross_spacing + tag_cross_width 2 - отсупа справа + отсуп крестика + ширина крестика
        //dy2=tag_inner_bottom_padding - отступ снизу
        Tags[i].rect = newTagRect;//записываем в память
        leftTopPoint.setX(newTagRect.right() + tag_horisontal_spacing);//устанавливаем новую точку
    }
}

void TagsPresenter::CalculateTagOnEdit(QPoint &leftTopPoint, const QRect &widgetSizes)
{
    const int fontMetricsHeight= m_viewWidgetFontMetrics.height();//высота шрифта
    const int editedTagHeight=tag_inner_top_padding+ fontMetricsHeight + tag_inner_bottom_padding;
    const int textMetricsWidth=FONT_METRICS_WIDTH(m_viewWidgetFontMetrics, m_textLayout->text());
    const int editedTagWidth = textMetricsWidth +tag_inner_left_padding + 2*tag_inner_right_padding+ tag_cross_spacing + tag_cross_width;
    if (leftTopPoint.x()+editedTagWidth>widgetSizes.width())
    {
        leftTopPoint.setX(0);
        leftTopPoint.setY(leftTopPoint.y()+tag_inner_bottom_padding+fontMetricsHeight);
    }
    SetCurrentEdittedTagRect(QRect(leftTopPoint, QSize(editedTagWidth, editedTagHeight)));
    leftTopPoint.setX(leftTopPoint.x()+editedTagWidth + tag_horisontal_spacing);
}

int TagsPresenter::GetCursorPosition() const
{
    return m_cursorPosition;
}

void TagsPresenter::SetCursorPosition(int position)
{
    m_cursorPosition=position;
}

void TagsPresenter::setEditingIndex(int index)
{
    Q_ASSERT(index>=0 && index <= m_tags.count());
    if (GetCurrentEdittedTagText().isEmpty())
    {
        m_tags.erase(std::next(m_tags.begin(), std::ptrdiff_t(m_currentEditIndex)));
        if (m_currentEditIndex <= index)
        {
            --index;
        }
    }
    m_currentEditIndex = index;
}

const QString &TagsPresenter::GetCurrentEdittedTagText() const
{
    return m_tags.at(m_currentEditIndex).text;
}

void TagsPresenter::RemoveTagAtIndex(int index)
{
    Q_ASSERT(index>=0 && index <=m_tags.count());
    m_tags.removeAt(index);
    if (index <= m_currentEditIndex)
    {
        m_currentEditIndex=m_currentEditIndex-1;
    }

}

void TagsPresenter::InsertEmptyTagAtIndex(int index)
{
    Q_ASSERT(index>=0);
    if (index<m_tags.count())
    {
        m_tags.insert(index, Tag());
    }
    else
    {
        m_tags.append(Tag());
    }
}

bool TagsPresenter::IsEditedTextHasDuplicate()
{
    const QString &currentEditetTagText=GetTagTextByIndex(GetCurrentEditIndex());
    for(int i=0; i<GetCurrentEditIndex(); i++)
    {
        if (GetTagTextByIndex(i)==currentEditetTagText)
        {
            return true;
        }
    }
    for (int i=GetCurrentEditIndex()+1; i<GetTagsCount(); i++)
    {
        if(GetTagTextByIndex(i)==currentEditetTagText)
        {
            return true;
        }
    }
    return false;
}

QString &TagsPresenter::UpdateTextInEdittedTag(const QString &text)
{
    return m_tags[m_currentEditIndex].text=text;
}

QString &TagsPresenter::RemoveCharectersInEdittedTagText(int startPos, int count)
{
    if (startPos>=0 && startPos<m_tags[m_currentEditIndex].text.count())
    {
        return m_tags[m_currentEditIndex].text.remove(startPos, count);
    }
    else
    {

    }
}

QString &TagsPresenter::InsertCharectersInEdittedTagText(int startPositionInString, const QString &charecters)
{
    Q_ASSERT(startPositionInString>=0 && startPositionInString<=m_tags[m_currentEditIndex].text.count());
    return m_tags[m_currentEditIndex].text.insert(startPositionInString, charecters);
}

const QRect &TagsPresenter::GetCurrentEdittedTagRect() const
{
    return m_tags.at(m_currentEditIndex).rect;
}

const QRect &TagsPresenter::GetTagRectByIndex(int index) const
{
    Q_ASSERT(index>=0 && index<m_tags.count());
    return m_tags.at(index).rect;
}

const QRect TagsPresenter::GetEditedTranslatedTagRect() const
{
    return GetCurrentEdittedTagRect().translated( 0, -m_vecticalScrollValue);
}

const QRect TagsPresenter::GetTranslatedTagRectByIndex(int index) const
{
    return GetTagRectByIndex(index).translated(0, -m_vecticalScrollValue);
}

QRect &TagsPresenter::SetCurrentEdittedTagRect(const QRect &rect)
{
    return m_tags[m_currentEditIndex].rect=rect;
}

int TagsPresenter::GetCurrentEditIndex() const
{
    return m_currentEditIndex;
}

void TagsPresenter::AppendNewEmptyTag()
{
    m_tags.push_back(Tag());
    setEditingIndex(m_tags.count() - 1);
    SetCursorPosition(0);
}

void TagsPresenter::RemoveCharecterInEditedTag()
{
    RemoveCharectersInEdittedTagText(--m_cursorPosition, 1);
}

int TagsPresenter::GetAllTagsHeight() const
{
    return m_tags.back().rect.bottom() - m_tags.front().rect.top();
}

void TagsPresenter::CalculateVecticalScroll(const QRect &editedTagRect)
{
    const QRect widgetRect = m_inputWidgetRect;
    const int allTagsHeight = GetAllTagsHeight();
    int const cursorYPosition = editedTagRect.y();

//    qDebug()<< "cursorYPosition" << cursorYPosition<< "width" << widgetRect.width()<< "m_vecticalScrollValue before" << m_vecticalScrollValue ;

    if (allTagsHeight <= widgetRect.height())
    {
        // Просто заполняем все, текст помещается весь
        m_vecticalScrollValue = 0;
    }
    else
    {
        if (cursorYPosition - m_vecticalScrollValue >= widgetRect.height()-1)
        {
            //едитиэд текст не помещается, курсор находится снизу в текст эдите (прокрутить вниз)
            m_vecticalScrollValue = cursorYPosition +m_viewWidgetFontMetrics.height()- widgetRect.height() + 1;
        }
        else
        {
            if (cursorYPosition - m_vecticalScrollValue <= 0 && m_vecticalScrollValue < allTagsHeight)
            {
                //                qDebug()<< "firstElse val" <<cursorYPosition - m_vecticalScrollValue;
                m_vecticalScrollValue=cursorYPosition;
                //                qDebug()<< " and "<<  m_vecticalScrollValue;
            }
            else
            {
                //Поднялись выше, но текст еще в пределах вьюва
                //                 qDebug()<< "secondElse val" <<cursorYPosition - m_vecticalScrollValue;
                m_vecticalScrollValue = allTagsHeight - widgetRect.height();
                //                qDebug()<<" and "<<  m_vecticalScrollValue;
            }
        }
    }
}

void TagsPresenter::EditPreviousTag()
{
    if (m_currentEditIndex > 0)
    {
        setEditingIndex(m_currentEditIndex - 1);
        SetCursorPosition(GetCurrentEdittedTagText().size());
    }
}

void TagsPresenter::EditNextTag()
{
    if (m_currentEditIndex < m_tags.count() - 1)
    {
        setEditingIndex(m_currentEditIndex + 1);
        SetCursorPosition(0);
    }
}

void TagsPresenter::SetTagEditableAtIndex(int i)
{
    assert(i >= 0 && i < m_tags.count());
    setEditingIndex(i);
    SetCursorPosition(GetCurrentEdittedTagText().size());
}





