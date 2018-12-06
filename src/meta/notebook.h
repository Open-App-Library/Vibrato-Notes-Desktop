#ifndef NOTEBOOK_H
#define NOTEBOOK_H
#include <QString>
#include <QVector>
#include <QObject>

#define NOTEBOOK_DEFAULT_TITLE "Untitled Notebook"
#define NOTEBOOK_DEFAULT_NOTEBOOK_ID -1

class Notebook : public QObject
{
  Q_OBJECT
public:
  Notebook(int id, QString title, Notebook *parent=nullptr, QVector<Notebook*> children={});

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

signals:
  void notebookChanged(Notebook *notebook);
  void notebookIDChanged(Notebook *notebook);
  void notebookTitleChanged(Notebook *notebook);
  void notebookParentChanged(Notebook *notebook);
  void notebookChildrenChanged(Notebook *notebook);
  void deletingNotebook();

private:
  int m_id;
  QString m_title;
  Notebook *m_parent=nullptr;
  QVector<Notebook*> m_children;
  // TODO: Implement user
  // TODO: implement shared_with
  // TODO: implement is_public
};

#endif // NOTEBOOK_H
