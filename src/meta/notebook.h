#ifndef NOTEBOOK_H
#define NOTEBOOK_H
#include <QString>
#include <QVector>
#include <QObject>
#include <QUuid>
#include <QDateTime>
#include "vibratoobject.h"

class SQLManager;

#define NOTEBOOK_DEFAULT_FIELDS VIBRATOOBJECT_DEFAULT_FIELDS + \
    QVector<QString>({"parent", "children", "row"})
#define NOTEBOOK_DEFAULT_TITLE "Untitled Notebook"
#define NOTEBOOK_DEFAULT_PARENT nullptr
#define NOTEBOOK_DEFAULT_CHILDREN {}
#define NOTEBOOK_DEFAULT_ROW -255

class Notebook : public VibratoObject
{
  Q_OBJECT
public:
  Notebook(VibratoObjectMap fields = VibratoObjectMap());


  Notebook *parent() const;
  void setParent(Notebook *parent);
  void setParentExplicitly(Notebook *parent);

  QVector<Notebook *> children() const;
  QVector<Notebook *> recurseChildren(Notebook* parent=nullptr) const;
  void setChildren(const QVector<Notebook *> &children);

  void appendChild(Notebook *child);
  void appendChildExplicitly(Notebook *child);

  void removeChild(Notebook *child);
  void removeChildExplicitly(Notebook *child);

  int row() const;
  void setRow(int row);
  void setRowExplicitly(int row);

  // Returns true if default notebook
  bool isDefaultNotebook() const;

  static Notebook *createBlankNotebook();

  void assignFieldsExplicitly(QMap<QString, QVariant> fields) override;
  VibratoObjectMap fields() const override;

  QVector<QString>   defaultFieldKeys() const override;
  Notebook*          defaultParent() const;
  QVector<Notebook*> defaultChildren() const;
  int                defaultRow() const;
  QString            defaultTitle() const override;

signals:
  // General change signal - When any property is changed.
  void changed(Notebook *notebook);

  // Property signals
  void parentChanged(Notebook *notebook);
  void childrenChanged(Notebook *notebook);
  void rowChanged(Notebook *notebook);

private:
  Notebook *m_parent;
  QVector<Notebook*> m_children;
  int m_row;
};

#endif // NOTEBOOK_H
