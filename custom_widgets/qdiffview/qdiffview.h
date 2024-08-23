#pragma once

#include <QLabel>

namespace qdiffview
{
struct Diff;
}

class QDiffView : public QLabel
{
    Q_OBJECT
public:
    explicit QDiffView(QWidget *parent = 0);
    virtual ~QDiffView();
    void setSource(const QString &oldString, const QString &newString);

private:
    QList<qdiffview::Diff *> _diffs;
    void _update();
};
