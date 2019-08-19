#ifndef LISTITEMWITHID_H
#define LISTITEMWITHID_H
#include <QListWidgetItem>
#include <QUuid>

class ListItemWithID : public QObject, public QListWidgetItem
{
	Q_OBJECT
public:
  ListItemWithID(QString label, QUuid syncHash);
	int id() const;
  QUuid syncHash() const;

public slots:
	void setLabel(QString label);
  void setSyncHash(QUuid syncHash);

private:
  QUuid m_sync_hash = nullptr;
};

#endif // LISTITEMWITHID_H
