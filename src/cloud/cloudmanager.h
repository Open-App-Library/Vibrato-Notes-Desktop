#ifndef CLOUDMANAGER_H
#define CLOUDMANAGER_H
#include <QObject>
#include "api/vibrato-cloud-api.h"

class CloudManager : public QObject
{
  Q_OBJECT
public:
  CloudManager();

  int noteExists(QUuid syncHash, bool refresh=false);
  int notebookExists(QUuid syncHash, bool refresh=false);
  int tagExists(QUuid syncHash, bool refresh=false);

  void refresh();
  void refreshNotes();
  void refreshNotebooks();
  void refreshTags();
};

#endif // CLOUDMANAGER_H
