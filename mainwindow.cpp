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

    //кнопки
    connect(ui->actionOpencsv, SIGNAL(triggered(bool)), this, SLOT(openCSV()));
    connect(ui->actionOpenDb, SIGNAL(triggered(bool)), this, SLOT(openDB()));
    connect(ui->convertButton, SIGNAL(clicked(bool)), this, SLOT(convertIntoCSV()));
    connect(ui->convertSqlButton, SIGNAL(clicked(bool)), this, SLOT(convertIntoSql()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::convertIntoCSV()
{
    if (contractor.convertToCSV())
        ui->statusBar->showMessage("Файл конвертирован!", 5000);
    else
        ui->statusBar->showMessage("Не выбран файл для сохранения!", 5000);
}

void MainWindow::convertIntoSql()
{
    if (contractor.convertToSQL())
        ui->statusBar->showMessage("Файл конвертирован!", 5000);
    else
        ui->statusBar->showMessage("Не выбран файл для сохранения!", 5000);
}

void MainWindow::openDB()
{
    if (contractor.openSQL())
    {
        emit sendCurrentTable(ui->tableBox->currentText());

        ui->tableBox->show();
        ui->convertButton->show();
        ui->convertSqlButton->hide();
    }
    else
       ui->statusBar->showMessage("Файл не выбран!", 5000);
}


void MainWindow::openCSV()
{
    if (contractor.openCSV())
    {

        ui->tableBox->hide();
        ui->convertButton->hide();
        ui->convertSqlButton->show();

        ui->sqlView->setModel(contractor.getModel());

        ui->statusBar->showMessage("Талица отображена!", 5000);
    }
    else
        ui->statusBar->showMessage("Файл не выбран!", 5000);
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

