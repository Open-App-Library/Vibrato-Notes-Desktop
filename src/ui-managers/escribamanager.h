#ifndef ESCRIBAMANAGER_H
#define ESCRIBAMANAGER_H

#include <QObject>
#include <escriba.h>
#include "../meta/note.h"

class EscribaManager : public QObject
{
	Q_OBJECT

public:
	EscribaManager(Escriba *editor);

	void setNote( Note *note );
	Note *note();

public slots:
    void contentChangedFromEditor(QString markdown);
	void titleChangedFromEditor(QString title);

private:
	Escriba *m_editor;
	Note *m_curNote = nullptr;
};

#endif
