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

void MainWindow::on_convertSqlButton_clicked()
{
    ConvertClass convSql;
    convSql.convertToSql(contractor.getModel());
    ui->statusBar->showMessage("Файл конвертирован", 5000);
}

void MainWindow::on_actionOpencsv_triggered()
{
    contractor.openCSV();

    ui->showButton->hide();
    ui->tableBox->hide();
    ui->convertButton->hide();
    ui->convertSqlButton->show();

    ui->sqlView->setModel(contractor.getModel());

    ui->statusBar->showMessage("Талица отображена!", 5000);
}

