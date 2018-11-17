#ifndef ESCRIBAMANAGER_H
#define ESCRIBAMANAGER_H

#include <QObject>
#include <escriba.h>
#include "../meta/note.h"
#include <ui_escribaaddons.h>

namespace Ui {
    class EscribaAddonsWidget;
}

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
    Ui::EscribaAddonsWidget *m_addons_ui;
	Note *m_curNote = nullptr;

    QLineEdit *m_titleWidget;
    QLineEdit *m_tagsWidget;
    QToolButton *m_notebookWidget;
    QToolButton *m_moreWidget;
    QToolButton *m_trashWidget;
    QLabel *m_dateCreatedWidget;
    QLabel *m_dateModifiedWidget;
};

#endif
