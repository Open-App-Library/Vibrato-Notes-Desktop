#ifndef NOTEBOOK_H
#define NOTEBOOK_H
#include <QString>
#include <QList>

class Notebook
{
public:
    Notebook();

    int id() const;
    void setId(int id);

    QString title() const;
    void setTitle(const QString &title);

    Notebook *parent() const;
    void setParent(Notebook *parent);

private:
    int m_id;
    QString m_title;
    Notebook *m_parent;
    // TODO: Implement user
    // TODO: implement shared_with
    // TODO: implement is_public
};

#endif // NOTEBOOK_H
