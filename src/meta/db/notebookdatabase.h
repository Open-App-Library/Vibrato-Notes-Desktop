#ifndef NOTEBOOKDATABASE_H
#define NOTEBOOKDATABASE_H
#include <QVector>
#include "../notebook.h"

class NotebookDatabase : public QObject
{
	Q_OBJECT
public:
	NotebookDatabase();
	QVector<Notebook *> list() const;
	int               size() const;
	QVector<Notebook *> listRecursively() const;
	QVector<Notebook *> listRecursively(const QVector<Notebook*> notebookList) const;

	int getNewNotebookId(); // Returns an Id that is safe to use

	Notebook *addNotebook(QString title="New Notebook", Notebook *parent=nullptr, QVector<Notebook*> children={});
	void addNotebook(Notebook *notebook);
	void addNotebook(Notebook *notebook, Notebook *parent);

	void removeNotebook(int index);
	void clearNotebooks();

	Notebook *findNotebookWithID(int id);

	void loadJSON(QJsonDocument jsonDocument);

	void loadDummyNotebooks();

	void jsonObjectToNotebookList(QJsonObject notebookObj, Notebook *parent=nullptr);

private slots:
	void notebookChanged_slot(Notebook *notebook);
	void notebookIDChanged_slot(Notebook *notebook);
	void notebookTitleChanged_slot(Notebook *notebook);
	void notebookParentChanged_slot(Notebook *notebook);
	void notebookChildrenChanged_slot(Notebook *notebook);
signals:
	void notebookAdded(Notebook *notebook);
	void notebookChanged(Notebook *notebook);
	void notebookIDChanged(Notebook *notebook);
	void notebookTitleChanged(Notebook *notebook);
	void notebookParentChanged(Notebook *notebook);
	void notebookChildrenChanged(Notebook *notebook);

private:
	QVector<Notebook*> m_list;

};

#endif // NOTEBOOKDATABASE_H
