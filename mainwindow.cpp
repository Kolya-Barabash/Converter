#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QTreeView>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//функция обработки строк
QString specialProc(QString str)
{
    if (str.contains(";") || str.contains(",") || str.contains("\"") || str.contains("\n"))
        return "\"" + str + "\"";

    if (str.contains("\""))
    {
        str.replace("\"","\"\"");
    }
    return str;
}


//конвертация в csv-файл
void MainWindow::on_convertButton_clicked()
{
    QString table = ui->tableBox->currentText();

    //создаем csv файл с выбранной таблицей
    QFile fileCsv(table + ".csv");
    fileCsv.open(QIODevice::ReadWrite);
    QTextStream csv(&fileCsv);

    //запишем поля заголовков в файл
    QSqlRecord fields = db.record(table);

    QStringList str;
    for (int i = 0; i < fields.count(); i++)
    {
        //! необходимо реализовать обработку специальных символов
        str << specialProc(fields.fieldName(i));
    }
    csv << str.join(";") << endl;

    //запишем строчки таблицы в файл
    QSqlQuery q;
    q.exec("SELECT * from " + table);

    //Обрабатываем каждую строку результата запроса
    while(q.next())
    {
        str.clear();
        for (int i = 0; i < fields.count(); i++)
        {
            str << specialProc(q.value(i).toString());
        }
        csv << str.join(';') << endl;
    }

    fileCsv.close();
}

void MainWindow::on_convertSqlButton_clicked()
{
    QFile file(name);
    if ( !file.open(QFile::ReadOnly | QFile::Text) )
    {
        qDebug() << "File not exists";
    }
    else
    {
        // Создаём поток для извлечения данных из файла
        QTextStream in(&file);
        QString line = in.readLine();
        QString line2 = in.readLine();
        QStringList parse = parseStr(line);
        QStringList parse2 = parseStr(line2);

        QString *types = new QString[parse.size()];

        int i = 0;
        for (QString item : parse2)
        {
            types[i++] = whatType(item);
        }

        i = 0;

        QString ex_cr("create table T1(");
        QString ex_in("insert into T1(");
        QString ex_v(") values(");
        QString *title = new QString[parse.size()] ;

        for (QString item : parse)
        {
            title[i] = item;
            ex_cr += item+" ";
            ex_in += item+", ";
            ex_v += "?, ";
            ex_cr += types[i++]+", ";
        }
        ex_in.remove(ex_in.size()-2,2);
        ex_cr.remove(ex_cr.size()-2,2);
        ex_v.remove(ex_v.size()-2,2);
        ex_in += ex_v + ")";
        ex_cr+=")";

        QString table = ui->tableBox->currentText();
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(table + ".sqlite");

        //открываем базу данных
        if (!db.open())
            qDebug() << db.lastError();

        //получаем список таблиц
        QStringList tables = db.tables();

        QSqlQuery q;
        //проверяем, есть ли таблицы в базе
        if (!tables.empty())
        {
            qDebug() << "Already have tables!";
            QSqlRecord schema = db.record(tables.at(0));
            if (schema.count() == parseStr(line).count() )
            {
                int j = 0;
                for (QString item : parseStr(line))
                {
                    if (item == schema.fieldName(j))
                        j++;
                    else
                        break;
                }

                if ( j == schema.count() )
                {
                    q.exec(QString("DELETE FROM "+tables.at(0)+";"));
                    qDebug() << "Truncate!";
                }
                else
                {
                    q.exec(QString("DROP TABLE "+tables.at(0)+";"));
                    if (!q.exec(ex_cr))
                        qDebug() << q.lastError();
                }
            }
            else
            {
                q.exec(QString("DROP TABLE "+tables.at(0)+";"));
                if (!q.exec(ex_cr))
                    qDebug() << q.lastError();
            }
        }
        else
            if (!q.exec(ex_cr))
                qDebug() << q.lastError();



        if (!q.prepare(ex_in))
               qDebug() << q.lastError();
        for (QString item : parse2)
        {
            q.addBindValue(item);
        }
        q.exec();

        // Считываем данные до конца файла
        while (!in.atEnd())
        {
            line = in.readLine();

            for (QString item : parseStr(line))
            {
                q.addBindValue(item);
            }
            q.exec();
        }
        file.close();
        db.close();
    }
    qDebug() << "Done";
}

