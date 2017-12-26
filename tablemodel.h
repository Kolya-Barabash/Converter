/*!
 *\file
 *\brief Header file for TableModel class
 */
#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractItemModel>

/*!
 * \brief The TableModel class.
 *
 * Provides data storage
 */
class TableModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    /*!
     * \brief TableModel - constructor.
     * \param parent
     */
    explicit TableModel(QObject* parent = 0);

    /*!
     * \brief setTableName - sets name of table to the model.
     * \param tableName - name.
     */
    void setTableName(const QString& tableName);

    /*!
     * \brief setData - sets data to the model.
     * \param tableData - matrix with data.
     */
    void setData(const QVector<QVector<QVariant>>& tableData);

    /*!
     * \brief setHeader - sets table headers to the model.
     * \param tableHeader - list with header.
     */
    void setHeader(const QStringList& tableHeader);

    /*!
     * \brief setTypes - stores column data types.
     * \param tableTypes - vector of types.
     */
    void setTypes(const QVector<QString>& tableTypes);

    /*!
     * \brief getTableName - returns table name.
     * \return string with table name.
     */
    QString getTableName();

    /*!
     * \brief getData - returns a data matrix.
     * \return data matrix.
     */
    QVector<QVector<QVariant>> getData();

    /*!
     * \brief getHeader - returns list of headers.
     * \return list of headers.
     */
    QStringList getHeader();

    /*!
     * \brief getTypes - returns vector of types.
     * \return vector of types.
     */
    QVector<QString> getTypes();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

    void clear();
protected:
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QString name;
    QVector<QString> types;
    QVector<QVector<QVariant>> tData;
    QStringList header;
};

#endif // TABLEMODEL_H
