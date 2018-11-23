/*
 * Main Unit Test File.
 * Needs work.
 */

#define private public
#include "../src/meta/note.h"
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
    Note note;
    QStringList words = {"one", "two", "three", "four", "five", "six",
                         "seven", "eight", "nine", "ten"};
    for (int i = 1; i <= 10; i++)
        QCOMPARE(note.numberToString(i), words.at(i-1));

    QCOMPARE(note.numberToString(11), "11");
}

void GenericTest::dateFormatting()
{
    Note note;
    // For unit tests, the current date will always be:
    //                              2000-12-25T09:38:59Z
    note.setDate_created( isoDate( "2000-01-01T09:38:59Z") );
    QCOMPARE(note.date_created_str(), "January 1, 2000");
    QCOMPARE(note.date_created_str_informative(), "January 1, 2000 at 9:38AM UTC");

    note.setDate_modified( isoDate("2000-12-25T09:38:29Z") ); // 30 seconds ago
    QCOMPARE(note.date_modified_str(), "Just now");

    note.setDate_modified( isoDate("2000-12-25T09:36:59Z") ); // two minutes ago
    QCOMPARE(note.date_modified_str(), "Two minutes ago");

    note.setDate_modified( isoDate("2000-12-25T09:26:59Z") ); // twelve minutes ago
    QCOMPARE(note.date_modified_str(), "12 minutes ago");

    note.setDate_modified( isoDate("2000-11-01T09:38:59Z") ); // one month ago
    QCOMPARE(note.date_modified_str(), "One month ago");

    note.setDate_modified( isoDate("1999-12-25T09:38:59Z") ); // one year ago
    QCOMPARE(note.date_modified_str(), "One year ago");

    note.setDate_modified( isoDate("1989-12-25T09:38:59Z") ); // eleven years ago
    QCOMPARE(note.date_modified_str(), "11 years ago");

    // Invalid date scenerio where modified date is later than current
    note.setDate_modified( isoDate("2001-01-01T09:38:59Z") );
    QCOMPARE(note.date_modified_str(), "Just now"); // "Just now" is possibly the safest response we could expect.

}

QDateTime GenericTest::isoDate(QString str)
{
    return QDateTime::fromString(str, Qt::ISODate);
}

QTEST_MAIN(GenericTest)
#include "unit-tests.moc"
