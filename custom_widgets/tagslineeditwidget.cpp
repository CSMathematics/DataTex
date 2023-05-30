#include "tagslineeditwidget.h"
#include "qboxlayout.h"
#include <QDebug>
#include "sqlfunctions.h"
#include "datatex.h"

TagsLineEditWidget::TagsLineEditWidget(QWidget* parent,QStringList tags)
    : QWidget(parent)
    , m_textLayout(new QTextLayout())
    , m_cursorBlinkTimerId(0)
    , m_cursorBlinkStatus(false)
    , m_inputControl(QInputControl::TextEdit)

{
    QFont font=this->font();
    font.setPixelSize(14);
    this->setFont(font);

//    area = new QScrollArea(this);
//    area->setWidget(this);
//    area->setWidgetResizable(true);
    m_tagsPresenter=QSharedPointer<TagsPresenter>(new TagsPresenter(this->fontMetrics(), m_textLayout));
    UpdateTextLayout();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);//Растягивание виджета
    setFocusPolicy(Qt::StrongFocus);//Фокус пропадает как мышь уходит
    setCursor(Qt::IBeamCursor);
    setMouseTracking(true);

    SetCursorVisible(hasFocus());

    setContextMenuPolicy(Qt::CustomContextMenu);
    RightClick = new QDialog(this);
    QVBoxLayout * layout = new QVBoxLayout;
    mPopup = new QListWidget(RightClick);
    layout->addWidget(mPopup);
    layout->setMargin(0);
    RightClick->setLayout(layout);
    RightClick->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    RightClick->hide();

    items.append(tags);
    for (int item=0;item<items.count();item++ ) {
        mPopup->addItem(items.at(item));
        mPopup->item(item)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        mPopup->item(item)->setCheckState(Qt::Unchecked);
    }

    connect(mPopup,&QListWidget::itemSelectionChanged,[=](){
        QListWidgetItem *item = mPopup->currentItem();
        Qt::CheckState flag = static_cast<Qt::CheckState>(2*item->isSelected());
        item->setCheckState(flag);
    });

    connect(mPopup,&QListWidget::itemChanged,[=](QListWidgetItem *item){
        QString text = item->text();
        if(item->checkState() == Qt::Checked){
            m_tagsPresenter->InsertCharectersInEdittedTagText(m_tagsPresenter->GetCursorPosition(), text);
            m_tagsPresenter->SetCursorPosition(m_tagsPresenter->GetCursorPosition() + text.count());

            if (!m_tagsPresenter->GetCurrentEdittedTagText().isEmpty() && !m_tagsPresenter->IsEditedTextHasDuplicate())
            {
                m_tagsPresenter->InsertEmptyTagAtIndex(m_tagsPresenter->GetCurrentEditIndex() + 1);
                m_tagsPresenter->EditNextTag();
            }

            RepaintWidget();
            Q_EMIT ToTagsEdited();
        }
        else{
            int i = -1;
            foreach(QString tag,GetTags()){
                i++;
                if(text==tag){
                    m_tagsPresenter->RemoveTagAtIndex(i);
                    UpdateTextLayout();
                    m_tagsPresenter->CalculateAllTagsRects();
                    setFocus();
                    SetCursorVisible(true);
                }
            }
        }
    });
    setStyleSheet("background-color: rgb(255, 255, 255);border: 1px solid #C9CDD0;");
}

TagsLineEditWidget::~TagsLineEditWidget()
{
    delete mPopup;
}

void TagsLineEditWidget::resizeEvent(QResizeEvent* event)
{
    m_tagsPresenter->SetInputWidgetRect(GetInputWidgetRect());
    m_tagsPresenter->CalculateAllTagsRects();
    QWidget::resizeEvent(event);
    adjustPosition();
}

void TagsLineEditWidget::focusInEvent(QFocusEvent* event)
{
    SetCursorVisible(true);
    UpdateTextLayout();
    m_tagsPresenter->CalculateAllTagsRects();
    update();
    RightClick->setVisible(event->lostFocus());
}

void TagsLineEditWidget::focusOutEvent(QFocusEvent*)
{
    SetCursorVisible(false);
    UpdateTextLayout();
    m_tagsPresenter->CalculateAllTagsRects();
    update();
}

void TagsLineEditWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // opt
    QStyleOptionFrame panel;
    InitStyleOptionFrame(&panel);

    // draw frame
    style()->drawPrimitive(QStyle::PE_PanelLineEdit, &panel, &painter, this);//поле лайн эдита

    // clip
    QRect inputRect = GetInputWidgetRect();//поле где все + маргины
    painter.setClipRect(inputRect);//Включает обрезку и задает область клипа заданному прямоугольнику с помощью заданной операции клипа. По умолчанию используется операция замены текущего прямоугольника клипа.

    const QRect &editedTagRect = m_tagsPresenter->GetCurrentEdittedTagRect();
    const QPoint &editedTextPoint = editedTagRect.topLeft() + QPoint(tag_inner_left_padding,
                                                                     ( top_text_margin +tag_inner_top_padding));

    // scroll
    m_tagsPresenter->CalculateVecticalScroll(editedTagRect);

    // tags
    DrawTagsOnWidget(painter, 0, m_tagsPresenter->GetCurrentEditIndex(), 0);

    // draw edited text
    const QVector<QTextLayout::FormatRange>  textLayoutVector = EditetTextFormating();
    m_textLayout->draw(&painter, editedTextPoint - QPoint(0, m_tagsPresenter->GetVericalScrollValue()), textLayoutVector);
    // draw cursor
    if (m_cursorBlinkStatus)
    {
        m_textLayout->drawCursor(&painter, editedTextPoint - QPointF( 0, m_tagsPresenter->GetVericalScrollValue()), m_tagsPresenter->GetCursorPosition());
    }
    //end
    DrawTagsOnWidget(painter, m_tagsPresenter->GetCurrentEditIndex()+1, m_tagsPresenter->GetTagsCount(), 0);
}

void TagsLineEditWidget::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
    m_cursorBlinkStatus = !m_cursorBlinkStatus;
    update();
}

void TagsLineEditWidget::mousePressEvent(QMouseEvent* event)
{
    bool hasTagFound = false;
    QPoint point = event->pos();
    RightClick->exec();
    adjustPosition();
    for (int i = 0; i < m_tagsPresenter->GetTagsCount(); ++i)
    {
        mPopup->setVisible(!m_tagsPresenter->IsPointInCrossRectArea(i, event->pos()));
        if (m_tagsPresenter->IsPointInCrossRectArea(i, event->pos()))
        {
            if(items.contains(GetTags().at(i))){
                mPopup->item(items.indexOf(GetTags().at(i)))->setCheckState(Qt::Unchecked);
            }
            else{
                m_tagsPresenter->RemoveTagAtIndex(i);
            }
            hasTagFound = true;
            break;
        }
        else
        {
            if (m_tagsPresenter->GetTranslatedTagRectByIndex(i).contains(event->pos()))
            {
                if (m_tagsPresenter->GetCurrentEditIndex() == i)
                {
                    const QTextLine &textLine=m_textLayout->lineAt(0);
                    int xEditedTopLeft=m_tagsPresenter->GetEditedTranslatedTagRect().topLeft().x();
                    int cursorPositionAtTheEndOfTag=textLine.xToCursor(event->pos().x() -xEditedTopLeft);
                    m_tagsPresenter->SetCursorPosition(cursorPositionAtTheEndOfTag);
                    mPopup->setVisible(false);
                }
                else
                {
                    m_tagsPresenter->SetTagEditableAtIndex(i);
                }

                hasTagFound = true;
                break;
            }
        }
    }

    if (!hasTagFound)
    {
        m_tagsPresenter->AppendNewEmptyTag();
    }
    RepaintWidget();
}

QSize TagsLineEditWidget::sizeHint() const
{
    ensurePolished();
    QFontMetrics fontMetrics(font());
    int heightResult = fontMetrics.height() + 2 * vertical_margin + top_text_margin + bottom_text_margin + topmargin + bottommargin;
    int widthResult = fontMetrics.boundingRect(QLatin1Char('x')).width() * 17 + 2 * horizontal_margin + leftmargin + rightmargin; // "some"
    QStyleOptionFrame opt;
    InitStyleOptionFrame(&opt);
    return (style()->sizeFromContents(QStyle::CT_LineEdit, &opt,
                                      QSize(widthResult, heightResult).expandedTo(QApplication::globalStrut()), this));
}

QSize TagsLineEditWidget::minimumSizeHint() const {
    ensurePolished();
    QFontMetrics fontmetrics = fontMetrics();
    int heightResult = fontmetrics.height() + qMax(2 * vertical_margin, fontmetrics.leading()) + top_text_margin + bottom_text_margin + topmargin + bottommargin;
    int widthResult = fontmetrics.maxWidth() + leftmargin + rightmargin;
    QStyleOptionFrame opt;
    InitStyleOptionFrame(&opt);
    return (style()->sizeFromContents(QStyle::CT_LineEdit, &opt,
                                      QSize(widthResult, heightResult).expandedTo(QApplication::globalStrut()), this));
}

