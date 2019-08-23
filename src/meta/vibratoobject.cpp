#include "vibratoobject.h"
#include "helper-io.hpp"

VibratoObject::VibratoObject(VibratoObjectMap fields)
{
    assignFieldsExplicitly(fields);

    connect(this, &VibratoObject::changed,
            this, &VibratoObject::handleChange);
}

VibratoObject::~VibratoObject()
{
}

QString VibratoObject::title() const
{
    return m_title;
}

void VibratoObject::setTitle(QString new_title)
{
    if ( new_title.isEmpty() ||
         m_title == new_title.trimmed())
      return;

    setTitleExplicitly(new_title);
    emit titleChanged(this);
    emit changed(this);
}

void VibratoObject::setTitleExplicitly(QString new_title)
{
    m_title = new_title.trimmed();
}

QUuid VibratoObject::uuid() const
{
    return m_uuid;
}

void VibratoObject::setUuidExplicitly(QUuid new_uuid)
{
    m_uuid = new_uuid;
}

QDateTime VibratoObject::dateCreated() const
{
    return m_date_created;
}

QString VibratoObject::dateCreatedStr() const
{
    return m_date_created.toString("MMMM d, yyyy");
}

QString VibratoObject::dateCreatedStrInformative() const
{
    return informativeDate(m_date_created);
}

void VibratoObject::setDateCreated(QDateTime new_date)
{
    if ( new_date == m_date_created )
        return;
    setDateCreatedExplicitly(new_date);
    emit dateCreatedChanged(this);
    emit changed(this);
}

void VibratoObject::setDateCreatedExplicitly(QDateTime new_date)
{
    m_date_created = new_date;
}

QDateTime VibratoObject::dateModified() const
{
    return m_date_modified;
}

QString VibratoObject::dateModifiedStr() const
{
#ifdef UNIT_TEST
    QDateTime currentDateTime = QDateTime::fromString("2000-12-25T09:38:59Z", Qt::ISODate);
#else
    QDateTime currentDateTime = QDateTime::currentDateTime();
#endif

    // Time difference in seconds
    int td_sec = static_cast<int>(currentDateTime.toSecsSinceEpoch() - m_date_modified.toSecsSinceEpoch());
    int secs_in_minute = 60;
    int secs_in_hour   = 3600;
    int secs_in_year   = 31557600;

    int amount = 0;
    QString unit = ""; // years, months, hours, minutes,
    int diviser = 1;

    if (td_sec < secs_in_minute)
        return "Just now";
    else if ( td_sec >= secs_in_year ) {
        unit = "year";
        diviser = secs_in_year;
    }
    else if ( currentDateTime.date().month() != m_date_modified.date().month() ) {
        int m = m_date_modified.date().month();
        int months_since = 0;
        while ( m != currentDateTime.date().month() ) {
            months_since++;
            if ( m == 12 )
                m = 1;
            else
                m++;
        }
        unit = "month";
        if ( months_since > 1 )
            unit.append('s');
        return QString("%1 %2 ago").arg(HelperIO::numberToString(months_since, true), unit);
    }
    else if ( td_sec >= secs_in_hour ) {
        unit = "hour";
        diviser = secs_in_hour;
    }
    else if ( td_sec >= secs_in_minute ) {
        unit = "minute";
        diviser = secs_in_minute;
    }
    amount = td_sec / diviser;
    if ( amount > 1 )
        unit.append('s');
    return QString("%1 %2 ago").arg(HelperIO::numberToString(amount, true), unit);
}

QString VibratoObject::dateModifiedStrInformative() const
{
    return informativeDate( m_date_modified );
}

void VibratoObject::setDateModifiedExplicitly(QDateTime new_date)
{
    m_date_modified = new_date;
}

bool VibratoObject::encrypted() const
{
    return m_encrypted;
}

void VibratoObject::setEncrypted(bool is_encrypted)
{
    if ( m_encrypted == is_encrypted )
        return;
    setEncryptedExplicitly(is_encrypted);
    emit encryptedChanged(this);
    emit changed(this);
}

void VibratoObject::setEncryptedExplicitly(bool is_encrypted)
{
    m_encrypted = is_encrypted;
}

QMap<QString, QVariant> VibratoObject::fields() const
{
    VibratoObjectMap map;
    map["uuid"] = uuid();
    map["title"] = title();
    map["date_created"] = dateCreated();
    map["date_modified"] = dateModified();
    map["encrypted"] = encrypted();
    return map;
}

QVector<QString> VibratoObject::fieldKeys()
{
    return defaultFieldKeys();
}

void VibratoObject::assignFields(QMap<QString, QVariant> fields)
{
    assignFieldsExplicitly(fields);
    emit changed(this);
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

QVector<QString> VibratoObject::defaultFieldKeys() const
{
    return VIBRATOOBJECT_DEFAULT_FIELDS;
}

QUuid VibratoObject::defaultUuid() const
{
    return QUuid::createUuid();
}

QString VibratoObject::defaultTitle() const
{
    return VIBRATOOBJECT_DEFAULT_TITLE;
}

QDateTime VibratoObject::defaultDateCreated() const
{
    return VIBRATOOBJECT_DEFAULT_DATE_CREATED;
}

QDateTime VibratoObject::defaultDateModified() const
{
    return VIBRATOOBJECT_DEFAULT_DATE_MODIFIED;
}

bool VibratoObject::byDateCreatedAsc(const VibratoObject *v1, const VibratoObject *v2)
{
    return compareTwoDateTimes( v1->dateCreated(), v2->dateCreated(), '<' );
}

bool VibratoObject::byDateCreatedDesc(const VibratoObject *v1, const VibratoObject *v2)
{
    return compareTwoDateTimes( v1->dateCreated(), v2->dateCreated(), '>' );
}

bool VibratoObject::byDateModifiedAsc(const VibratoObject *v1, const VibratoObject *v2)
{
    return compareTwoDateTimes( v1->dateModified(), v2->dateModified(), '<' );
}

bool VibratoObject::byDateModifiedDesc(const VibratoObject *v1, const VibratoObject *v2)
{
    return compareTwoDateTimes( v1->dateModified(), v2->dateModified(), '>' );
}

bool VibratoObject::defaultEncrypted() const
{
    return VIBRATOOBJECT_DEFAULT_ENCRYPTED;
}

void VibratoObject::handleChange()
{
    this->setDateModifiedExplicitly( QDateTime::currentDateTime() );
}

QString VibratoObject::informativeDate(QDateTime date) const
{
    QString dateStr = date.toString("MMMM d, yyyy");
    QString timeStr = date.toString("h:mA t");
    return QString("%1 at %2").arg(dateStr, timeStr);
}

bool VibratoObject::compareTwoDateTimes(QDateTime t1, QDateTime t2, char comparisonSymbol)
{
    qint64 d1 = t1.toMSecsSinceEpoch();
    qint64 d2 = t2.toMSecsSinceEpoch();
    if ( comparisonSymbol == '>' && d1 > d2 ) {
        return true;
    } else if ( comparisonSymbol == '<' && d1 < d2 )
        return true;
    return false;
}
