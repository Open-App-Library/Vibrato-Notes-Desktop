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
  Notebook(int syncId, int id, QString title, Notebook *parent=nullptr, QVector<Notebook*> children={});

  int syncId() const;
  void setSyncId(int syncId);

  int id() const;
  void setId(int id);

  QString title() const;
  void setTitle(const QString &title);

  Notebook *parent() const;
  void setParent(Notebook *parent);
  void requestParentChangeToID(int parentID);

  QVector<Notebook *> children() const;
  QVector<Notebook *> recurseChildren(Notebook* parent=nullptr) const;
  void setChildren(const QVector<Notebook *> &children);

  void addChild(Notebook *child);
  void removeChild(Notebook *child, bool dont_emit_change_signal=false);

signals:
  void notebookChanged(Notebook *notebook);
  void notebookSyncIDChanged(Notebook *notebook);
  void notebookIDChanged(Notebook *notebook);
  void notebookTitleChanged(Notebook *notebook);
  void notebookParentChanged(Notebook *notebook);
  void requestedParentWithID(Notebook *notebook, int requestedParentID);
  void notebookChildrenChanged(Notebook *notebook);
  void deletingNotebook();

private:
  int m_syncId;
  int m_id;
  QString m_title;
  Notebook *m_parent=nullptr;
  QVector<Notebook*> m_children;
  // Below are some todo items that are needed for sharing notes
  // TODO: Implement user
  // TODO: implement shared_with
  // TODO: implement is_public
};

#endif // NOTEBOOK_H
