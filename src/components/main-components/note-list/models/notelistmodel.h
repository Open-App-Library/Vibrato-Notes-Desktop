#ifndef NOTELISTMODEL_H
#define NOTELISTMODEL_H
#include <QAbstractItemModel>
#include <QListView>
#include "items/notelistitem.h"
#include "../meta/db/notedatabase.h"

class NoteListModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  NoteListModel(QListView *view, NoteDatabase *noteDatabase);

  QVector<NoteListItem*> noteItems() const;

  void refresh(int row);
  void clear();
  NoteListItem *prependItem(Note *note);
  NoteListItem *appendItem(Note *note);

  void noteDateChanged(NoteListItem *item);

  bool insertRows(int position, int rows,
                  const QModelIndex &parent = QModelIndex()) override;
  bool removeRows(int position, int rows,
                  const QModelIndex &parent = QModelIndex()) override;

  QVariant data(const QModelIndex &index, int role) const override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  Note *noteFromIndex(QModelIndex index);

  void removeNoteItemWithSyncHash(QUuid syncHash);

private:
  QListView *m_view;
  QVector<NoteListItem*> m_noteItems;
  NoteDatabase *m_noteDatabase;
};

#endif // NOTELISTMODEL_H