void TagsLineEditWidget::keyPressEvent(QKeyEvent* event)
{
    event->setAccepted(false);
    bool isCharecterKey = false;

    if (event == QKeySequence::SelectPreviousChar)
    {
        m_tagsPresenter->SetCursorPosition(m_textLayout->previousCursorPosition(m_tagsPresenter->GetCursorPosition()));
        event->accept();
    }
    else
        if (event == QKeySequence::SelectNextChar)
        {
            m_tagsPresenter->SetCursorPosition(m_textLayout->nextCursorPosition(m_tagsPresenter->GetCursorPosition()));
            event->accept();
        }
        else
        {
            switch (event->key())
            {
            case Qt::Key_Left:
            {
                if (m_tagsPresenter->GetCursorPosition() == 0)
                {
                    m_tagsPresenter->EditPreviousTag();
                }
                else
                {
                    m_tagsPresenter->SetCursorPosition(m_textLayout->previousCursorPosition(m_tagsPresenter->GetCursorPosition()));
                }
                event->accept();
                break;
            }
            case Qt::Key_Right:
            {
                if (m_tagsPresenter->GetCursorPosition() == m_tagsPresenter->GetCurrentEdittedTagText().size())
                {
                    m_tagsPresenter->EditNextTag();
                }
                else
                {
                    m_tagsPresenter->SetCursorPosition(m_textLayout->nextCursorPosition(m_tagsPresenter->GetCursorPosition()));
                }
                event->accept();
                break;
            }
            case Qt::Key_Home:
            {
                if (m_tagsPresenter->GetCursorPosition() == 0)
                {
                    m_tagsPresenter->SetTagEditableAtIndex(0);
                }
                else
                {
                    m_tagsPresenter->SetCursorPosition(0);
                }
                event->accept();
                break;
            }
            case Qt::Key_End:
            {
                if (m_tagsPresenter->GetCursorPosition() == m_tagsPresenter->GetCurrentEdittedTagText().size())
                {
                    m_tagsPresenter->SetTagEditableAtIndex(m_tagsPresenter->GetTagsCount() - 1);
                }
                else
                {
                    m_tagsPresenter->SetCursorPosition(m_tagsPresenter->GetCurrentEdittedTagText().length());
                }
                event->accept();
                break;
            }
            case Qt::Key_Backspace:
            {
                if (!m_tagsPresenter->GetCurrentEdittedTagText().isEmpty())
                {
                    m_tagsPresenter->RemoveCharecterInEditedTag();
                }
                else
                {
                    if (m_tagsPresenter->GetCurrentEditIndex() > 0)
                    {
                        m_tagsPresenter->EditPreviousTag();
                    }
                }
                event->accept();
                break;
            }
            case Qt::Key_Enter :
            case Qt::Key_Return :
            {
                if (!m_tagsPresenter->GetCurrentEdittedTagText().isEmpty() && !m_tagsPresenter->IsEditedTextHasDuplicate())
                {
                    m_tagsPresenter->InsertEmptyTagAtIndex(m_tagsPresenter->GetCurrentEditIndex() + 1);
                    m_tagsPresenter->EditNextTag();
                }
                event->accept();
                break;
            }
            default:
            {
                isCharecterKey = true;
                break;
            }
            }
        }

    if (isCharecterKey && m_inputControl.isAcceptableInput(event))
    {
        m_tagsPresenter->InsertCharectersInEdittedTagText(m_tagsPresenter->GetCursorPosition(), event->text());
        m_tagsPresenter->SetCursorPosition(m_tagsPresenter->GetCursorPosition() + event->text().count());
        event->accept();
        isCharecterKey = false;
    }

    if (event->isAccepted())
    {
        RepaintWidget();
        Q_EMIT ToTagsEdited();
    }
}

void TagsLineEditWidget::SetTags(QVector<QString> const& newTags)
{
    m_tagsPresenter->SetTags(newTags);
    UpdateTextLayout();
    update();
}

QVector<QString> TagsLineEditWidget::GetTags()
{
    return m_tagsPresenter->GetTags();
}

void TagsLineEditWidget::mouseMoveEvent(QMouseEvent* event)
{
    for (int i = 0; i < m_tagsPresenter->GetTagsCount(); ++i)
    {
        if (m_tagsPresenter->IsPointInCrossRectArea(i, event->pos()))
        {
            setCursor(Qt::ArrowCursor);
            return;
        }
    }
    setCursor(Qt::IBeamCursor);
}

void TagsLineEditWidget::InitStyleOptionFrame(QStyleOptionFrame *option) const
{
    Q_ASSERT(option);
    option->initFrom(this);
    option->rect = this->contentsRect();
    option->lineWidth = this->style()->pixelMetric(QStyle::PM_DefaultFrameWidth, option, this);
    option->midLineWidth = 0;
    option->state |= QStyle::State_Sunken;
    option->features = QStyleOptionFrame::None;
}

void TagsLineEditWidget::RepaintWidget()
{
    UpdateTextLayout();
    m_tagsPresenter->CalculateAllTagsRects();
    UpdateCursorBlinking();
    update();
}

