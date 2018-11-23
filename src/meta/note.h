#ifndef NOTE_H
#define NOTE_H
#include <QString>
#include <QDateTime>
#include "notebook.h"
#include "tag.h"

class Note : public QObject
{
	Q_OBJECT

public:
	Note();

	int id() const;
	void setId(int value);

	QString title() const;
	void    setTitle(const QString title);

	QString text() const;
	void    setText(const QString text);

	QDateTime date_created() const;
    QString date_created_str(); // ex. January 26, 1965
    QString date_created_str_informative(); // ex. January 26, 1965 at 12:30pm EST
	void setDate_created(const QDateTime &date_created);

	QDateTime date_modified() const;
    QString date_modified_str(); // ex. 5 minutes ago
    QString date_modified_str_informative(); // ex. January 26, 1965 at 12:30pm EST
	void setDate_modified(const QDateTime &date_modified);

	int  notebook() const;
	void setNotebook(int id);

	QList<int> tags() const;
	void setTags(const QList<int> &value);

signals:
	void noteChanged(Note *note);

private:
	int        m_id;
	QString    m_title;
	QString    m_text;
	QDateTime  m_date_created;
	QDateTime  m_date_modified;
	int        m_notebook;
	QList<int> m_tags;

    QString informativeDate(QDateTime date);
    QString numberToString(int number, bool capitalize=false);

	// TODO: Implement user (shows ID of note owner)
	// TODO: Implement shared_with
	// TODO: Implement is_public
};

#endif // NOTE_H
