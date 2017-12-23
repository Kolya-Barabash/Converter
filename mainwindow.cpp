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

    ui->tableBox->hide();
    ui->convertButton->hide();
    ui->convertSqlButton->hide();

    //connections
    connect(&contractor, SIGNAL(sendListOfTables(const QStringList&)), this, SLOT(slotFillBox(const QStringList&)));
    connect(this, SIGNAL(sendCurrentTable(const QString&)), &contractor, SLOT(showTableSQL(const QString&)));
    connect(ui->tableBox, SIGNAL(currentIndexChanged(const QString&)), &contractor, SLOT(showTableSQL(const QString&)));
    connect(&contractor, SIGNAL(setTableToView(TableModel*)), this, SLOT(setModel(TableModel*)));
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

    contractor.convertToCSV();

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

    contractor.openSQL();

    emit sendCurrentTable(ui->tableBox->currentText());

    ui->tableBox->show();
    ui->convertButton->show();
    ui->convertSqlButton->hide();
}

void MainWindow::on_actionOpencsv_triggered()
{
    isDatabase = false;
    if (db.isOpen())
        db.close();

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open database"), "", tr("Databases files (*.csv)"));
    name = fileName.mid(fileName.lastIndexOf("/") + 1);

    QString catName = name;
    catName.replace(".csv", "");

    ui->tableBox->clear();
    ui->tableBox->addItem(catName);

    ui->tableBox->show();
    ui->convertButton->hide();
    ui->convertSqlButton->show();
}

void MainWindow::slotFillBox(const QStringList& tables)
{
    ui->tableBox->setEnabled(false);
    ui->tableBox->clear();

    ui->tableBox->addItems(tables);
    ui->tableBox->setEnabled(true);
}

void MainWindow::setModel(TableModel *model)
{
    ui->sqlView->setModel(model);
}
