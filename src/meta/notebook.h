#ifndef NOTEBOOK_H
#define NOTEBOOK_H
#include <QString>
#include <QVector>
#include <QObject>
#include <QUuid>
#include <QDateTime>

#define NOTEBOOK_DEFAULT_TITLE "Untitled Notebook"
#define NOTEBOOK_DEFAULT_NOTEBOOK_ID -1

class Notebook : public QObject
{
  Q_OBJECT
public:
  Notebook(QUuid sync_hash = QUuid::createUuid(),
           QString title = "",
           QDateTime date_modified = QDateTime::currentDateTime(),
           Notebook *parent = nullptr,
           int row = -255,
           bool encrypted = false);

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
  void removeChild(Notebook *child);

  // These functions should not be used in the majority of cases
  // They do not have the safety checks of the other notebook
  // manipulation functions.
  void setParent_primitive(Notebook *parent);
  void addChild_primitive(Notebook *child);
  void removeChild_primitive(Notebook *child);

signals:
  // General change signal - When any property is changed.
  void changed(Notebook *notebook);

  // Property signals
  void syncHashChanged(Notebook *notebook);
  void titleChanged(Notebook *notebook);
  void dateModifiedChanged(Notebook *notebook);
  void parentChanged(Notebook *notebook);
  void childrenChanged(Notebook *notebook);
  void rowChanged(Notebook *notebook);
  void encryptedChanged(Notebook *notebook);

  void deletingNotebook();

private:
  QUuid m_sync_hash;
  QString m_title = "";
  QDateTime m_date_modified;
  Notebook *m_parent;
  QVector<Notebook*> m_children;
  int m_row;
  bool m_encrypted;
};

#endif // NOTEBOOK_H
