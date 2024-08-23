#pragma once

#include "dtl/variables.hpp"
#include <QStringList>

namespace qdiffview
{
struct Diff {
    int oldOffset;
    int newOffset;
    bool visible;
    dtl::edit_t type;
    QStringList lines;

    Diff(int oo, int no, dtl::edit_t t)
        : oldOffset(oo)
        , newOffset(no)
        , visible(false)
        , type(t)
    {}
};
}
