#include "ByteArrayListModel.h"

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
                file.seek( row * 16);
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

bool ByteArrayListModel::open(const QString filename)
{
    file.setFileName(filename);

    return file.open(QFile::ReadOnly);
}
