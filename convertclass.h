/*!
 *\file
 *\brief Header file for ConvertToSqlClass
 */
#ifndef CONVERTCLASS_H
#define CONVERTCLASS_H

#include <processing.h>
#include <tablemodel.h>
#include "QFileDialog"
#include <QSqlQuery>
#include <QDebug>
#include <QtSql>

/*!
 * \brief The ConvertToSqlClass class.
 *
 * This class provides file handling and conversion to Sqlite
 */
class ConvertToSqlClass
{
public:
    /*!
     * \brief ConvertToSqlClass constructor.
     */
    ConvertToSqlClass();

    void setTableModel(TableModel* modelT);
    bool convertToSql();
private:
    QSqlDatabase dbMy;
    QString fName,exCr,exIn;
    QVector<QString> types;
    TableModel *model = Q_NULLPTR;
    void generateQuery ();
    bool determineSchema ();
};

#endif // CONVERTCLASS_H
