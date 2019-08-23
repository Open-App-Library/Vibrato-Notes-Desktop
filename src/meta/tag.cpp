#include "tag.h"

Tag::Tag(VibratoObjectMap fields) :
    VibratoObject (fields)
{
    assignFieldsExplicitly(fields);
}

int Tag::row() const
{
  return m_row;
}

void Tag::setRow(int row)
{
  if ( row == m_row ) return;
  setRowExplicitly(row);
  emit rowChanged(this);
  emit changed(this);
}

void Tag::setRowExplicitly(int row)
{
    m_row = row;
}

QVector<QString> Tag::defaultFieldKeys() const
{
    return TAG_DEFAULT_FIELDS;
}

VibratoObjectMap Tag::fields() const
{
    VibratoObjectMap fields = VibratoObject::fields();
    fields["row"] = row();
    return fields;
}

int Tag::defaultRow() const
{
    return TAG_DEFAULT_ROW;
}

QString Tag::defaultTitle() const
{
    return TAG_DEFAULT_TITLE;
}

void Tag::assignFieldsExplicitly(QMap<QString, QVariant> fields)
{
    VibratoObject::assignFieldsExplicitly(fields);

    this->setRowExplicitly(
                fields.contains("row") ?
                    fields.value("row").toInt() : defaultRow());
}
