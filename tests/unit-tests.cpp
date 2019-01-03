/*
 * Main Unit Test File.
 * Needs work.
 */

#define UNIT_TEST
#define private public
#include "../src/meta/note.h"
#include "../src/sql/sqlmanager.h"
#include <helper-io.hpp>
#define private private



#include <QtTest/qtest.h>
#include <QCoreApplication>
#include <QDebug>

class GenericTest : public QObject
{
  Q_OBJECT

private slots:
  void numberToStringWord();
  void dateFormatting();
  void sqlmanager();

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
  Note *note = new Note(-1, 1, "Test Note", "",
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

void addNoteToSQLite()
{

}

void GenericTest::sqlmanager()
{
  SQLManager manager;

  //
  // Test: SQL Errors
  //
  qDebug("TESTING AN ERROR");
  QVERIFY( !manager.runScript(":test/heroes-error.sql") );
  qDebug("DONE TESTING AN ERROR");

  //
  // Test: Columns
  //
  QVERIFY( manager.runScript(":test/heroes.sql") );

  QVector<QVariant> col = manager.column("SELECT name FROM heroes");
  QStringList expected = {"Batman", "Spiderman"};

  for (int i=0; i<2; i++)
    QCOMPARE( col[i].toString(), expected[i] );

  //
  // Test: Rows
  //
  MapVector rows = manager.rows("SELECT * FROM vehicles", {"name", "color", "speed"});
  QCOMPARE(rows[0]["name"].toString(), "Ferrari");
  QCOMPARE(rows[1]["color"].toString(), "camo");
  QCOMPARE(rows[2]["speed"].toInt(), 30000);

  //
  // Test: Dropping tables
  //
  QVERIFY( manager.realBasicQuery("DROP TABLE heroes") );
  QVERIFY( manager.realBasicQuery("DROP TABLE vehicles") );

  //
  // Test: Dropping tables
  //
  QStringList tables = {"notes", "notebooks", "tags", "notes_tags"};
  for (QString t : tables)
    QVERIFY( manager.realBasicQuery( QString("drop table if exists %1").arg(t) ) );
  QVERIFY( manager.runScript(":sql/create.sql") );

  //
  // Test: Adding a note
  //
  Note note(-1, 1, "Test Note", "Hello world.",
            isoDate( "2000-01-01T09:38:59Z"), // Date Created
            QDateTime::currentDateTime(),  // Date Modified
            false, -1, {}, false);

  QVERIFY( manager.addNote(&note) );

  //
  // Test: Updating note to database
  //
  note.setTitle("My new title");
  manager.updateNoteToDB(&note);
  Map mynote = manager.row("select title from notes", {"title"});
  QCOMPARE(mynote["title"].toString(), note.title());

  //
  // Test: Updating note from database
  //
  manager.realBasicQuery( QString("update notes set title = 'from database' where id = %1").arg(note.id()) );
  manager.updateNoteFromDB(&note);
  mynote = manager.row("select title from notes", {"title"});
  QCOMPARE(note.title(), mynote["title"].toString());

  //
  // Test: Loading a notebook
  //
  QVERIFY (manager.realBasicQuery("insert into notebooks (sync_id, id, title, parent) values "
                                  "(1, 2, 'Recipes', -1)"));
  QVector<Notebook*> notebooks = manager.notebooks();
  for (Notebook *n : notebooks) {
    QCOMPARE( n->syncId(), 1 );
    QCOMPARE( n->id(), 2 );
    QCOMPARE( n->title(), "Recipes" );
    QCOMPARE( n->parent(), nullptr );
  }
  // Free memory
  for (int i=notebooks.length()-1; i>=0; i--)
    delete notebooks[i];

  //
  // Test: Loading nested notebooks (hierarchial)
  //
  QVERIFY (manager.realBasicQuery("insert into notebooks (sync_id, id, title, parent) values "
                                  "(2, 3, 'Cool', 1)"));
  notebooks = manager.notebooks();
  for (Notebook *n : notebooks) {
    QCOMPARE( n->syncId(), 1 );
    QCOMPARE( n->id(), 2 );
    QCOMPARE( n->title(), "Recipes" );
    QCOMPARE( n->parent(), nullptr );
    for (Notebook *n2 : n->children()) {
      QCOMPARE( n2->syncId(), 2 );
      QCOMPARE( n2->id(), 3 );
      QCOMPARE( n2->title(), "Cool" );
      QCOMPARE( n2->parent(), n );
    }
  }
  // Free memory
  for (int i=notebooks.length()-1; i>=0; i--)
    delete notebooks[i];

  //
  // Test: Read tags
  //
  QStringList tags = {"fun", "2018", "2019", "pictures"};
  QVERIFY( manager.realBasicQuery("insert into tags (title) values ('fun'), ('2018'), ('2019'), ('pictures')"));
  VariantList tagNames = manager.column("select * from tags", 2);
  for (int i=0; i<tags.length(); i++)
    QCOMPARE( tagNames[i].toString(), tags[i] );

  //
  // Test: Add new Tag
  //
  Tag *newTag = new Tag(1, 5, "cats&dogs");
  manager.addTag(newTag);
  tagNames = manager.column("select * from tags", 2);
  QStringList tlist;
  tags.append("cats&dogs");
  for (QVariant t : tagNames)
    tlist.append(t.toString());
  QCOMPARE(tlist, tags);
  delete newTag;
}


QDateTime GenericTest::isoDate(QString str)
{
  return QDateTime::fromString(str, Qt::ISODate);
}

QTEST_MAIN(GenericTest)
#include "unit-tests.moc"
