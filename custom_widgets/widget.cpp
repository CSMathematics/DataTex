#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *lay=new QVBoxLayout;
    m_tags=new TagsLineEditWidget(this);
    lay->addWidget(m_tags);
    setLayout(lay);
}

Widget::~Widget()
{
}

