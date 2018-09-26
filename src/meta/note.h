#ifndef NOTE_H
#define NOTE_H
#include <QString>
#include <QDateTime>
#include "notebook.h"
#include "tag.h"

class Note
{
public:
    Note();

    int id() const;
    void setId(int value);

    QString title() const;
    void    setTitle(const QString title);

    QString text() const;
    void    setText(const QString text);

    QDateTime date_created() const;
    void setDate_created(const QDateTime &date_created);

    QDateTime date_modified() const;
    void setDate_modified(const QDateTime &date_modified);

    Notebook *notebook() const;
    void setNotebook(Notebook *value);

    QList<Tag *> tags() const;
    void setTags(const QList<Tag *> &value);

private:
    int         m_id;
    QString     m_title;
    QString     m_text;
    QDateTime       m_date_created;
    QDateTime       m_date_modified;
    Notebook    *m_notebook;
    QList<Tag*> m_tags;
    // TODO: Implement user (shows ID of note owner)
    // TODO: Implement shared_with
    // TODO: Implement is_public
};

#endif // NOTE_H
