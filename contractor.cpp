#include "contractor.h"

Contractor::Contractor()
{

}

void Contractor::openCSV()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,"Open database", "","Databases files (*.csv)");
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
        model = new TableModel;
        model->setTableName(name);

        // Создаём поток для извлечения данных из файла
        QTextStream in(&file);
        QString line = in.readLine();

        QStringList title = parseStr(line);

        //model->setHeaderData(title);
        model ->setHeaderData(QStringList(0));

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
}

TableModel* Contractor::getModel()
{
    return model;
}
