#ifndef NOTELISTMODEL_H
#define NOTELISTMODEL_H
#include <QAbstractItemModel>
#include <QListView>
#include "items/notelistitem.h"

class NoteListModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	NoteListModel(QListView *view);

	void refresh(int row);
    void clear();
	NoteListItem *prependItem(Note *note);
	NoteListItem *appendItem(Note *note);

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
											int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column,
										const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
	QListView *m_view;
	QVector<NoteListItem*> m_noteItems;

};

#endif // NOTELISTMODEL_H
