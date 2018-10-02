/*
 * Main Unit Test File.
 * Needs work.
 */

#include <QtTest>
#include <QCoreApplication>

class GenericTest : public QObject
{
    Q_OBJECT

private slots:
    void toUpper();

};

void GenericTest::toUpper()
{
    QString str = "hello";
    //QVERIFY(str.toUpper() == "HEfLLO");
    QCOMPARE(str.toUpper(), "ehe");
}

QTEST_MAIN(GenericTest)
#include "main.moc"
