#include "ByteArrayListModel.h"

#include <assert.h>

ByteArrayListModel::ByteArrayListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int ByteArrayListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    long ret = 0;
    if(file.isOpen()){
        long size = file.size();
        ret = size / 16 +
                (((size % 16) > 0) ? 1 : 0);
    }
    return ret;
}

QVariant ByteArrayListModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;

    if(file.isOpen()){
        int row = index.row();

        if(row < rowCount()){
            switch (role) {
            case Qt::DisplayRole:
            {
                file.seek(row * 16);
                ret = file.read(16);
            }
                break;
            default:
                break;
            }
        }
    }

    return ret;
}

bool ByteArrayListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool ret = false;

    if(file.isOpen()){
        int row = index.row();

        if(row < rowCount()){
            switch (role) {
            case Qt::EditRole:
            {
                assert(value.type() == QVariant::ByteArray);

                QByteArray array = value.toByteArray();

                assert(array.size() <= 16);

                file.seek(row * 16);
                qint64 count = file.write(array);

                assert(count == array.size());
            }
                break;
            default:
                break;
            }
        }
    }

    return ret;
}

bool ByteArrayListModel::open(const QString filename)
{
    file.setFileName(filename);

    return file.open(QFile::ReadWrite);
}

Qt::ItemFlags ByteArrayListModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}
