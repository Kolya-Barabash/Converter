#include <QtTest>

// add necessary includes here
#include "../processing.h"

class Tester : public QObject
{
    Q_OBJECT

public:
    Tester();

private slots:
    void testWhatType();
    void testParseStr();
    void testWithoutQuotes();
    void testCSVtoSQL();

};

Tester::Tester()
{

}

void Tester::testWhatType()
{
    QString row1("17.154");
    QString rowRes1("REAL");

    QString row2("1843");
    QString rowRes2("INTEGER");

    QString row3("17 wersdf 17");
    QString rowRes3("TEXT");

    QCOMPARE(whatType(row1), rowRes1);
    QCOMPARE(whatType(row2), rowRes2);
    QCOMPARE(whatType(row3), rowRes3);
}

void Tester::testParseStr()
{
    QString row("124;\"/n/n/n there is nothing \";;\"kang;oo\"");
    QStringList rowRes;
    rowRes << QString("124") << QString("/n/n/n there is nothing ") << QString("") << QString("kang;oo");

    QCOMPARE(parseStr(row), rowRes);
}

void Tester::testWithoutQuotes()
{
    QString row("\" \"\"\" boroda \"");
    QString rowRes(" \"\" boroda ");

    QCOMPARE(withoutQuotes(row), rowRes);
}

void Tester::testCSVtoSQL()
{
    QString row("Hi, Robert! How are you? \n -I'm fine, \"my rabbit\"!");
    QString rowRes("\"Hi, Robert! How are you? \n -I'm fine, \"\"my rabbit\"\"!\"");

    QCOMPARE(processingForCsvStr(row), rowRes);
}

QTEST_APPLESS_MAIN(Tester)

#include "tst_tester.moc"

/*
void Test_CsvStream::deleteExtraQuotes()
{
  CsvStream csv("abc.csv");

  QString word1("\"one, two, three,\"\"\"\",\"\"\"\"\"");
  QString word1_r("one, two, three,\"\",\"\"");

  QString word2("\"\"");
  QString word2_r("");

  QCOMPARE(csv.deleteExtraQuotes(word1), word1_r);
  QCOMPARE(csv.deleteExtraQuotes(word2), word2_r);
  QCOMPARE(csv.deleteExtraQuotes(word2_r), word2_r);
}
*/
