/*!
 *\file
 *\brief Header file for ConvertClass
 */
#ifndef CONVERTCLASS_H
#define CONVERTCLASS_H

#include "QtSql"
#include "QFileDialog"
#include <processing.h>
#include <QMainWindow>
#include <QtSql>
#include <QSqlQuery>
#include "QFileDialog"
#include <QStandardItemModel>
#include <QTreeView>
#include <tablemodel.h>

/*!
 * \brief The ConvertClass class.
 *
 * This class provides file handling and conversion to Sqlite
 */
class ConvertClass
{
public:
    /*!
     * \brief ConvertClass constructor.
     */
    ConvertClass();

    void convertToSql(TableModel* model);
private:
    QSqlDatabase dbMy;
    QString fName,exCr,exIn;
    QVector<QString> types;
    void generateQuery (QStringList header);
    void determineSchema (QStringList header);
};

#endif // CONVERTCLASS_H
