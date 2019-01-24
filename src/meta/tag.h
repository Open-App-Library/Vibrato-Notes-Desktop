#ifndef TAG_H
#define TAG_H
#include <QObject>
#include <QString>
#include <QUuid>
#include <QDateTime>

#define TAG_DEFAULT_TITLE "Untitled Tag"

class Tag : public QObject
{
  Q_OBJECT
public:
  Tag(QUuid     sync_hash     = QUuid::createUuid(),
      QString   title         = TAG_DEFAULT_TITLE,
      QDateTime date_modified = QDateTime::currentDateTime(),
      int       row           = -255,
      bool      encrypted     = false);

  // Sync Hash
  QUuid syncHash() const;
  void setSyncHash(QUuid syncHash);

  // Title
  QString title() const;
  void setTitle(const QString value);

  // Date Modified
  QDateTime dateModified() const;
  void setDateModified(QDateTime dateModified, bool emitChangeSignal=true);

  // Row
  int row() const;
  void setRow(int row);

  // Encrypted
  bool encrypted() const;
  void setEncrypred(bool encrypted);

signals:
  // General change function - whenever any property is changed
  void changed(Tag *tag);

  // Property signals
  void syncHashChanged(Tag *tag);
  void titleChanged(Tag *tag);
  void dateModifiedChanged(Tag *tag);
  void rowChanged(Tag *tag);
  void encryptedChanged(Tag *tag);

private slots:
  void handleChange(Tag *tag);

private:
  QUuid     m_sync_hash;
  QString   m_title;
  QDateTime m_date_modified;
  int       m_row;
  bool      m_encrypted;

};

#endif // TAG_H
