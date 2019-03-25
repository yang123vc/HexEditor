#ifndef BYTEARRAYLISTMODEL_H
#define BYTEARRAYLISTMODEL_H

#include <AbstractByteArrayModel.h>
#include <QByteArray>
#include <QMap>
#include <QFile>
#include <QSharedPointer>

class ByteArrayListModel : public AbstractByteArrayModel
{
    Q_OBJECT

    QString hash;
    mutable QSharedPointer<QFile> file;
    QMap<qint64, QByteArray> editingCache;
    bool writeCacheToFile(QFile &readWriteFile) const;
    bool writeRowToFile(QFile &file, qint64 row, const QByteArray &array) const;
    QString calculateHash(const QString &filename) const;
public:
    explicit ByteArrayListModel(QObject *parent = nullptr);
    bool open(const QString filename);
    QString getFilename() const;
    bool isEdited() const;
    QString getHash() const;

public slots:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool save();
    bool saveAs(const QString filename);
};

#endif // BYTEARRAYLISTMODEL_H
