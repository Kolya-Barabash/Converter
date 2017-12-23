#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractItemModel>

class TableModel: public QAbstractTableModel
{
    Q_OBJECT
  public:
    //using QAbstractTableModel::setData;

    explicit TableModel(QObject* parent = 0);

    void setTableName(const QString& tableName);
    void setData(const QVector<QVector<QVariant>>& tableData);
    void setHeaderData(const QStringList& dataHeader);

    QString getTableName();
    QVector<QVector<QVariant>> getData();
    QStringList getHeaderData();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

    void clear();

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  private:
    QString name;
    QVector<QString> types;
    QVector<QVector<QVariant>> tData;
    QStringList header;
};

#endif // TABLEMODEL_H
