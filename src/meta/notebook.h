#ifndef NOTEBOOK_H
#define NOTEBOOK_H
#include <QString>
#include <QVector>
#include <QObject>

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

    QVector<Notebook *> children() const;
    QVector<Notebook *> recurseChildren(Notebook* parent=nullptr) const;
    void setChildren(const QVector<Notebook *> &children);

    void addChild(Notebook *child);
    void removeChild(Notebook *child);

private:
    int m_id;
    QString m_title;
    Notebook *m_parent;
    QVector<Notebook*> m_children;
    // TODO: Implement user
    // TODO: implement shared_with
    // TODO: implement is_public
};

#endif // NOTEBOOK_H
