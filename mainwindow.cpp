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


void MainWindow::on_convertSqlButton_clicked()
{
    ConvertClass convSql;
    convSql.convertToSql(contractor.getModel());
    ui->statusBar->showMessage("Файл конвертирован", 5000);
}

void MainWindow::on_actionOpencsv_triggered()
{
    contractor.openCSV();

    ui->tableBox->hide();
    ui->convertButton->hide();
    ui->convertSqlButton->show();

    ui->sqlView->setModel(contractor.getModel());

    ui->statusBar->showMessage("Талица отображена!", 5000);
}

//конвертация в csv-файл
void MainWindow::on_convertButton_clicked()
{
    QString table = ui->tableBox->currentText();

    contractor.convertToCSV();

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

