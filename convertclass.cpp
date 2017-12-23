#include "convertclass.h"

ConvertClass::ConvertClass()
{
}

void ConvertClass::convertToSql(TableModel* model)
{
    this->types = model->getTypes();
    this->generateQuery(model->getHeader());
    this->determineSchema(model->getHeader());

    QSqlQuery q;

    if (!q.prepare(exIn))
      qDebug() << q.lastError();

    QVector<QVector<QVariant>> data = model->getData();

    for (int i = 0 ; i < data.count(); i++)
    {
        for (int j = 0 ; j < data[i].count(); j++)
        {
            q.addBindValue(data[i][j]);
        }
        q.exec();
    }

    dbMy.close();

  /*
  QFile file(fName);
  if ( !file.open(QFile::ReadOnly | QFile::Text) )
    qDebug() << "File not exists";
  else
  {
    QTextStream in(&file);
    QString line = in.readLine();
    this ->generateQuery(line);
    this->determineSchema(line);
    QSqlQuery q;

    if (!q.prepare(exIn))
      qDebug() << q.lastError();
    // Считываем данные до конца файла
    while (!in.atEnd())
    {
      line = in.readLine();

      for (QString item : parseStr(line))
        q.addBindValue(item);
      q.exec();
    }
    file.close();
    dbMy.close();
  }
  */

}

void ConvertClass::generateQuery (QStringList header)
{
  int i = 0;

  exCr = "create table T1(";
  exIn = "insert into T1(";
  QString exV(") values(");

  for (QString item : header)
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



/*
void ConvertClass::determineType()
{
  QFile file(fName);
  if ( !file.open(QFile::ReadOnly | QFile::Text) )
    qDebug() << "File not exists";
  else
  {
    QTextStream in(&file);
    QString line = in.readLine();
    if (!in.atEnd())
    {
      line = in.readLine();
      QString temp;
      QStringList parse = parseStr(line);
      types = new QString[parse.count()];
      int i = 0;
      for (QString item : parse)
        types[i++] = whatType(item);
      i = 0;
      while (!in.atEnd())
      {
        i = 0;
        line = in.readLine();
        QStringList parse = parseStr(line);
        for (QString item : parse)
        {
          temp = whatType(item);
          if (types[i] != temp)
          {
            if ((temp == "TEXT") || (temp == "REAL" && types[i] == "INTEGER"))
              types[i] = temp;
          }
          i++;
        }
      }
    }
    file.close();
  }
}
*/



void ConvertClass::determineSchema (QStringList header)
{
  dbMy = QSqlDatabase::addDatabase("QSQLITE");
  QString fileName = QFileDialog::getSaveFileName(nullptr, " Save File as", "", "Databases files (*.sqlite)");
  dbMy.setDatabaseName(fileName);

  //открываем базу данных
  if (!dbMy.open())
    qDebug() << dbMy.lastError();

  //получаем список таблиц
  QStringList tables = dbMy.tables();

  QSqlQuery q;
  //проверяем, есть ли таблицы в базе
  if (!tables.empty())
  {
    //qDebug() << "Already have tables!";
    QSqlRecord schema = dbMy.record(tables.at(0));
    if (schema.count() == header.count() )
    {
      int j = 0;
      for (QString item : header)
      {
        if (item == schema.fieldName(j))
          j++;
        else
          break;
      }

      if ( j == schema.count() )
      {
        q.exec(QString("DELETE FROM " + tables.at(0) + ";"));
        //qDebug() << "Truncate!";
      }
      else
      {
        q.exec(QString("DROP TABLE " + tables.at(0) + ";"));
        if (!q.exec(exCr))
          qDebug() << q.lastError();
      }
    }
    else
    {
      q.exec(QString("DROP TABLE " + tables.at(0) + ";"));
      if (!q.exec(exCr))
        qDebug() << q.lastError();
    }
  }
  else if (!q.exec(exCr))
    qDebug() << q.lastError();
}
