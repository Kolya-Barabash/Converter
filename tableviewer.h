/*!
  \file
  \brief Header file for TableViewer class.

  This class is responsible for displaying data in a table
 */
#ifndef TABLEVIEWER_H
#define TABLEVIEWER_H

#include "QStandardItemModel"
#include "QSqlDatabase"
#include "QFile"
#include <QtSql>
#include <QSqlQuery>
#include "processing.h"


/*!
 * \brief The TableViewer class
 *
 *  Wrapper over  QStandardItemModel.Displays table data.
 */
class TableViewer
{
public:
    /*!
     * \brief TableViewer - constructor.
     */
    TableViewer();

    /*!
     * \brief setData - fills the model with data
     * \param db - database
     * \param table - table name
     */
    void setData(QSqlDatabase& db, QString table);

    /*!
     * \brief setData - fills the model with data. Another way.
     * \param name - table name
     */
    void setData(QString name);

    /*!
     * \brief returnModel
     * \return return a model.
     */
    QStandardItemModel* returnModel();

private:
    QStandardItemModel *model = nullptr;
};

#endif // TABLEVIEWER_H
