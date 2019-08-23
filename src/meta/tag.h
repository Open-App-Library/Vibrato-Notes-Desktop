#ifndef TAG_H
#define TAG_H
#include "vibratoobject.h"

class SQLManager;

#define TAG_DEFAULT_TITLE "Untitled Tag"
#define TAG_DEFAULT_FIELDS VIBRATOOBJECT_DEFAULT_FIELDS + \
    QVector<QString>({"row"})
#define TAG_DEFAULT_DB_FIELDS TAG_DEFAULT_FIELDS
#define TAG_DEFAULT_ROW -255

class Tag : public VibratoObject
{
  Q_OBJECT
public:
  Tag(VibratoObjectMap fields = VibratoObjectMap());

  int row() const;
  void setRow(int row);
  void setRowExplicitly(int row);

  QVector<QString> defaultFieldKeys() const override;
  VibratoObjectMap fields() const override;
  virtual int      defaultRow() const;
  QString          defaultTitle() const override;

  void assignFieldsExplicitly(QMap<QString, QVariant> fields) override;

signals:
  void rowChanged(Tag *tag);

private:
  int         m_row;
};

#endif // TAG_H
