#ifndef BYTEARRAYMODEL_H
#define BYTEARRAYMODEL_H

#include <QAbstractTableModel>
class AbstractByteArrayModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    AbstractByteArrayModel(QObject *parent = nullptr);

    virtual QString getFilename() const = 0;
    virtual bool isEdited() const = 0;

public slots:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const = 0;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const = 0;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const = 0;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const = 0;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) = 0;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const = 0;

    virtual bool save() = 0;
    virtual bool saveAs(const QString filename) = 0;

signals:
    void cacheChanged();
    void cacheSaved();
};

#endif // BYTEARRAYMODEL_H
