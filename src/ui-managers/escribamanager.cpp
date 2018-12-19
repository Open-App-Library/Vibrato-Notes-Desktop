#include "escribamanager.h"
#include <QDebug>
#include "../models/items/treeitemwithid.h"

EscribaManager::EscribaManager(Escriba *editor, Database *db, Manager *manager) :
  m_editor(editor),
  m_db(db),
  m_manager(manager),
  m_addons_ui(new Ui::EscribaAddonsWidget)
{
  m_addons_ui->setupUi( editor->addonsArea() );
  m_titleWidget    = m_addons_ui->f_noteTitle;
  m_tagsInputWidget     = m_addons_ui->f_noteTagsInput;
  m_tagsViewerWidget     = m_addons_ui->f_noteTagsViewer;
  m_notebookWidget = m_addons_ui->f_noteNotebook;
  m_favoriteButton = m_addons_ui->f_favoriteButton;
  m_trashButton    = m_addons_ui->f_noteTrash;
  m_moreWidget     = m_addons_ui->f_noteMore;
  m_dateCreatedWidget = m_addons_ui->f_noteDateCreated;
  m_dateModifiedWidget = m_addons_ui->f_noteDateModified;

  connect(editor, &Escriba::documentChanged,
          this, &EscribaManager::contentChangedFromEditor);
  connect(m_titleWidget, &QLineEdit::textChanged,
          this, &EscribaManager::titleChangedFromEditor);
  connect(m_titleWidget, &QLineEdit::returnPressed,
          this, &EscribaManager::focusEditor);
  connect(m_tagsInputWidget, &QLineEdit::returnPressed,
          this, &EscribaManager::addTag);
  connect(m_notebookWidget, &QToolButton::clicked,
          this, &EscribaManager::openNotebookEditor);
  connect(m_favoriteButton, &QToolButton::clicked,
          this, &EscribaManager::toggleFavorited);
  connect(m_trashButton, &QToolButton::clicked,
          this, &EscribaManager::trashNote);
  connect(m_tagsViewerWidget, &QToolButton::clicked,
          this, &EscribaManager::openTagsEditor);
  connect(m_db->noteDatabase(), &NoteDatabase::noteDeleted,
          this, &EscribaManager::aNoteWasRemoved);
  connect(m_db->notebookDatabase(), &NotebookDatabase::notebooksRemoved,
          this, &EscribaManager::notebooksRemoved);

  // Deselect by default
  deselect();
}

void EscribaManager::updateTagsButtonCounter()
{
  m_tagsViewerWidget->setText( QString("Manage Tags (%1)").arg( m_curNote->tags().count() ) );
}

Note *EscribaManager::note()
{
  return m_curNote;
}

void EscribaManager::setNote( Note *note )
{
  if ( note == m_curNote)
    return;

  ///
  // Save current note and disconnect signals/slots
  ///
  bool curNoteExists = m_db->noteDatabase()->noteWithIDExists(m_id) && m_id != -1;
  if (m_curNote != nullptr && curNoteExists ) {
    m_curNote->setTitle( m_titleWidget->text() );
    m_curNote->setText( m_editor->toMarkdown() );
    disconnect(m_curNote, &Note::noteChanged,
               this, &EscribaManager::noteChanged);
    disconnect(m_curNote, &Note::noteIdChanged,
               this, &EscribaManager::noteIDChanged);
    disconnect(m_curNote, &Note::noteNotebookChanged,
               this, &EscribaManager::noteNotebookChanged);
    disconnect(m_curNote, &Note::noteFavoritedChanged,
               this, &EscribaManager::updateFavoriteButton);
    disconnect(m_curNote, &Note::noteTrashedOrRestored,
            this, &EscribaManager::updateTrashButton);
  } else if (!curNoteExists) {
    m_curNote = nullptr;
    m_id = -1;
  }

  if ( m_curNotebook != nullptr )
    disconnect(m_curNotebook, &Notebook::notebookChanged,
            this, &EscribaManager::notebookChanged);

  ///
  // Open the new note
  ///
  m_curNote = note;

  if ( m_curNote == nullptr )
    return;
  else {
    if ( !m_curNote->trashed() ) {
      m_editor->setDisabled(false);
      m_editor->setToolTip("");
    } else {
      m_editor->setDisabled(true);
      m_editor->setToolTip(tr("You may not edit a notebook that is in the trash. Restore it to edit it."));
    }
  }

  // Change to requested note
  m_curNote = note;
  m_id = m_curNote->id();
  connect(m_curNote, &Note::noteChanged,
          this, &EscribaManager::noteChanged);
  connect(m_curNote, &Note::noteNotebookChanged,
          this, &EscribaManager::noteNotebookChanged);
  connect(m_curNote, &Note::noteIdChanged,
          this, &EscribaManager::noteIDChanged);
  connect(m_curNote, &Note::noteFavoritedChanged,
          this, &EscribaManager::updateFavoriteButton);
  connect(m_curNote, &Note::noteTrashedOrRestored,
          this, &EscribaManager::updateTrashButton);
  m_titleWidget->setText(note->title());
  m_editor->setMarkdown(note->text());
  updateTagsButtonCounter();

  // Setting up notebook
  Notebook *notebook = m_db->notebookDatabase()->findNotebookWithID(m_curNote->notebook());
  m_curNotebook = notebook;

  if ( m_curNotebook != nullptr ) {
    m_notebook_id = m_curNotebook->id();
    connect(m_curNotebook, &Notebook::notebookChanged,
            this, &EscribaManager::notebookChanged);
  } else m_notebook_id = NOTEBOOK_DEFAULT_NOTEBOOK_ID;

  updateNotebookWidget();
  updateFavoriteButton();
  updateDateWidgets();
  updateTrashButton();

  QString created = "<strong>Created:</strong> %1";
  QString modified = "<strong>Modified:</strong> %1";
  //m_dateCreatedWidget->setText( created.arg( m_curNote->date_created().toString() ) );

  emit selectedNewNote(note);
}

