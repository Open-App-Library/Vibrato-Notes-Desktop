#ifndef NOTE_H
#define NOTE_H
#include <QString>
#include <QDate>
#include "notebook.h"
#include "tag.h"

class Note
{
public:
    Note();
    QString title() const;
    void    setTitle(const QString title);

    QString text() const;
    void    setText(const QString text);

    QDate date_created() const;
    void setDate_created(const QDate &date_created);

    QDate date_modified() const;
    void setDate_modified(const QDate &date_modified);

    Notebook *notebook() const;
    void setNotebook(Notebook *value);

    QList<Tag *> getTags() const;
    void setTags(const QList<Tag *> &value);

    int getId() const;
    void setId(int value);

private:
    int         id;
    QString     m_title;
    QString     m_text;
    QDate       m_date_created;
    QDate       m_date_modified;
    Notebook    *m_notebook;
    QList<Tag*> m_tags;
    // TODO: Implement user (shows ID of note owner)
    // TODO: Implement shared_with
    // TODO: Implement is_public
};

#endif // NOTE_H
