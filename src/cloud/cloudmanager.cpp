#include "cloudmanager.h"

CloudManager::CloudManager()
{

}

QDateTime CloudManager::lastSync() const {
  return m_lastSync;
}

QDateTime CloudManager::lastRefresh() const {
  return m_lastRefresh;
}

bool CloudManager::noteExists(QUuid syncHash, bool refresh)
{
  if (refresh)
    refreshNotes();
  return m_noteList.contains(syncHash);
}

bool CloudManager::notebookExists(QUuid syncHash, bool refresh)
{
  if (refresh)
    refreshNotebooks();
  return m_notebookList.contains(syncHash);
}

bool CloudManager::tagExists(QUuid syncHash, bool refresh)
{
  if (refresh)
    refreshTags();
  return m_tagList.contains(syncHash);
}

void CloudManager::refresh()
{
  refreshNotes();
  refreshNotebooks();
  refreshTags();
}

void CloudManager::refreshNotes()
{

}

void CloudManager::refreshNotebooks()
{

}

void CloudManager::refreshTags()
{

}
