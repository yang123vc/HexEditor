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
public:
    ByteArrayListModel(QObject *parent = nullptr);
    bool open(const QString filename);
    QString getFilename() const;

public slots:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void saveCacheToFile();

signals:
    void cacheChanged();
    void cacheSaved();
};

#endif // BYTEARRAYLISTMODEL_H
