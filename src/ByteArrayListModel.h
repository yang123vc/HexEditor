#ifndef BYTEARRAYLISTMODEL_H
#define BYTEARRAYLISTMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QFile>

class ByteArrayListModel : public QAbstractListModel
{
    Q_OBJECT

    mutable QFile file;
public:
    ByteArrayListModel(QObject *parent = nullptr);
    bool open(const QString filename);

public slots:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

#endif // BYTEARRAYLISTMODEL_H
