#ifndef NOTEBOOK_H
#define NOTEBOOK_H
#include <QString>
#include <QVector>
#include <QObject>
#include <QUuid>
#include <QDateTime>

class SQLManager;

#define NOTEBOOK_DEFAULT_TITLE "Untitled Notebook"
#define NOTEBOOK_DEFAULT_ROW -255

class Notebook : public QObject
{
  Q_OBJECT
public:
  Notebook(SQLManager *sql_manager,
           QUuid uuid = QUuid::createUuid(),
           QString title =NOTEBOOK_DEFAULT_TITLE,
           QDateTime date_created= QDateTime::currentDateTime(),
           QDateTime date_modified = QDateTime::currentDateTime(),
           Notebook *parent = nullptr,
           int row = NOTEBOOK_DEFAULT_ROW,
           bool encrypted = false);

  static Notebook *createBlankNotebook();

  SQLManager *sqlManager();
  void setSQLManager(SQLManager *sql_manager);

  QUuid uuid() const;
  void setUUID(QUuid uuid);

  QString title() const;
  void setTitle(const QString &title);

  QDateTime dateCreated() const;
  void setDateCreated(QDateTime dateCreated);

  QDateTime dateModified() const;
  void setDateModified(QDateTime dateModified);

  Notebook *parent() const;
  void setParent(Notebook *parent);

  QVector<Notebook *> children() const;
  QVector<Notebook *> recurseChildren(Notebook* parent=nullptr) const;
  void setChildren(const QVector<Notebook *> &children);

  void addChild(Notebook *child);
  void removeChild(Notebook *child);

  // These functions should not be used in the majority of cases
  // They do not have the safety checks of the other notebook
  // child/parent manipulation functions.
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
  void requestParentWithUUID(QUuid parentUUID);

signals:
  // General change signal - When any property is changed.
  void changed(Notebook *notebook);

  // Property signals
  void UUIDChanged(Notebook *notebook);
  void titleChanged(Notebook *notebook);
  void dateModifiedChanged(Notebook *notebook);
  void parentChanged(Notebook *notebook);
  void childrenChanged(Notebook *notebook);
  void rowChanged(Notebook *notebook);
  void encryptedChanged(Notebook *notebook);

  void deletingNotebook();
  void requestedParentWithUUID(Notebook *notebook, QUuid parentUUID);

private slots:
  void handleChange(Notebook *notebook);

private:
  SQLManager *m_sql_manager;
  QUuid m_uuid;
  QString m_title = "";
  QDateTime m_date_created;
  QDateTime m_date_modified;
  Notebook *m_parent;
  QVector<Notebook*> m_children;
  int m_row;
  bool m_encrypted;
};

#endif // NOTEBOOK_H
