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
    /*!
     * \brief ConvertClass destructor.
     */
    ~ConvertClass(){ delete []types; }

    /*!
     * \brief convertToSql - main function. Processes a file and converts it to Sqlite.
     * \param filename - file directory.
     */
    void convertToSql(QString filename);
private:
    QSqlDatabase dbMy;
    QString *types = nullptr;
    QString fname,ex_cr,ex_in;
    void determineSchema (QString str);
    void generateQuary (QString str);
    void determineType();
};

#endif // CONVERTCLASS_H
