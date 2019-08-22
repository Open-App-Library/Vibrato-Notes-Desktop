#include "vibratoobject.h"

VibratoObject::VibratoObject(SQLManager *sql_managerm,
                             VibratoObjectMap fields)
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

void VibratoObject::assignFields(QMap<QString, QVariant> fields)
{
    VibratoObjectMapIterator it(fields);
    while (it.hasNext()) {
        it.next();
        QString key = it.key();
        QVariant val = it.value();
        if (key == "uuid") {
            this->setUuidExplicitly(val.toUuid());
        } else if (key == "title") {
            this->setTitleExplicitly(val.toString());
        } else if (key == "date_created") {
            this->setDateCreatedExplicitly(val.toDateTime());
        } else if (key == "date_modified") {
            this->setDateModifiedExplicitly(val.toDateTime());
        } else if (key == "encrypted") {
            this->setEncrypted(val.toBool());
        }
    }
}

void VibratoObject::handleChange()
{
    this->setDateModifiedExplicitly( QDateTime::currentDateTime() );
}
