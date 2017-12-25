#ifndef CONTRACTOR_H
#define CONTRACTOR_H

#include "convertclass.h"
#include "QTextStream"

#include "QDebug"

class Contractor : public QObject
{
    Q_OBJECT

public:
    explicit Contractor(QObject* parent = 0);

    bool openCSV();
    bool openSQL();
    void closeSql();

    bool convertToSQL();
    bool convertToCSV();

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
