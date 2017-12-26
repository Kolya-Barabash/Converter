#include <QtTest>
#include <QFile>
#include <QDir>

// add necessary includes here
#include "../processing.h"
#include "../contractor.h"
#include "../convertclass.h"
#include "../tablemodel.h"

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
    void testAll();

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

void Tester::testAll()
{
    QDir dir;
    QString fileName = dir.absoluteFilePath("input.csv");
    QString dbName = dir.absoluteFilePath("input.sqlite");
    QString fileNameOut = dir.absoluteFilePath("input1.csv");

    Contractor contractor;
    contractor.openCSV(fileName);

    TableModel *tm = contractor.getModel();
    ConvertToSqlClass converter;
    converter.setTableModel(tm);
    converter.convertToSql(dbName);

    contractor.openSQL(dbName);
    contractor.showTableSQL("input");
    contractor.convertToCSV(fileNameOut);

    QFile file1(fileName);
    file1.open(QIODevice::ReadOnly);
    QTextStream in1(&file1);
    QString str1;
    in1 >> str1;
    file1.close();

    QFile file2(fileNameOut);
    file2.open(QIODevice::ReadOnly);
    QTextStream in2(&file2);
    QString str2;
    in2 >> str2;

    QCOMPARE(str1, str2);

}



QTEST_APPLESS_MAIN(Tester)

#include "tst_tester.moc"
