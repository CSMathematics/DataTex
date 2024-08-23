#include "qdiffview.h"

#include <utility>
#include <QDebug>
#include <QFontDatabase>
#include <QMap>
#include "private/diff.h"
#include "private/dtl/dtl.hpp"

QString bodyTemplate1 = "<html>\n"
                        "<head>\n"
                        "<style>\n"
                        ".text {font-weight:600;}\n"
                        "body {background-color: blue;width: 100%;}\n"
                        "table {width: 100%; }\n"
                        "th {color: #b2b2b2;}\n"
                        ".rp {background-color: #f8cbcb;}\n"
                        ".ap {background-color: #a6f3a6;}\n"
                        ".rl {background-color: #ffecec;}\n"
                        ".rl th {background-color: #ffdddd;}\n"
                        ".cl {background-color: #ffffff;}\n"
                        ".cl2 {background-color: #f4f7fb;}\n"
                        ".cl2 th {background-color: #edf2f9;}\n"
                        ".cl2 a {text-decoration:none;}\n"
                        ".al {background-color: #eaffea;}\n"
                        ".al th {background-color: #dbffdb;}\n"
                        "</style>\n"
                        "</head>\n"
                        "<body>\n"
                        "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"3\">\n";
QString bodyTemplate2 = "</table>\n"
                        "</body></html>\n";
QString addedTemplate = "<tr class=\"al\"><th width=\"40\"></th><th "
                        "align=\"right\" width=\"40\">%1</th><td width=\"100%\">%2</td></tr>\n";
QString removedTemplate = "<tr class=\"rl\"><th align=\"right\" "
                          "width=\"40\">%1</th><th width=\"40\"></th><td "
                          "width=\"100%\">%2</td></tr>\n";
QString commonTemplate
    = "<tr class=\"cl\"><th align=\"right\" width=\"40\">%1</th><th align=\"right\" "
      "width=\"40\">%2</th><td width=\"100%\">%3</td></tr>\n";
QString commonTemplate2
    = "<tr class=\"cl2\"><th align=\"right\" width=\"40\">%1</th><th align=\"right\" "
      "width=\"40\">%2</th><td width=\"100%\">%3</td></tr>\n";
QString commonLinkTemplate = "<tr class=\"cl2\"><th width=\"40\" colspan=\"2\"><a href=\"%1\">&gt;&#x2195;&lt;</a></th><td  "
                             "align=\"center\" width=\"100%\"</td></tr>\n";

QDiffView::QDiffView(QWidget *parent)
    : QLabel(parent)
{
    this->setTextFormat(Qt::RichText);
    auto fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    this->setFont(fixedFont);

    connect(this, &QLabel::linkActivated, this, [=](const QString &link) {
        auto index = link.toInt();
        this->_diffs[index]->visible = true;
        this->_update();
    });
}

QDiffView::~QDiffView() { qDeleteAll(this->_diffs); }

