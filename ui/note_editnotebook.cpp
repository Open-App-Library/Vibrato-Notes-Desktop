#include "note_editnotebook.h"
#include "ui_note_editnotebook.h"
#include "../../models/items/treeitemwithid.h"

#include <QDebug>

Note_EditNotebook::Note_EditNotebook(Database *db, Note *note, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Note_EditNotebook),
    m_db(db),
    m_note(note)
{
    ui->setupUi(this);

    // Button box
    m_selectNotebook = new QPushButton("Select Notebook");
    m_selectNotebook->setEnabled(false);
    ui->buttonBox->addButton(m_selectNotebook, QDialogButtonBox::ActionRole);

    updateTitle();

    connect(note, &Note::noteChanged,
            this, &Note_EditNotebook::noteChanged);
}

Note_EditNotebook::~Note_EditNotebook()
{
    delete m_selectNotebook;
    delete ui;
}

Note *Note_EditNotebook::note()
{
    return m_note;
}

void Note_EditNotebook::noteChanged(void)
{
    updateTitle();
}

void Note_EditNotebook::updateTitle()
{
    if ( m_note == nullptr)
        return;

    setWindowTitle( QString("Editing notebook for \"%1\"").arg(m_note->title()) );
}
