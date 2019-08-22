#ifndef VIBRATOOBJECT_H
#define VIBRATOOBJECT_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QUuid>

#define VIBRATOOBJECT_DEFAULT_TITLE "Untitled"
#define VIBRATOOBJECT_DEFAULT_FIELDS {"uuid", "title", "date_created", "date_modified", "encrypted"}

#define VibratoObjectMap QMap<QString, QVariant>
#define VibratoObjectMapIterator QMapIterator<QString, QVariant>

class SQLManager;

class VibratoObject : public QObject
{
    Q_OBJECT
public:
    VibratoObject(SQLManager *sql_managerm,
                  VibratoObjectMap fields = VibratoObjectMap());
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

    const QString title();
    void setTitle(QString new_title);
    void setTitleExplicitly(QString new_title);

    const QUuid uuid();
    void setUuidExplicitly(QUuid new_uuid);

    const QDateTime dateCreated();
    void setDateCreated(QDateTime new_date);
    void setDateCreatedExplicitly(QDateTime new_date);

    const QDateTime dateModified();
    void setDateModifiedExplicitly(QDateTime new_date);

    bool encrypted();
    void setEncrypted(bool is_encrypted);
    void setEncryptedExplicitly(bool is_encrypted);

    VibratoObjectMap fields();
    void assignFields(VibratoObjectMap fields);

    // SQL Functions
    void save();
    void restore();


signals:
    void changed();

private slots:
    void handleChange();

private:
    SQLManager *m_sql_manager;
    QUuid       m_uuid;
    QString     m_title;
    QDateTime   m_date_created;
    QDateTime   m_date_modified;
    bool        m_encrypted;

};

#endif // VIBRATOOBJECT_H
