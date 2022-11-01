#include "transactiontable.h"

TransactionTable::TransactionTable(QObject *parent):
    QAbstractTableModel(parent),
    datas(),
    headerDatas(2)
{
}
TransactionTable::TransactionTable(std::vector<QStringList> _datas,QObject *parent):
    QAbstractTableModel(parent),
    datas(_datas),
    headerDatas(2)
{
}

QVariant TransactionTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation ==Qt::Horizontal){
        return headerDatas[0][section];
    }
    else if(orientation == Qt::Vertical){
        return headerDatas[1][section];
    }
    else{
        return QVariant();
    }
}

bool TransactionTable::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        headerDatas[orientation][section] = value.toString();
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}


int TransactionTable::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    return datas.size();
}

int TransactionTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return datas.begin()->count();
}

bool TransactionTable::hasChildren(const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

bool TransactionTable::canFetchMore(const QModelIndex &parent) const
{
    // FIXME: Implement me!
    return false;
}

void TransactionTable::fetchMore(const QModelIndex &parent)
{
    // FIXME: Implement me!
}

QVariant TransactionTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!

    return QVariant(datas[index.row()][index.column()]);
}

bool TransactionTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        datas[index.row()][index.column()] = value.toString();
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags TransactionTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool TransactionTable::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    endInsertRows();
    return true;
}

bool TransactionTable::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool TransactionTable::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

bool TransactionTable::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}
