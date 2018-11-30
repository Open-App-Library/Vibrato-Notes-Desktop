#ifndef LISTITEMWITHID_H
#define LISTITEMWITHID_H
#include <QListWidgetItem>

#define LIST_ITEM_UNKNOWN -1

class ListItemWithID : public QObject, public QListWidgetItem
{
	Q_OBJECT
public:
	ListItemWithID(QString label, int id);
	int id() const;

public slots:
	void setLabel(QString label);
	void setID(int id);

private:
	int m_id = LIST_ITEM_UNKNOWN;
};

#endif // LISTITEMWITHID_H
