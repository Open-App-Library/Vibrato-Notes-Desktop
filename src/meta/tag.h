#ifndef TAG_H
#define TAG_H
#include <QObject>
#include <QString>

class Tag : public QObject
{
	Q_OBJECT
public:
	Tag(int id, QString title="Untitled Tag");
	int id() const;
	void setId(int id);

	QString title() const;
	void setTitle(const QString &value);

signals:
	void tagChanged(Tag *tag);
	void tagIDChanged(Tag *tag);
	void tagTitleChanged(Tag *tag);

private:
	int     m_id;
	QString m_title;
};

#endif // TAG_H
