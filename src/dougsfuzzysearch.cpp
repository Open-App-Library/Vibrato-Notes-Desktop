#include "dougsfuzzysearch.h"
#include <QDebug>
#include <QRegularExpression>
#include <iostream>

DougsFuzzySearch::DougsFuzzySearch()
{

}

DougsFuzzySearch::DougsFuzzySearch(QStringList sentenceList)
{
  setSentenceList(sentenceList);
}

QStringList DougsFuzzySearch::sentenceList() const
{
  return m_sentenceList;
}

QStringList DougsFuzzySearch::sentenceListLower()
{
  if ( list_has_changed ) {
    m_sentenceListLower = lowerCaseList(m_sentenceList);
    list_has_changed = false;
  }
  return m_sentenceListLower;
}

void DougsFuzzySearch::setSentenceList(QStringList sentenceList)
{
  sentenceList.removeDuplicates();
  m_sentenceList = sentenceList;
  list_has_changed = true;
}

QStringList DougsFuzzySearch::search(QString searchQuery)
{
  QStringList words = lowerCaseList(searchQuery.split(" "));
  words.removeDuplicates();
  QVector<m_sentenceObject> sentenceObjects;
  for (QString sentence: sentenceListLower()) {
    int levScore=0;
    for (QString i : words) {
      int lowestLev = -1;
      for (QString j: sentence.split(" ")) {
        int lev = levenshtein(i, j);
        if (lev < lowestLev || lowestLev == -1)
          lowestLev = lev;
        // IDEA: Factor in string.count(str, case-sensitiviy)
        levScore += lowestLev;
      }
      m_sentenceObject obj = {sentence, levScore};
      sentenceObjects.append(obj);
    }
  }
  std::sort(sentenceObjects.begin(), sentenceObjects.end());

  QStringList returnList;
  for (m_sentenceObject obj : sentenceObjects)
    returnList << obj.sentence;
  return returnList;
}

int DougsFuzzySearch::levenshtein(QString a, QString b, bool ignoreCase, bool ignoreSpecialCharacters)
{
  if (ignoreCase) {
    a = a.toLower();
    b = b.toLower();
  }
  if (ignoreSpecialCharacters) {
    a = removeSpecialCharacters(a);
    b = removeSpecialCharacters(b);
  }
  return lev(a, b, a.length(), b.length());
}

int DougsFuzzySearch::levenshteinSentences(QString a, QString b, bool ignoreCase, bool ignoreSpecialCharacters)
{
  QStringList aWords = a.split(" ");
  QStringList bWords = b.split(" ");
  int totalLev = 0;
  for (QString aWord : aWords) {
    int smallestLev = -1;
    for (QString bWord : bWords) {
      if (bWord.length() > aWord.length())
        bWord = bWord.chopped(aWord.length());
      int lev = levenshtein(aWord, bWord, ignoreCase, ignoreSpecialCharacters);
      if (lev < smallestLev || smallestLev == -1)
        smallestLev = lev;
    }
    totalLev += smallestLev;
  }
  return totalLev;
}

int DougsFuzzySearch::lev(QString a, QString b, int i, int j)
{
  if ( qMin(i, j) == 0)
    return qMax(i, j);

  return qMin(qMin(lev(a, b, i-1, j)+1,
                   lev(a, b, i, j-1)+1),
              lev(a, b, i-1, j-1) + (a.at(i-1) != b.at(j-1) ? 1 : 0));
}

QString DougsFuzzySearch::removeSpecialCharacters(QString string)
{
  // Very limited functionality at the moment.
  return string.remove(QRegularExpression("[\\^$]+"));
}

QStringList DougsFuzzySearch::lowerCaseList(QStringList words)
{
  QStringList list;
  for (QString word : words)
    list.append(word.toLower());
  return list;
}
