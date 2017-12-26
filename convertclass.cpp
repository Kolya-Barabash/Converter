#include "convertclass.h"

ConvertToSqlClass::ConvertToSqlClass()
{
}

void ConvertToSqlClass::setTableModel(TableModel* modelT)
{
    model = modelT;
}

bool ConvertToSqlClass::convertToSql()
{
    this->types = model->getTypes();
    this->generateQuery();
    if (this->determineSchema())
    {
        QSqlQuery q(dbMy);

        if (!q.prepare(exIn))
          qDebug() << q.lastError();

        QVector<QVector<QVariant>> data = model->getData();

        dbMy.transaction();
        for (int i = 0 ; i < data.count(); i++)
        {
            for (int j = 0 ; j < data[i].count(); j++)
            {
                q.addBindValue(data[i][j]);
            }
            q.exec();
        }
        dbMy.commit();
        q.finish();
        q.clear();
        dbMy.close();
        dbMy.removeDatabase("tmp");
        return true;
    }

    return false;
}

void ConvertToSqlClass::generateQuery ()
{
  int i = 0;

  exCr = "create table " + model->getTableName() + "(";
  exIn = "insert into " + model->getTableName() + "(";
  QString exV(") values(");

  for (QString item : model->getHeader())
  {
    exCr += item + " ";
    exIn += item + ", ";
    exV += "?, ";
    exCr += types[i++] + ", ";
  }
  exIn.remove(exIn.size() - 2, 2);
  exCr.remove(exCr.size() - 2, 2);
  exV.remove(exV.size() - 2, 2);
  exIn += exV + ")";
  exCr += ")";
}

bool ConvertToSqlClass::determineSchema ()
{

    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, " Save File as", "", "Databases files (*.sqlite)");
    if (fileName != "")
    {
        dbMy = QSqlDatabase::addDatabase("QSQLITE","tmp");
        dbMy.setDatabaseName(fileName);

        //открываем базу данных
        if (!dbMy.open())
            qDebug() << dbMy.lastError();

        //получаем список таблиц
        QStringList tables = dbMy.tables();

        QSqlQuery q(dbMy);
        //проверяем, есть ли таблицы в базе

        int decision = 0;
        if (!tables.empty())
        {
            if (tables.contains(model->getTableName()))
            {
                //qDebug() << "Already have tables!";
                QSqlRecord schema = dbMy.record(model->getTableName());
                if (schema.count() == model->getHeader().count() )
                {
                    int j = 0;
                    for (QString item : model->getHeader())
                    {
                        if (item == schema.fieldName(j))
                          j++;
                        else
                          break;
                    }

                    if ( j != schema.count() )
                    {
                        decision = 1;
                    }
                }
                else
                {
                    decision = 1;
                }
            }
            else
            {
                decision = 2;
            }
        }
        else
        {
           decision = 2;
        }

        dbMy.transaction();
        switch (decision)
        {
        case 0:
            q.exec(QString("DELETE FROM " + model->getTableName() + ";"));
            qDebug() << "Odinakovie";
            break;
        case 1:
            q.exec(QString("DROP TABLE " + model->getTableName() + ";"));
            qDebug() << "Raznie schemi";
        case 2:
            if (!q.exec(exCr))
                qDebug() << q.lastError();
            qDebug() << "Net tablici";
            break;
        default:
            break;
        }
        dbMy.commit();
        q.finish();
        q.clear();
        return true;
    }
    return false;
}