void EscribaManager::deselect() {
  setNote(nullptr);
  m_titleWidget->setText("Untitled");
  m_editor->setMarkdown("");
  m_editor->setDisabled(true);
  emit deselected();
}

void EscribaManager::contentChangedFromEditor(QString markdown)
{
  if (!m_curNote)
    return;
  if (!QString::compare(m_curNote->text(), markdown)) // If titles are same, exit
    return;
  m_curNote->setText(markdown);
}

void EscribaManager::titleChangedFromEditor(QString title)
{
  if (!m_curNote)
    return;
  if (!QString::compare(m_curNote->title(), title)) // If titles are same, exit
    return;
  m_curNote->setTitle( title );
}

void EscribaManager::addTag()
{
  m_db->addTagToNote(m_curNote, m_tagsInputWidget->text());
  updateTagsButtonCounter();
  m_tagsInputWidget->clear();
}

void EscribaManager::openNotebookEditor()
{
  if ( m_curNote == nullptr)
    return;

  // If dialog is not a nullptr and its note ID is not equal to the new note, delete it.
  if (m_editNotebookDialog != nullptr && m_editNotebookDialog->note()->id() != m_curNote->id()) {
    delete m_editNotebookDialog;
    m_editNotebookDialog = nullptr;
  }

  if (m_editNotebookDialog == nullptr) {
    m_editNotebookDialog = new Note_EditNotebook(m_db, m_manager, m_curNote, m_notebookWidget);
  }

  m_editNotebookDialog->show();
}

void EscribaManager::openTagsEditor()
{
  if ( m_curNote == nullptr)
    return;

  // If dialog is not a nullptr and its note ID is not equal to the new note, delete it.
  if (m_editTagsDialog != nullptr && m_editTagsDialog->note()->id() != m_curNote->id()) {
    delete m_editNotebookDialog;
    m_editTagsDialog = nullptr;
  }

  if (m_editTagsDialog == nullptr) {
    m_editTagsDialog = new Note_EditTags(m_db, m_curNote, m_tagsViewerWidget);
  }

  m_editTagsDialog->show();
}

void EscribaManager::focusEditor()
{
  m_editor->focusEditor();
}

void EscribaManager::noteChanged()
{
  updateTagsButtonCounter();
  updateDateWidgets();
}

void EscribaManager::updateNotebookWidget(void)
{
  // Adjust notebook selector widget
  if ( m_curNote != nullptr && m_curNote->notebook() > 0 ) {
    if ( m_curNotebook != nullptr ) {
      m_notebookWidget->setText( m_curNotebook->title() );
      // If notebook has parents, append a nice little hierarchy indication
      if ( m_curNotebook->parent() != nullptr ) {
        Notebook *parent = m_curNotebook->parent();
        while (parent != nullptr) {
          m_notebookWidget->setText( QString(parent->title() + " → ").append( m_notebookWidget->text() ) );
          parent = parent->parent();
        }
      }
    }
    return;
  }
  m_notebookWidget->setText( "Default Notebook" );
}

void EscribaManager::updateDateWidgets(void)
{
  if ( m_curNote == nullptr )
    return;
  //QString cur = m_curNote->date_created().toString()
  m_dateCreatedWidget->setText( "<strong>Created:</strong> " + m_curNote->date_created_str() );
  m_dateCreatedWidget->setToolTip( m_curNote->date_created_str_informative() );

  m_dateModifiedWidget->setText( "<strong>Modified:</strong> " + m_curNote->date_modified_str() );
  m_dateModifiedWidget->setToolTip( m_curNote->date_modified_str_informative() );
}

void EscribaManager::updateFavoriteButton(void) {
  if ( m_curNote->favorited() )
    m_favoriteButton->setIcon( QIcon::fromTheme("vibrato-draw-star-solid") );
  else
    m_favoriteButton->setIcon( QIcon::fromTheme("vibrato-draw-star") );
  m_favoriteButton->setChecked( m_curNote->favorited() );
}

void EscribaManager::updateTrashButton(void) {
  // If note is trashed, disable trash button
  // If note is not trashed, enable trash button
  bool trashed = m_curNote->trashed();
  m_trashButton->setEnabled( !trashed );
  if ( trashed )
    m_trashButton->setToolTip(tr("This note has been trashed."));
  else
    m_trashButton->setToolTip(tr("Move this note to the trash."));
}

void EscribaManager::toggleFavorited() {
  m_curNote->setFavorited( !m_curNote->favorited() );
  updateFavoriteButton();
}

void EscribaManager::trashNote() {
  if ( m_curNote->trashed() )
    return;
  m_curNote->setTrashed(true);
  updateTrashButton();
  deselect();
}

void EscribaManager::aNoteWasRemoved(int noteID) {
  if ( m_id == noteID ) {
    m_id = -1;
    m_curNote = nullptr; // Set to nullptr since note is deleted.
    m_curNotebook = nullptr;
    deselect();
  }
}

void EscribaManager::notebooksRemoved(QVector<int> notebookIDs) {
  if ( notebookIDs.contains( m_notebook_id ))
    m_curNotebook = nullptr;
}

void EscribaManager::noteIDChanged(Note* note) {
  m_id = note->id();
}

void EscribaManager::notebookChanged(Notebook *notebook)
{
  m_notebook_id = notebook->id();
  updateNotebookWidget();
}

void EscribaManager::noteNotebookChanged(Note *note) {
  m_curNotebook = m_db->notebookDatabase()->findNotebookWithID( note->notebook() );
  updateNotebookWidget();
}
