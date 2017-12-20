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

    ui->showButton->hide();
    ui->tableBox->hide();
    ui->convertButton->hide();
    ui->convertSqlButton->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (db.isOpen())
        db.close();
}

//конвертация в csv-файл
void MainWindow::on_convertButton_clicked()
{
    QString table = ui->tableBox->currentText();

    //создаем csv файл с выбранной таблицей
    QString fileName = QFileDialog::getSaveFileName(this," Save File as", "", tr("Databases files (*.csv)"), Q_NULLPTR, QFileDialog::DontConfirmOverwrite);
    QString shortName = fileName.mid(fileName.lastIndexOf("/") + 1);
    QFile fileCsv(shortName);
    fileCsv.open(QIODevice::ReadWrite);
    QTextStream csv(&fileCsv);

    //запишем поля заголовков в файл
    QSqlRecord fields = db.record(table);

    QStringList str;
    for (int i = 0; i < fields.count(); i++)
    {
        //! необходимо реализовать обработку специальных символов
        str << processingForCsvStr(fields.fieldName(i));
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
            str << processingForCsvStr(q.value(i).toString());
        }
        csv << str.join(';') << endl;
    }

    fileCsv.close();
    ui->statusBar->showMessage("Файл конвертирован", 5000);
}

void MainWindow::on_convertSqlButton_clicked()
{
    ConvertClass convSql;
    convSql.convertToSql(name);
    ui->statusBar->showMessage("Файл конвертирован", 5000);
}

//тренируемся запоминать данные
void MainWindow::on_actionOpenDb_triggered()
{
    isDatabase = true;
    if (db.isOpen())
        db.close();

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

    ui->showButton->show();
    ui->tableBox->show();
    ui->convertButton->show();
    ui->convertSqlButton->hide();
}


void MainWindow::on_showButton_clicked()
{

    QString table = ui->tableBox->currentText();
    TableViewer tv;

    if (isDatabase)
    {
        tv.setData(db, table);
        ui->sqlView->setModel(tv.returnModel());
    }
    //если работаем с файлом
    else
    {
        tv.setData(name);
        ui->sqlView->setModel(tv.returnModel());
    }
    ui->statusBar->showMessage("Талица отображена!", 5000);
}

void MainWindow::on_actionOpencsv_triggered()
{
    isDatabase = false;
    if (db.isOpen())
        db.close();

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open database"), "E:\qt_projects\convert", tr("Databases files (*.csv)"));
    name = fileName.mid(fileName.lastIndexOf("/") + 1);

    QString catName = name;
    catName.replace(".csv", "");

    ui->tableBox->clear();
    ui->tableBox->addItem(catName);

    ui->showButton->show();
    ui->tableBox->show();
    ui->convertButton->hide();
    ui->convertSqlButton->show();
}

