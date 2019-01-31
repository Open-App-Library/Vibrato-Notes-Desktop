#ifndef CLOUDMANAGER_H
#define CLOUDMANAGER_H
#include <QObject>
#include <QDateTime>
#include <QVector>
#include <QUuid>
#include "api/vibrato-cloud-api.h"

class CloudManager : public QObject
{
  Q_OBJECT
public:
  CloudManager();

  QDateTime lastSync() const;
  QDateTime lastRefresh() const;

  bool noteExists(QUuid syncHash, bool refresh=false);
  bool notebookExists(QUuid syncHash, bool refresh=false);
  bool tagExists(QUuid syncHash, bool refresh=false);

  void refresh();
  void refreshNotes();
  void refreshNotebooks();
  void refreshTags();

private:
  QDateTime m_lastSync;
  QDateTime m_lastRefresh;

  QVector<QUuid> m_noteList;
  QVector<QUuid> m_notebookList;
  QVector<QUuid> m_tagList;
}

#endif // CLOUDMANAGER_H
