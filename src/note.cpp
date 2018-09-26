#include "note.h"

Note::Note()
{
    m_title = "";
    m_text  = "";
}

QString Note::title()
{
    return m_title;
}

QString Note::text()
{
    return m_text;
}

void Note::setTitle(QString title)
{
    m_title = title;
}

void Note::setText(QString text)
{
    m_text = text;
}
