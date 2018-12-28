/* "What is this?"
 * It's a quick & easy fuzzy string matching library!
 */

#ifndef DOUGSFUZZYSEARCH_H
#define DOUGSFUZZYSEARCH_H
#include <QString>
#include <QStringList>

class DougsFuzzySearch
{
public:
  DougsFuzzySearch();
  DougsFuzzySearch(QStringList sentenceList);

  QStringList sentenceList() const;
  QStringList sentenceListLower();
  void setSentenceList(QStringList sentenceList);

  QStringList search(QString searchQuery);

  int levenshtein(QString a, QString b, bool ignoreCase=false, bool ignoreSpecialCharacters=false);
  int levenshteinSentences(QString a, QString b, bool ignoreCase=false, bool ignoreSpecialCharacters=false);

private:
  struct m_sentenceObject {
    QString sentence;
    int levScore;
    bool operator<(const m_sentenceObject &obj) {
      return levScore < obj.levScore;
    }
  };

  QStringList m_sentenceList;
  int lev(QString a, QString b, int i, int j);
  QString removeSpecialCharacters(QString string);
  QStringList lowerCaseList(QStringList words);

  bool list_has_changed=false;
  QStringList m_sentenceListLower;
};

#endif // DOUGSFUZZYSEARCH_H
