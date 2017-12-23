#ifndef CONTRACTOR_H
#define CONTRACTOR_H

#include "tablemodel.h"
#include "processing.h"
#include "QTextStream"
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QSqlRecord"
#include "QFileDialog"

class Contractor : public QObject
{
    Q_OBJECT

public:
    explicit Contractor(QObject* parent = 0);

    void openCSV();

    void openSQL();
    void closeSql();

    void convertToSQL();
    void convertToCSV();

    TableModel* getModel();
public slots:
    void showTableSQL(const QString& tableName);

signals:
    void sendListOfTables(const QStringList&);
    void setTableToView(TableModel*);
private:
    TableModel *model;
    QString dbFileName;
    QSqlDatabase db;
};

#endif // CONTRACTOR_H
