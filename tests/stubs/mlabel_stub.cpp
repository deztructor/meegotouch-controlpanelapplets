#include <MLabel>

#include <QFont>
#include <QColor>

MLabel::MLabel(QGraphicsItem *parent, MLabelModel *model)
{
    Q_UNUSED (parent);
    Q_UNUSED (model);
}

MLabel::MLabel(QString const &text, QGraphicsItem *parent)
{
    Q_UNUSED (text);
    Q_UNUSED (parent);
}

MLabel::~MLabel()
{

}

void
MLabel::setAlignment(Qt::Alignment alignment)
{
    Q_UNUSED (alignment);
}

Qt::Alignment
MLabel::alignment() const
{
    return Qt::AlignLeft;
}

QTextOption::WrapMode
MLabel::wrapMode() const
{
    return QTextOption::NoWrap;
}

void
MLabel::setWrapMode(QTextOption::WrapMode wrapMode)
{
    Q_UNUSED (wrapMode);
}

void
MLabel::setWordWrap(bool wrap)
{
    Q_UNUSED (wrap);
}

bool
MLabel::wordWrap() const
{
    return false;
}

QString
MLabel::text() const
{
    return "";
}

void
MLabel::setTextElide(bool elide)
{
    Q_UNUSED (elide);
}

bool
MLabel::textElide() const
{
    return false;
}

void
MLabel::setFont(const QFont &font)
{
    Q_UNUSED (font);
}

QFont
MLabel::font() const
{
    return QFont ();
}

void
MLabel::setColor(const QColor &color)
{
    Q_UNUSED (color);
}

QColor
MLabel::color() const
{
    return QColor ();
}

void
MLabel::addHighlighter(MLabelHighlighter *highlighter)
{
    Q_UNUSED (highlighter);
}

void
MLabel::removeHighlighter(MLabelHighlighter *highlighter)
{
    Q_UNUSED (highlighter);
}

void
MLabel::removeAllHighlighters()
{

}

void
MLabel::setText(const QString &text)
{
    Q_UNUSED (text);
}

