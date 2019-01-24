#ifndef BASICTREEITEM_H
#define BASICTREEITEM_H
#include <QVariant>
#include <QIcon>
#include "../../meta/notebook.h"
#include "../../meta/tag.h"

typedef union
{
  Notebook *notebook;
  Tag      *tag;
} NotebookOrTag;

class BasicTreeItem : public QObject
{
  Q_OBJECT
public:
  explicit BasicTreeItem(const QString label, BasicTreeItem *parent = nullptr);
  explicit BasicTreeItem(Notebook *notebook, BasicTreeItem *parent = nullptr);
  explicit BasicTreeItem(Tag *tag, BasicTreeItem *parent = nullptr);

  ~BasicTreeItem();

  enum TypeOfItem {Type_Notebook, Type_Tag, Type_SearchQuery, Type_NotebooksLabel, Type_TagsLabel, Type_Other};

  bool isNotebook() const;
  bool isTag() const;
  bool isSearchQuery() const;
  bool isNotebooksLabel() const;
  bool isTagsLabel() const;
  bool isOther() const;

  bool selectable() const;
  void setSelectable(bool selectable=true);

  QUuid syncHash() const;

  QString   label() const;
  void      setLabel(QString label);
  void      updateLabel();

  QIcon icon() const;
  void setIcon(QIcon icon);

  NotebookOrTag object() const;
  void setObjectNotebook(Notebook *notebook);
  void setObjectTag(Tag *tag);

  // For items with a type of Type_SearchQuery
  QString searchQuery() const;
  void setSearchQuery(QString searchQuery);

  void setIsNotebooksLabel();
  void setIsTagsLabel();

  BasicTreeItem *getChild(int index) const;
  BasicTreeItem *appendChild(BasicTreeItem *child);
  void removeChild(int index);
  void removeChild(BasicTreeItem *item);
  void moveChild(int row, int newRow);
  void moveChild(BasicTreeItem *item, int newRow);
  QVector<BasicTreeItem*> children() const;
  QVector<BasicTreeItem*> recurseChildren() const;
  int childCount() const;

  BasicTreeItem *parentItem();
  void           setParent(BasicTreeItem *parent);

  int row() const;

private slots:
  void notebookTitleChanged(Notebook *notebook);
  void notebookSyncHashChanged(Notebook *notebook);
  void tagTitleChanged(Tag *tag);
  void tagSyncHashChanged(Tag *tag);

private:
  QUuid m_sync_hash; // Grab ID of notebook or tag without accessing object. This is a safety feature.
  QString m_label;
  QIcon m_icon;
  int m_type; // Type_Notebook, Type_Tag, etc.
  bool m_selectable=true;
  NotebookOrTag m_object;
  QVector<BasicTreeItem*> m_childItems;
  BasicTreeItem *m_parentItem;
  QString m_searchQuery = "";
};

#endif // BASICTREEITEM_H
