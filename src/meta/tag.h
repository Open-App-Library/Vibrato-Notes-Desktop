#ifndef TAG_H
#define TAG_H
#include <QObject>
#include <QString>

#define TAG_DEFAULT_TITLE "Untitled Tag"
#define TAG_DEFAULT_TAG_ID -1

class Tag : public QObject
{
  Q_OBJECT
public:
  Tag(int syncId, int id, QString title="Untitled Tag");

  int syncId() const;
  void setSyncId(int syncId);

  int id() const;
  void setId(int id);

  QString title() const;
  void setTitle(const QString &value);

signals:
  void tagChanged(Tag *tag);
  void tagSyncIDChanged(Tag *tag);
  void tagIDChanged(Tag *tag);
  void tagTitleChanged(Tag *tag);

private:
  int     m_syncId;
  int     m_id;
  QString m_title;
};

#endif // TAG_H
