#include "tablemodel.h"

TableModel::TableModel(QObject *parent): QAbstractTableModel(parent)
{
}


void TableModel::setTableName(const QString &tableName)
{
    name = tableName;
}

void TableModel::setData(const QVector<QVector<QVariant>> &tableData)
{
    if (tableData.empty())
    {
        clear();
    }
    else
    {
        tData = tableData;
        emit dataChanged(createIndex(0, 0), createIndex(tData.count(), tData.at(0).count()));
    }
}

void TableModel::setHeaderData(const QStringList& dataHeader)
{
    header = dataHeader;
}

void TableModel::clear()
{
    beginResetModel();
    tData.clear();
    endResetModel();
}


QString TableModel::getTableName()
{
    return name;
}

QVector<QVector<QVariant>> TableModel::getData()
{
    return tData;
}

QStringList TableModel::getHeaderData()
{
    return header;
}

int TableModel::rowCount(const QModelIndex&) const
{
    return tData.count();
}

int TableModel::columnCount(const QModelIndex&) const
{
    return tData.at(0).count();
}


QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (tData.empty())
        {
            return QVariant();
        }
        return tData.at(index.row()).at(index.column());
    }
    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && section < header.count())
    {
        return QVariant(header.at(section));
    }
    return QVariant();
}
