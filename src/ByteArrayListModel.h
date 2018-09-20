#ifndef BYTEARRAYLISTMODEL_H
#define BYTEARRAYLISTMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QMap>
#include <QFile>

class ByteArrayListModel : public QAbstractListModel
{
    Q_OBJECT

    mutable QFile file;
    QMap<qint64, QByteArray> editingCache;
    void writeCacheToFile(QFile &readWriteFile) const;
    void writeRowToFile(QFile &file, qint64 row, const QByteArray &array) const;
public:
    ByteArrayListModel(QObject *parent = nullptr);
    bool open(const QString filename);
    QString getFilename() const;
    bool isEdited() const;

public slots:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void save();
    bool saveAs(const QString filename);

signals:
    void cacheChanged();
    void cacheSaved();
};

#endif // BYTEARRAYLISTMODEL_H
