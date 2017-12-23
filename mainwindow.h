/*!
  \file
  \brief Header file for MainWindow. Provides the work of the graphical shell. Responsible for the operability of buttons and windows.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <convertclass.h>
#include <QMainWindow>
#include <QtSql>
#include <Contractor.h>

namespace Ui {
>>>>>>> Zheka's-branch
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

public slots:
    void slotFillBox(const QStringList& tables);
    void setModel(TableModel*);
signals:
    void sendCurrentTable(const QString& tableName);

private:
    Ui::MainWindow *ui;

    Contractor contractor;
    QSqlDatabase db;    //база данных
    QString name;
    bool isDatabase = false;
};



#endif // MAINWINDOW_H
