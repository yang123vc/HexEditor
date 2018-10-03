#include "ByteArrayListModel.h"

#include <assert.h>

ByteArrayListModel::ByteArrayListModel(QObject *parent) :
    AbstractByteArrayModel(parent)
{
}

QString ByteArrayListModel::getFilename() const
{
    return file.fileName();
}

void ByteArrayListModel::writeCacheToFile(QFile &readWriteFile) const
{
    for(QMap<qint64, QByteArray>::const_iterator it = editingCache.begin(); it != editingCache.end(); ++it) {
        const QByteArray &array = *it;
        const qint64 row = it.key();

        writeRowToFile(readWriteFile, row, array);
    }
}

void ByteArrayListModel::writeRowToFile(QFile &file, qint64 row, const QByteArray &array) const
{
    file.seek(row * 16);
    qint64 count = file.write(array);
    assert(count == array.size());
}

void ByteArrayListModel::save()
{
    if(!editingCache.empty()){
        writeCacheToFile(file);

        editingCache.clear();
        emit cacheSaved();
    }
}

bool ByteArrayListModel::saveAs(const QString filename)
{
    bool ret = false;

    if(filename == this->getFilename()){
        save();
        return true;
    }else{
        QFile saveAsFile(filename);

        if(saveAsFile.open(QFile::WriteOnly)){
            for(qint64 row = 0; row < rowCount(); row++){
                const QByteArray array = data(index(row, 0)).toByteArray();
                writeRowToFile(saveAsFile, row, array);
            }
            writeCacheToFile(saveAsFile);
            ret = true;
        }

    }

    return ret;
}

bool ByteArrayListModel::isEdited() const
{
    return !editingCache.empty();
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

int ByteArrayListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
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
                QMap<qint64, QByteArray>::const_iterator it = editingCache.find(row);
                bool foundInCache = it != editingCache.end();

                if(foundInCache){
                    ret = *it;
                }else{
                    file.seek(row * 16);
                    ret = file.read(16);
                }
            }
                break;
            default:
                break;
            }
        }
    }

    return ret;
}

QVariant ByteArrayListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant ret;
    if(orientation == Qt::Vertical){
        switch (role) {
        case Qt::DisplayRole:
        {
            qint64 begin = section * 16;
            qint64 end = section + 16 - 1;

            ret = QString::number(begin, 16).rightJustified(4, '0') + " : " +
                    QString::number(end, 16).rightJustified(4, '0');
        }
            break;
        default:
            break;
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

                const QByteArray originalArray = data(index).toByteArray();
                const QByteArray array = value.toByteArray();

                assert(array.size() <= 16);
                assert(array.size() == originalArray.size());

                if(originalArray != array){
                    editingCache.insert(row, array);
                    emit cacheChanged();
                }
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