void QDiffView::setSource(const QString &oldString, const QString &newString)
{
    QMap<QString, int> line2id;
    QMap<int, QString> id2line;
    std::vector<int> oldStringIndex;
    std::vector<int> newStringIndex;
    qDeleteAll(this->_diffs);
    this->_diffs.clear();

    for (const QString &line : oldString.split(QRegExp("\n|\r\n|\r"))) {
        if (line2id.contains(line)) {
            oldStringIndex.push_back(line2id[line]);
        } else {
            int newId = line2id.size();
            oldStringIndex.push_back(newId);
            line2id[line] = newId;
            id2line[newId] = line;
        }
    }

    for (const QString &line : newString.split(QRegExp("\n|\r\n|\r"))) {
        if (line2id.contains(line)) {
            newStringIndex.push_back(line2id[line]);
        } else {
            int newId = line2id.size();
            newStringIndex.push_back(newId);
            line2id[line] = newId;
            id2line[newId] = line;
        }
    }

    dtl::Diff<int> linediff(oldStringIndex, newStringIndex);
    linediff.compose();

    dtl::edit_t last = -2;
    int oldLineNumber = 1;
    int newLineNumber = 1;

    for (auto ses : linediff.getSes().getSequence()) {
        //qDebug() << ses.second.type << id2line[ses.first];
        qdiffview::Diff *diff;
        if (ses.second.type != last) {
            diff = new qdiffview::Diff(oldLineNumber, newLineNumber, ses.second.type);
            last = ses.second.type;
            this->_diffs.append(diff);
        } else {
            diff = this->_diffs.last();
        }
        diff->lines.append(id2line[ses.first]);
        switch (ses.second.type) {
        case dtl::SES_COMMON:
            oldLineNumber++;
            newLineNumber++;
            break;
        case dtl::SES_ADD:
            newLineNumber++;
            break;
        case dtl::SES_DELETE:
            oldLineNumber++;
            break;
        }
    }
    bool isLastChanged = false;
    for (int i = 0; i < this->_diffs.length() - 1; i++) {
        auto oldDiff = this->_diffs[i];
        auto newDiff = this->_diffs[i + 1];
        if (oldDiff->type == dtl::SES_ADD && newDiff->type == dtl::SES_DELETE) {
            std::swap(this->_diffs[i], this->_diffs[i + 1]);
            std::swap(oldDiff, newDiff);
        }
        auto oldString = oldDiff->lines.join('\n');
        if (oldDiff->type != dtl::SES_DELETE || newDiff->type != dtl::SES_ADD) {
            oldDiff->lines = oldString.replace(' ', "&nbsp;").replace('\t', "&nbsp;&nbsp;&nbsp;&nbsp;").split('\n');
        } else {
            i++;
            if (i == this->_diffs.length()) {
                isLastChanged = true;
            }
            auto newString = newDiff->lines.join('\n');
            std::vector<QChar> oldChars(oldString.constBegin(), oldString.constEnd());
            std::vector<QChar> newChars(newString.constBegin(), newString.constEnd());
            dtl::Diff<QChar> chardiff(oldChars, newChars);
            chardiff.compose();
            auto oldlast = dtl::SES_COMMON;
            auto newlast = dtl::SES_COMMON;
            QString oldFormatString;
            QString newFormatString;
            auto resetIfNeeded = [](dtl::edit_t &last, QString &formatString) {
                if (last != dtl::SES_COMMON) {
                    formatString.append("</span>");
                    last = dtl::SES_COMMON;
                }
            };
            auto append = [](QString &formatString, QChar ch) {
                if (ch == ' ') {
                    formatString.append("&nbsp;");
                } else if (ch == '\t') {
                    formatString.append("&nbsp;&nbsp;&nbsp;&nbsp;");
                } else {
                    formatString.append(ch);
                }
            };

            for (auto ses : chardiff.getSes().getSequence()) {
                if (ses.second.type == dtl::SES_COMMON) {
                    resetIfNeeded(oldlast, oldFormatString);
                    append(oldFormatString, ses.first);
                    resetIfNeeded(newlast, newFormatString);
                    append(newFormatString, ses.first);
                } else if (ses.second.type == dtl::SES_DELETE) {
                    if (oldlast != dtl::SES_DELETE) {
                        oldFormatString.append("<span class=\"rp\">");
                        oldlast = dtl::SES_DELETE;
                    }
                    append(oldFormatString, ses.first);
                } else if (ses.second.type == dtl::SES_ADD) {
                    if (newlast != dtl::SES_ADD) {
                        newFormatString.append("<span class=\"ap\">");
                        newlast = dtl::SES_ADD;
                    }
                    append(newFormatString, ses.first);
                }
            }
            resetIfNeeded(oldlast, oldFormatString);
            resetIfNeeded(newlast, newFormatString);
            oldDiff->lines = oldFormatString.split('\n');
            newDiff->lines = newFormatString.split('\n');
        }
    }
    if (!isLastChanged) {
        auto lastLines = this->_diffs.last()->lines.join('\n');
        this->_diffs.last()->lines = lastLines.replace(' ', "&nbsp;").replace('\t', "&nbsp;&nbsp;&nbsp;&nbsp;").split('\n');
    }
    this->_update();
}

void QDiffView::_update()
{
    QStringList contents{bodyTemplate1};
    auto dumpCommon = [&](int oldLineNumber, int newLineNumber, const QStringList &commons) {
        for (int i = 0; i < commons.length(); i++) {
            auto &line = commons[i];
            if ((commons.length() > 6) && (2 < i) && (i < commons.length() - 3)) {
                contents.append(commonTemplate2.arg(oldLineNumber++).arg(newLineNumber++).arg(line));
            } else {
                contents.append(commonTemplate.arg(oldLineNumber++).arg(newLineNumber++).arg(line));
            }
        }
    };
    for (int i = 0; i < this->_diffs.length(); i++) {
        auto diff = this->_diffs[i];
        if (diff->type == dtl::SES_COMMON) {
            auto length = diff->lines.length();
            if (length > 7 && !diff->visible) {
                dumpCommon(diff->oldOffset, diff->newOffset, diff->lines.mid(0, 3));
                contents.append(commonLinkTemplate.arg(i));
                dumpCommon(diff->oldOffset + length - 3, diff->newOffset + length - 3,
                           diff->lines.mid(length - 3));
            } else {
                dumpCommon(diff->oldOffset, diff->newOffset, diff->lines);
            }

        } else if (diff->type == dtl::SES_DELETE) {
            int oldLineNumber = diff->oldOffset;
            for (auto &line : diff->lines) {
                contents.append(removedTemplate.arg(oldLineNumber++).arg(line));
            }
        } else {
            int newLineNumber = diff->newOffset;
            for (auto &line : diff->lines) {
                contents.append(addedTemplate.arg(newLineNumber++).arg(line));
            }
        }
    }
    contents.append(bodyTemplate2);
    this->setText(contents.join(""));
}
