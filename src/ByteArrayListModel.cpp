#include "ByteArrayListModel.h"

#include <assert.h>

#include <QCryptographicHash>
#include <QFileInfo>

ByteArrayListModel::ByteArrayListModel(QObject *parent) :
    AbstractByteArrayModel(parent)
{
}

QString ByteArrayListModel::getFilename() const
{
    return file->fileName();
}

bool ByteArrayListModel::writeCacheToFile(QFile &readWriteFile) const
{
    bool ret = true;

    for(QMap<qint64, QByteArray>::const_iterator it = editingCache.begin(); it != editingCache.end(); ++it) {
        const QByteArray &array = *it;
        const qint64 row = it.key();

        ret&= writeRowToFile(readWriteFile, row, array);
    }

    return ret;
}

bool ByteArrayListModel::writeRowToFile(QFile &file, qint64 row, const QByteArray &array) const
{
    file.seek(row * 16);
    const qint64 count = file.write(array);
    assert(count == array.size());

    return (count == array.size());
}

bool ByteArrayListModel::save()
{
    bool ret = true;
    if(!editingCache.empty()){
        const bool ok = saveAs(file->fileName());

        if(ok){
            editingCache.clear();
            emit cacheSaved();
        }else{
            ret = false;
        }
    }
    return ret;
}

bool ByteArrayListModel::saveAs(const QString filename)
{
    bool ret = false;

    if(!file.isNull() && file->isOpen()){
        QFile saveAsFile(filename);
        const bool fileChanging = filename != file->fileName();
        const bool opened = (fileChanging) ?
                    saveAsFile.open(QFile::WriteOnly) :
                    saveAsFile.open(QFile::ReadWrite);

        ret = opened;
        if(opened){
            if(fileChanging){
                for(qint64 row = 0; row < rowCount(); row++){
                    const QByteArray array = data(index(row, 0)).toByteArray();
                    ret&= writeRowToFile(saveAsFile, row, array);
                }
            }

            ret&= writeCacheToFile(saveAsFile);
            saveAsFile.close();

            if(fileChanging){
                QSharedPointer<QFile> reopenFile(new QFile(filename));
                ret&= reopenFile->open(QIODevice::ReadOnly);
                file.swap(reopenFile);
                reopenFile->close();
            }
        }
    }

    return ret;
}

bool ByteArrayListModel::isEdited() const
{
    return !editingCache.empty();
}

QString ByteArrayListModel::getHash() const
{
    return hash;
}

int ByteArrayListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    long ret = 0;
    if(!file.isNull() && file->isOpen()){
        const long size = file->size();
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

    if(!file.isNull() && file->isOpen()){
        const int row = index.row();

        if(row < rowCount()){
            switch (role) {
            case Qt::DisplayRole:
            {
                QMap<qint64, QByteArray>::const_iterator it = editingCache.find(row);
                const bool foundInCache = it != editingCache.end();

                if(foundInCache){
                    ret = *it;
                }else{
                    file->seek(row * 16);
                    ret = file->read(16);
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
    switch (role) {
    case Qt::DisplayRole:
    {
        if(orientation == Qt::Vertical){
            const qint64 begin = section * 16;
            const qint64 end = section + 16 - 1;

            ret = QString::number(begin, 16).rightJustified(4, '0') + " : " +
                    QString::number(end, 16).rightJustified(4, '0');
        }else
        {
            QFileInfo info(*file.data());
            QString filename = info.fileName();
            QString path = info.absolutePath();
            QString fullpath = info.filePath();

            constexpr int maxLen = 16 * 4;
            constexpr char dots[] = "...";

            if(fullpath.length() <= maxLen){
                ret = fullpath;
            }else if(filename.length() <= maxLen){
                QString header = QString(dots) + filename;
                const int pathLen = header.length() >= maxLen ? 0 :
                                                                maxLen - header.length();
                header.prepend(path.left(pathLen));

                ret = header;
            }else{
                ret = filename;
            }
        }
    }
        break;
    default:
        break;
    }
    return ret;
}

bool ByteArrayListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool ret = false;

    if(!file.isNull() && file->isOpen()){
        const int row = index.row();

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
    hash = calculateHash(filename);
    file = QSharedPointer<QFile>(new QFile(filename));

    return file->open(QFile::ReadOnly);
}

QString ByteArrayListModel::calculateHash(const QString &filename) const
{
    QCryptographicHash md5gen(QCryptographicHash::Md5);
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    md5gen.addData(&file);
    file.close();
    return md5gen.result().toHex();
}

Qt::ItemFlags ByteArrayListModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
