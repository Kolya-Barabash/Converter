/*!
 *\file
 *\brief Header file for ConvertToSqlClass
 */
#ifndef CONVERTCLASS_H
#define CONVERTCLASS_H

#include "processing.h"
#include "tablemodel.h"
#include <QFileDialog>
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
     * \brief ConvertToSqlClass - constructor.
     */
    ConvertToSqlClass();

    /*!
     * \brief setTableModel - method for including model
     * \param modelT - model with data
     */
    void setTableModel(TableModel* modelT);

    /*!
     * \brief convertToSql - converts data from the model into a sqlite table
     * \return returns true, if the conversion was successful, otherwise false
     */
    bool convertToSql(QString& fileName);
private:
    QSqlDatabase dbMy;
    QString fName,exCr,exIn;
    QVector<QString> types;
    TableModel *model = Q_NULLPTR;
    void generateQuery ();
    bool determineSchema ();
};

#endif // CONVERTCLASS_H
