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

  enum TypeOfItem {Type_Notebook, Type_Tag};

  bool isNotebook() const;
  bool isTag() const;
  bool isOther() const;

  int id() const;

  QString   label() const;
  void      setLabel(QString label);
  void      updateLabel();

  QIcon icon() const;
  void setIcon(QIcon icon);

  NotebookOrTag object() const;
  void setObjectNotebook(Notebook *notebook);
  void setObjectTag(Tag *tag);

  BasicTreeItem *getChild(int index) const;
  BasicTreeItem *appendChild(BasicTreeItem *child);
  void removeChild(int index);
  QVector<BasicTreeItem*> children() const;
  QVector<BasicTreeItem*> recurseChildren() const;
  int childCount() const;

  BasicTreeItem *parentItem();
  void           setParent(BasicTreeItem *parent);

  int row() const;

private slots:
  void notebookTitleChanged(Notebook *notebook);
  void notebookIDChanged(Notebook *notebook);
  void tagTitleChanged(Tag *tag);
  void tagIDChanged(Tag *tag);

private:
  int m_id; // Grab ID of notebook or tag without accessing object. This is a safety feature.
  QString m_label;
  QIcon m_icon;
  int m_type; // either TYPE_NOTEBOOK or TYPE_TAG
  NotebookOrTag m_object;
  QVector<BasicTreeItem*> m_childItems;
  BasicTreeItem *m_parentItem;
};

#endif // BASICTREEITEM_H
