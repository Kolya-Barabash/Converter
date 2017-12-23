#include "contractor.h"
#include "QFileDialog"

Contractor::Contractor(QObject* parent): QObject(parent)
{
}

void Contractor::openSQL()
{
    if (db.isOpen())
        db.close();

    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR,"Open File", "", "Databases files (*.sqlite)", Q_NULLPTR, QFileDialog::DontConfirmOverwrite);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    db.open();

    emit sendListOfTables(db.tables());
}

void Contractor::closeSql()
{
    db.close();
}

void Contractor::convertToCSV()
{
    //QString table = ui->tableBox->currentText();

    //создаем csv файл с выбранной таблицей
    QString fileName = QFileDialog::getSaveFileName(nullptr," Save File as", "", "Databases files (*.csv)", Q_NULLPTR, QFileDialog::DontConfirmOverwrite);
    //QString shortName = fileName.mid(fileName.lastIndexOf("/") + 1);

    QFile fileCsv(fileName);
    fileCsv.open(QIODevice::ReadWrite);
    QTextStream csv(&fileCsv);

    //запишем поля заголовков в файл
    QStringList headers = model->getHeaderData();
    QVector<QVector<QVariant>> tableData(model->getData());

    for (QString str : headers)
    {
        str = processingForCsvStr(str);
    }
    csv << headers.join(";") << endl;

    QStringList str;
    for (int i = 0; i < tableData.size();i++)
    {
        str.clear();
        for (int j = 0; j < tableData[i].size(); j++)
        {
            str << processingForCsvStr(tableData[i][j].toString());
        }
        csv << str.join(';') << endl;
    }

    fileCsv.close();
}

TableModel* Contractor::getModel()
{
    return model;
}

void Contractor::showTableSQL(const QString &tableName)
{
    if (tableName == "")
        return;

    QSqlQuery q(db);
    q.exec("SELECT * FROM " + tableName);

    QSqlRecord fieldsRec = db.record(tableName);

    QStringList fieldsStr;
    for (int i = 0; i < fieldsRec.count(); i++)
    {
        fieldsStr << fieldsRec.fieldName(i);
    }

    model = new TableModel();
    model->setHeaderData(fieldsStr);

    QVector<QVector<QVariant>> tableData;
    //Обрабатываем каждую строку результата запроса
    while(q.next())
    {
        QVector<QVariant> stringVector;
        for (int i = 0; i < fieldsRec.count(); i++)
        {
            stringVector.push_back(q.value(i).toString());
        }
        tableData.push_back(stringVector);
    }
    model->setData(tableData);

    emit setTableToView(model);
}
