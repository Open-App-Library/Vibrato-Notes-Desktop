#include "vibratoobject.h"

VibratoObject::VibratoObject(VibratoObjectMap fields)
{
    assignFields(fields);

    connect(this, &VibratoObject::changed,
            this, &VibratoObject::handleChange);
}

VibratoObject::~VibratoObject()
{

}

const QString VibratoObject::title()
{
    return m_title;
}

void VibratoObject::setTitle(QString new_title)
{
    setTitleExplicitly(new_title);
    emit changed();
}

void VibratoObject::setTitleExplicitly(QString new_title)
{
    m_title = new_title;
}

const QUuid VibratoObject::uuid()
{
    return m_uuid;
}

void VibratoObject::setUuidExplicitly(QUuid new_uuid)
{
    m_uuid = new_uuid;
}

const QDateTime VibratoObject::dateCreated()
{
    return m_date_created;
}

void VibratoObject::setDateCreated(QDateTime new_date)
{
    setDateCreatedExplicitly(new_date);
    emit changed();
}

void VibratoObject::setDateCreatedExplicitly(QDateTime new_date)
{
    m_date_created = new_date;
}

const QDateTime VibratoObject::dateModified()
{
    return m_date_modified;
}

void VibratoObject::setDateModifiedExplicitly(QDateTime new_date)
{
    m_date_modified = new_date;
}

bool VibratoObject::encrypted()
{
    return m_encrypted;
}

void VibratoObject::setEncrypted(bool is_encrypted)
{
    setEncryptedExplicitly(is_encrypted);
    emit changed();
}

void VibratoObject::setEncryptedExplicitly(bool is_encrypted)
{
    m_encrypted = is_encrypted;
}

QMap<QString, QVariant> VibratoObject::fields()
{
    VibratoObjectMap map;
    map["uuid"] = uuid();
    map["title"] = title();
    map["date_created"] = dateCreated();
    map["date_modified"] = dateModified();
    map["encrypted"] = encrypted();
    return map;
}

QVector<QString> VibratoObject::field_keys()
{
    return defaultFields();
}

void VibratoObject::assignFields(QMap<QString, QVariant> fields)
{
    assignFieldsExplicitly(fields);
    emit changed();
}

void VibratoObject::assignFieldsExplicitly(QMap<QString, QVariant> fields)
{
    this->setUuidExplicitly(
                fields.contains("uuid") ?
                    fields.value("uuid").toUuid() : defaultUuid());

    this->setTitleExplicitly(
                fields.contains("title") ?
                    fields.value("title").toString() : defaultTitle());

    this->setDateCreatedExplicitly(
                fields.contains("date_created") ?
                    fields.value("date_created").toDateTime() : defaultDateCreated());

    this->setDateModifiedExplicitly(
                fields.contains("date_modified") ?
                    fields.value("date_modified").toDateTime() : defaultDateModified());

    this->setEncrypted(
                fields.contains("encrypted") ?
                    fields.value("encrypted").toBool() : defaultEncrypted());

}

const QVector<QString> VibratoObject::defaultFields()
{
    return VIBRATOOBJECT_DEFAULT_FIELDS;
}

const QUuid VibratoObject::defaultUuid()
{
    return QUuid::createUuid();
}

const QString VibratoObject::defaultTitle()
{
    return VIBRATOOBJECT_DEFAULT_TITLE;
}

const QDateTime VibratoObject::defaultDateCreated()
{
    return VIBRATOOBJECT_DEFAULT_DATE_CREATED;
}

const QDateTime VibratoObject::defaultDateModified()
{
    return VIBRATOOBJECT_DEFAULT_DATE_MODIFIED;
}

bool VibratoObject::defaultEncrypted()
{
    return VIBRATOOBJECT_DEFAULT_ENCRYPTED;
}

void VibratoObject::handleChange()
{
    this->setDateModifiedExplicitly( QDateTime::currentDateTime() );
}