//тренируемся запоминать данные
void MainWindow::on_actionOpenDb_triggered()
{
    isDatabase = true;

    QString fileName = QFileDialog::getSaveFileName(this,"Open File", "", tr("Databases files (*.sqlite)"), Q_NULLPTR, QFileDialog::DontConfirmOverwrite);
    name = fileName.mid(fileName.lastIndexOf("/") + 1);

    //qDebug() << name;

    //открытие базы данных
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    db.open();

    //список таблиц
    QStringList tables = db.tables();

    //заполнение комбобокса
    if (!tables.empty())
    {
        ui->tableBox->clear();
        ui->tableBox->addItems(tables);
    }

    //скопировали из нижней, т.к. нет базы
}


void MainWindow::on_showButton_clicked()
{
    QString table = ui->tableBox->currentText();

    if (isDatabase)
    {
        QSqlQuery q;
        q.exec("SELECT * FROM " + table);

        QSqlRecord fieldsRec = db.record(table);

        QStringList fieldsStr;
        for (int i = 0; i < fieldsRec.count(); i++)
        {
            fieldsStr << specialProc(fieldsRec.fieldName(i));
        }

        QStandardItemModel* model = new QStandardItemModel(this);
        model->setColumnCount(fieldsRec.count());
        model->setHorizontalHeaderLabels(fieldsStr);

        ui->sqlView->setModel(model);

    //Обрабатываем каждую строку результата запроса
        while(q.next())
        {
            QList<QStandardItem*> qStandItemList;
            for (int i = 0; i < fieldsRec.count(); i++)
            {
                qStandItemList.append(new QStandardItem(q.value(i).toString()));
            }
            model->insertRow(model->rowCount(),qStandItemList);
        }
    }
    else
    {
        //случай открытия CSV файла
        QFile file(name);
        if ( !file.open(QFile::ReadOnly | QFile::Text) )
        {
            qDebug() << "File not exists";
        }
        else
        {
            // Создаём поток для извлечения данных из файла
            QTextStream in(&file);
            QString line = in.readLine();

            QStringList title = parseStr(line);
            QStandardItemModel* model = new QStandardItemModel(this);
            model->setColumnCount(title.count());
            model->setHorizontalHeaderLabels(title);
            ui->sqlView->setModel(model);

            while (!in.atEnd())
            {
                line = in.readLine();
                QList<QStandardItem*> qStandItemList;

                for (QString item : parseStr(line))
                {
                    qStandItemList.append(new QStandardItem(item));
                }
                 model->insertRow(model->rowCount(),qStandItemList);
            }
            file.close();
        }
    }
}

void MainWindow::on_actionOpencsv_triggered()
{
    isDatabase = false;

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open database"), "E:\qt_projects\convert", tr("Databases files (*.csv)"));
    name = fileName.mid(fileName.lastIndexOf("/") + 1);

    QString catName = name;
    catName.replace(".csv", "");

    ui->tableBox->clear();
    ui->tableBox->addItem(catName);
}


QString whatType(QString str)
{
    QRegExp reg("^\\-?\\d+\\.\\d+$");

    if (str.contains(reg))
        return "REAL";

    reg = QRegExp("^\\-?\\d+$");
    if(str.contains(reg))
        return "INTEGER";
    return "TEXT";
}

QStringList parseStr(QString str)
{
        QStringList ret;
        int in;

        while ( (in = str.indexOf(";")) != -1)
        {
            QString sub_str = str;
            sub_str.remove(in,str.size());
            int count_Q = sub_str.count("\"");

            while (count_Q % 2 != 0)
            {
                int in2 = str.indexOf(";",in+1);
                if (in2 == -1)
                {
                    ret.append(str);
                    return ret;
                }
                sub_str = str;
                sub_str.remove(in2,str.size());
                count_Q = sub_str.count("\"");
                in = in2;
            }

            ret.append(withoutQ(sub_str));
            str.remove(0,sub_str.size()+1);
        }

        ret.append(withoutQ(str));
        return ret;
}

QString withoutQ(QString str)
{
    if (str[0] == "\"")
    {
        str.remove(0,1);
        str.remove(str.size()-1,1);
    }

    str.replace("\"\"","\"");

    return str;
}


