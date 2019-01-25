#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "items/basictreeitem.h"


class TreeModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  explicit TreeModel(QObject *parent = nullptr);
  ~TreeModel();

  BasicTreeItem *root() const;

  QVariant data(const QModelIndex &index, int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  QVector<QModelIndex> recurseChildren();

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QModelIndex getItem(BasicTreeItem *item, QModelIndex parent=QModelIndex());

  Qt::DropActions supportedDropActions() const override;
  Qt::DropActions supportedDragActions() const override;

  QStringList mimeTypes() const override;
  QMimeData *mimeData(const QModelIndexList &indexes) const override;
  bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex & parent) override;

  void reOrderRowValues(BasicTreeItem *parent);

signals:
  void gaveNotebookNewParent(BasicTreeItem *parent, BasicTreeItem *child);

private:
  BasicTreeItem *m_rootItem;
  QVector<QModelIndex> m_recurseChildren(QModelIndex parent=QModelIndex());
};

#endif // TREEMODEL_H
