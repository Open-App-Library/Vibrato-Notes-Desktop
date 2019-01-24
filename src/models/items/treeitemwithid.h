#ifndef TREEITEMWITHID_H
#define TREEITEMWITHID_H
#include <QTreeWidgetItem>
#include <QUuid>

class TreeItemWithID : public QObject, public QTreeWidgetItem
{
  Q_OBJECT
public:
  TreeItemWithID(QString label, QUuid syncHash);
  QUuid syncHash() const;

public slots:
  void setSyncHash(QUuid syncHash);
private:
  QUuid m_sync_hash = nullptr;
};

#endif // TREEITEMWITHID_H
