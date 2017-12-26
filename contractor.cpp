#include "contractor.h"

Contractor::Contractor(QObject* parent): QObject(parent)
{
}

bool Contractor::openCSV(QString& fileName)
{
    //QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR,"Open database", "","Databases files (*.csv)",  Q_NULLPTR, QFileDialog::DontConfirmOverwrite);
    if (fileName != "")
    {
        QString name = fileName.mid(fileName.lastIndexOf("/") + 1);
        name.replace(".csv", "");

        //случай открытия CSV файла
        QFile file(fileName);
        if ( !file.open(QFile::ReadOnly | QFile::Text) )
        {
            qDebug() << "File not exists";
        }
        else
        {
            tmpModel = std::unique_ptr<TableModel>(new TableModel());
            model = tmpModel.get();
            //model = new TableModel();
            if (whatType(name) == "INTEGER")
                name = "table"+name;
            model->setTableName(name);

            // Создаём поток для извлечения данных из файла
            QTextStream in(&file);
            QString line = in.readLine();

            QStringList title = parseStr(line);

            model->setHeader(title);

            QVector<QString> types(title.count());
            QVector<QVector<QVariant>> data;

            if (!in.atEnd())
            {
                line = in.readLine();
                QStringList parse = parseStr(line);
                QVector<QVariant> temp(parse.count());

                int i = 0;
                for (QString item : parse)
                {
                    types[i] = whatType(item);
                    temp[i++] = item;
                }

                data.push_back(temp);
                QString tmp;

                while (!in.atEnd())
                {
                    line = in.readLine();
                    parse = parseStr(line);
                    i = 0;

                    for (QString item : parse)
                    {
                        tmp = whatType(item);
                        if (types[i] != tmp)
                        {
                            if ((tmp == "TEXT") || (tmp == "REAL" && types[i] == "INTEGER"))
                                types[i] = tmp;
                        }
                        temp[i] = item;
                        i++;
                    }
                    data.push_back(temp);
                }
            }
            file.close();

            model->setData(data);
            model->setTypes(types);
        }
        return true;
    }

    return false;
}

bool Contractor::openSQL(QString& fileName)
{
    if (db.isOpen())
        db.close();

    //QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR,"Open File", "", "Databases files (*.sqlite)", Q_NULLPTR, QFileDialog::DontConfirmOverwrite);
    if (fileName != "")
    {

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(fileName);
        db.open();

        emit sendListOfTables(db.tables());

        return true;
    }

    return false;
}

void Contractor::closeSQL()
{
    db.close();
}

bool Contractor::convertToCSV(QString& fileName)
{
    //QString table = ui->tableBox->currentText();

    //создаем csv файл с выбранной таблицей
    //QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR," Save File as", "", "Databases files (*.csv)", Q_NULLPTR, QFileDialog::DontConfirmOverwrite);
    if (fileName != "")
    {
        QFile fileCsv(fileName);
        fileCsv.open(QIODevice::ReadWrite);
        QTextStream csv(&fileCsv);

        //запишем поля заголовков в файл
        QStringList headers = model->getHeader();
        QVector<QVector<QVariant>> tableData(model->getData());

        for (QString str : headers)
        {
            str = processingForCsvStr(str);
        }
        csv << headers.join(";") << endl;

        QStringList str;
        for (int i = 0; i < tableData.size(); i++)
        {
            str.clear();
            for (int j = 0; j < tableData[i].size(); j++)
            {
                str << processingForCsvStr(tableData[i][j].toString());
            }
            csv << str.join(';') << endl;
        }

        fileCsv.close();
        return true;
    }

    return false;
}

bool Contractor::convertToSQL(QString& fileName)
{
    ConvertToSqlClass convSql;
    convSql.setTableModel(model);
    if (convSql.convertToSql(fileName))
        return true;
    return false;
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

    tmpModel = std::unique_ptr<TableModel>(new TableModel());
    model = tmpModel.get();
    //model = new TableModel();
    model->setHeader(fieldsStr);

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
