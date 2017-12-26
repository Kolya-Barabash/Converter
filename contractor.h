/*!
 *\file
 *\brief Header file for Contractor
 */
#ifndef CONTRACTOR_H
#define CONTRACTOR_H

#include "convertclass.h"
#include "QTextStream"
#include <memory>
#include "QDebug"


/*!
 * \brief The Contractor class.
 * This class is the engine of the program. Provides opening files and converting to the desired format.
 */
class Contractor : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Contractor - constructor.
     * \param parent - parent.
     */
    explicit Contractor(QObject* parent = 0);

    /*!
     * \brief openCSV - opens csv file and set model.
     * \param fileName - file name.
     * \return returns true, if the opening was successful, otherwise false .
     */
    bool openCSV(QString& fileName);

    /*!
     * \brief openSQL - opens database.
     * \param fileName - file name.
     * \return  returns true, if the opening was successful, otherwise false.
     */
    bool openSQL(QString& fileName);

    /*!
     * \brief closeSQL - close database
     */
    void closeSQL();

    /*!
     * \brief convertToSQL - converts data from the model into a sqlite table. Method call method from ConvertToSqlClass.
     * \param fileName - file name.
     * \return returns true, if the conversion was successful, otherwise false.
     */
    bool convertToSQL(QString& fileName);

    /*!
     * \brief convertToCSV - converts data from the model into a CSV file.
     * \param fileName - file name.
     * \return returns true, if the conversion was successful, otherwise false.
     */
    bool convertToCSV(QString& fileName);

    /*!
     * \brief getModel - return current model(table).
     * \return model.
     */
    TableModel* getModel();
public slots:
    /*!
     * \brief showTableSQL - set the table to model of TableModel of contractor.
     * \param tableName - table name.
     */
    void showTableSQL(const QString& tableName);

signals:
    void sendListOfTables(const QStringList&);
    void setTableToView(TableModel*);
private:
    TableModel *model = Q_NULLPTR;
    //QString dbFileName;
    QSqlDatabase db;
    std::unique_ptr<TableModel> tmpModel;
};

#endif // CONTRACTOR_H
