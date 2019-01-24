#ifndef NOTEBOOK_H
#define NOTEBOOK_H
#include <QString>
#include <QVector>
#include <QObject>
#include <QUuid>
#include <QDateTime>

#define NOTEBOOK_DEFAULT_TITLE "Untitled Notebook"

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

  static Notebook *createBlankNotebook();

  // Sync Hash
  QUuid syncHash() const;
  void setSyncHash(QUuid syncHash);

  // Title
  QString title() const;
  void setTitle(const QString &title);

  // Date Modified
  QDateTime dateModified() const;
  void setDateModified(QDateTime dateModified);

  // Parent & Child functions
  Notebook *parent() const;
  void setParent(Notebook *parent);

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

  int row() const;
  void setRow(int row);

  bool encrypted() const;
  void setEncrypted(bool encrypted);

  // Returns true if default notebook
  bool defaultNotebook() const;

  // Requests the notebookDatabase to change the parent of this notebook
  void requestParentWithSyncHash(QUuid parentSyncHash);

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
  void requestedParentWithSyncHash(Notebook *notebook, QUuid parentSyncHash);

private slots:
  void handleChange(Notebook *notebook);

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