void TagsLineEditWidget::UpdateTextLayout()
{
    m_textLayout->clearLayout();
    m_textLayout->setText(m_tagsPresenter->GetCurrentEdittedTagText());
    m_textLayout->beginLayout();
    m_textLayout->createLine();
    m_textLayout->endLayout();
}

void TagsLineEditWidget::UpdateCursorBlinking()
{
    SetCursorVisible(IsCursorVisible());
}

bool TagsLineEditWidget::IsCursorVisible() const
{
    return m_cursorBlinkStatus;
}

void TagsLineEditWidget::SetCursorVisible(bool visible)
{
    if (m_cursorBlinkStatus)
    {
        killTimer(m_cursorBlinkTimerId);
        m_cursorBlinkTimerId = 0;
        m_cursorBlinkStatus = true;
    }

    if (visible)
    {
        int cursorFlashTime = QGuiApplication::styleHints()->cursorFlashTime();
        m_cursorBlinkTimerId=startTimer(cursorFlashTime / 2);
    }
    else
    {
        m_cursorBlinkStatus = false;
    }
}

QVector<QTextLayout::FormatRange> TagsLineEditWidget::EditetTextFormating() const
{
    QTextLayout::FormatRange selection;
    return {selection};
}

QRect TagsLineEditWidget::GetInputWidgetRect()
{
    QStyleOptionFrame panel;
    InitStyleOptionFrame(&panel);
    QRect inputWidgetRect = this->style()->subElementRect(QStyle::SE_LineEditContents, &panel, this);
    inputWidgetRect.adjust(left_text_margin, top_text_margin, -right_text_margin, -bottom_text_margin);
    return inputWidgetRect;
}

void TagsLineEditWidget::DrawTagsOnWidget(QPainter &p, int startIndex, int lastIndex, int row)
{
    Q_ASSERT(startIndex>=0 && startIndex<=lastIndex);
    for (int i=startIndex; i< lastIndex; ++i)
    {
        // Рисуем прямоугольник тега
        QRect const& tagRect = m_tagsPresenter->GetTranslatedTagRectByIndex(i);
        const QColor blueColor(160, 215, 250);
        QPainterPath path;
        path.addRoundedRect(tagRect, 2, 2);
        p.fillPath(path, blueColor);

        // Рисуем текст
        QPoint const textPositionTopLeft = tagRect.topLeft() +
                QPoint(tag_inner_left_padding,
                       this->fontMetrics().ascent() +
                       ((tagRect.height()/(row+1) - this->fontMetrics().height()) / 2*(row+1)));
        p.setPen(Qt::black);
        p.drawText(textPositionTopLeft, m_tagsPresenter->GetTagTextByIndex(i));

        // Высчитаваем крестик закрытия
        QRect crossRect = m_tagsPresenter->GetCrossButtonRect(tagRect);

        // Рисуем крестик закрытия
        QPen pen = p.pen();
        pen.setWidth(2);

        p.save();
        p.setPen(pen);
        p.setRenderHint(QPainter::Antialiasing);
        p.drawLine(QLineF(crossRect.topLeft(), crossRect.bottomRight()));
        p.drawLine(QLineF(crossRect.bottomLeft(), crossRect.topRight()));
        p.restore();
    }
}

void TagsLineEditWidget::moveEvent(QMoveEvent *event)
{
    QWidget::moveEvent(event);
    adjustPosition();
}

void TagsLineEditWidget::adjustPosition()
{
    const QPoint globalPos = this->mapFromGlobal(QPoint(0, 0));
    const int posX = -globalPos.x();
    const int posY = -globalPos.y();
    RightClick->setGeometry(posX ,
                        posY + this->height(),
                        this->width(),100);
}

TagsFilterWidget::TagsFilterWidget(QWidget* parent,QStringList tags)
    : QWidget(parent)
{
    QHBoxLayout *grid = new QHBoxLayout;
    QSpacerItem *spacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Fixed);
    setLayout(grid);
    grid->setMargin(2);
    grid->setSpacing(2);
    QString style;
    QFile file(":/themes/tags_style.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        style = file.readAll();
        file.close();
    }
    foreach(QString tag,tags){
        QPushButton *button = new QPushButton(tag,parent);
        button->setLayoutDirection(Qt::RightToLeft);
        button->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        button->setCheckable(true);
        button->setFlat(true);

        button->setStyleSheet(style);
        tagButtonList.append(button);
        grid->addWidget(button);
    }
    grid->addItem(spacer);

    foreach (QPushButton * btn, tagButtonList) {
        connect(btn,&QPushButton::toggled,[=](){
            if(btn->isChecked()){
                tagList.append(btn->text());
            }
            else{
                tagList.removeOne(btn->text());
            }
            emit SelectedTags(tagList);
        });
    }
}

TagsFilterWidget::~TagsFilterWidget()
{

}
