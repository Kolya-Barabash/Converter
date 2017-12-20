/*!
  \file
  \brief Header file for MainWindow. Provides the work of the graphical shell. Responsible for the operability of buttons and windows.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <convertclass.h>
#include <tableviewer.h>
#include <QMainWindow>
#include <QtSql>

namespace Ui {
class MainWindow;
}

/*!
 * \brief The MainWindow class.
 *
 * The main window of application with table view and special buttons.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /*!
     * \brief MainWindow - constructor
     * \param parent
     */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_convertButton_clicked();
    void on_convertSqlButton_clicked();

    void on_actionOpenDb_triggered();
    void on_actionOpencsv_triggered();

    void on_showButton_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;    //база данных
    QString name;
    bool isDatabase = false;
};



#endif // MAINWINDOW_H
