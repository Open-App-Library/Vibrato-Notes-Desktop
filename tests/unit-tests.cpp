/*
 * Main Unit Test File.
 * Needs work.
 */

#define UNIT_TEST
#define private public
#include "../src/meta/note.h"
#include "../src/dougsfuzzysearch.h"
#include <helper-io.hpp>
#define private private

#include <QtTest>
#include <QCoreApplication>
#include <QDebug>

class GenericTest : public QObject
{
  Q_OBJECT

private slots:
  void numberToStringWord();
  void dateFormatting();
  void dougsFuzzySearch();

private:
  QDateTime isoDate(QString str);

};
//void GenericTest::toUpper()
//{
//    QString str = "hello";
//    //QVERIFY(str.toUpper() == "HEfLLO");
//    QCOMPARE(str.toUpper(), "ehe");
//}


void GenericTest::numberToStringWord()
{
  QStringList words = {"one", "two", "three", "four", "five", "six",
                       "seven", "eight", "nine", "ten"};
  for (int i = 1; i <= 10; i++)
    QCOMPARE(HelperIO::numberToString(i), words.at(i-1));

  QCOMPARE(HelperIO::numberToString(11), "11");
}

void GenericTest::dateFormatting()
{
  Note *note = new Note(1, "Test Note", "",
                        isoDate( "2000-01-01T09:38:59Z"), // Date Created
                        QDateTime::currentDateTime(),  // Date Modified
                        false, -1, {}, false);
  // For unit tests, the current date will always be:
  //                              2000-12-25T09:38:59Z
  QCOMPARE(note->date_created_str(), "January 1, 2000");
  QCOMPARE(note->date_created_str_informative(), "January 1, 2000 at 9:38AM UTC");

  QCOMPARE(note->date_modified_str(), "Just now");

  note->setDate_modified( isoDate("2000-12-25T09:36:59Z") ); // two minutes ago
  QCOMPARE(note->date_modified_str(), "Two minutes ago");

  note->setDate_modified( isoDate("2000-12-25T09:26:59Z") ); // twelve minutes ago
  QCOMPARE(note->date_modified_str(), "12 minutes ago");

  note->setDate_modified( isoDate("2000-11-01T09:38:59Z") ); // one month ago
  QCOMPARE(note->date_modified_str(), "One month ago");

  note->setDate_modified( isoDate("1999-12-25T09:38:59Z") ); // one year ago
  QCOMPARE(note->date_modified_str(), "One year ago");

  note->setDate_modified( isoDate("1989-12-25T09:38:59Z") ); // eleven years ago
  QCOMPARE(note->date_modified_str(), "11 years ago");

  // Invalid date scenerio where modified date is later than current
  note->setDate_modified( isoDate("2001-01-01T09:38:59Z") );
  QCOMPARE(note->date_modified_str(), "Just now"); // "Just now" is possibly the safest response we could expect.

}

void GenericTest::dougsFuzzySearch()
{
  DougsFuzzySearch *doug = new DougsFuzzySearch({"The old man and the sea",
                                                "The young man and the sea",
                                                "animal farm",
                                                "Zero dark thirty",
                                                "the third door",
                                                "eat sleep repeat",
                                                "eat sleep rave repeat",
                                                "wolf man eats a bone"});

  // Test the levenshtein function
  QCOMPARE(doug->levenshtein("cat", "Cat"), 1);
  QCOMPARE(doug->levenshtein("caT", "Cat"), 2);
  QCOMPARE(doug->levenshtein("puppy", "cat"), 5);

  // Ignoring case tests
  QCOMPARE(doug->levenshtein("dog", "DOG"), 3);
  QCOMPARE(doug->levenshtein("dog", "DOG", true), 0);

  // Ignoring special characters
  QCOMPARE(doug->levenshtein("$100", "100", false, true), 0);

  delete doug; // :(
}

QDateTime GenericTest::isoDate(QString str)
{
  return QDateTime::fromString(str, Qt::ISODate);
}

QTEST_MAIN(GenericTest)
#include "unit-tests.moc"
