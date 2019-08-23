#ifndef VIBRATOOBJECT_H
#define VIBRATOOBJECT_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QUuid>

typedef QMap<QString, QVariant> VibratoObjectMap;
typedef QMapIterator<QString, QVariant> VibratoObjectMapIterator;

#define VIBRATOOBJECT_DEFAULT_TITLE "Untitled"
#define VIBRATOOBJECT_DEFAULT_FIELDS QVector<QString>({"uuid", "title", "date_created", "date_modified", "encrypted"})
#define VIBRATOOBJECT_DEFAULT_DB_FIELDS VIBRATOOBJECT_DEFAULT_FIELDS
#define VIBRATOOBJECT_DEFAULT_DATE_CREATED QDateTime::currentDateTime();
#define VIBRATOOBJECT_DEFAULT_DATE_MODIFIED QDateTime::currentDateTime();
#define VIBRATOOBJECT_DEFAULT_ENCRYPTED false


class VibratoObject : public QObject
{
    Q_OBJECT
public:
    VibratoObject(VibratoObjectMap fields = VibratoObjectMap());
    virtual ~VibratoObject();

    /*
     * Explicit Functions:
     * -------------------
     * Functions that have "Explictly" tagged
     * onto the end of them DO NOT emit a note
     * changed signal. This ensures that the
     * date modified variable will not get
     * altered.
     */

    QString title() const;
    void setTitle(QString new_title);
    void setTitleExplicitly(QString new_title);

    QUuid uuid() const;
    void setUuidExplicitly(QUuid new_uuid);

    QDateTime dateCreated() const;
    QString dateCreatedStr() const; // ex. January 26, 1965
    QString dateCreatedStrInformative() const; // ex. January 26, 1965 at 12:30pm EST
    void setDateCreated(QDateTime new_date);
    void setDateCreatedExplicitly(QDateTime new_date);

    QDateTime dateModified() const;
    QString dateModifiedStr() const; // ex. 5 minutes ago
    QString dateModifiedStrInformative() const; // ex. January 26, 1965 at 12:30pm EST
    void setDateModifiedExplicitly(QDateTime new_date);

    bool encrypted() const;
    void setEncrypted(bool is_encrypted);
    void setEncryptedExplicitly(bool is_encrypted);

    virtual VibratoObjectMap fields() const;
    QVector<QString> fieldKeys();

    void assignFields(VibratoObjectMap fields);
    virtual void assignFieldsExplicitly(VibratoObjectMap fields);

    virtual QVector<QString> defaultFieldKeys() const;
    virtual QUuid            defaultUuid() const;
    virtual QString          defaultTitle() const;
    virtual QDateTime        defaultDateCreated() const;
    virtual QDateTime        defaultDateModified() const;
    virtual bool             defaultEncrypted() const;

    /*
     * Sorting comparison functions for your convenience.
     */
    static bool byDateCreatedAsc(const VibratoObject *v1, const VibratoObject *v2);
    static bool byDateCreatedDesc(const VibratoObject *v1, const VibratoObject *v2);
    static bool byDateModifiedAsc(const VibratoObject *v1, const VibratoObject *v2);
    static bool byDateModifiedDesc(const VibratoObject *v1, const VibratoObject *v2);

signals:
    void changed(VibratoObject *obj);
    void titleChanged(VibratoObject *obj);
    void dateCreatedChanged(VibratoObject *obj);
    void encryptedChanged(VibratoObject *obj);

private slots:
    void handleChange();

private:
    QUuid       m_uuid;
    QString     m_title;
    QDateTime   m_date_created;
    QDateTime   m_date_modified;
    bool        m_encrypted;

    QString informativeDate(QDateTime date) const;
    static bool compareTwoDateTimes(QDateTime t1, QDateTime t2, char comparisonSymbol);

};

#endif // VIBRATOOBJECT_H
