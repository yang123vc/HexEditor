#ifndef BYTEARRAYDIFFMODEL_H
#define BYTEARRAYDIFFMODEL_H

#include <QMap>
#include <QFile>

#include "AbstractByteArrayModel.h"
#include "ByteArrayListModel.h"

class ByteArrayDiffModel : public AbstractByteArrayModel
{
    Q_OBJECT

    ByteArrayListModel *child1;
    ByteArrayListModel *child2;
public:
    ByteArrayDiffModel(ByteArrayListModel *child1,
                       ByteArrayListModel *child2, QObject *parent = nullptr);
    QString getFilename() const;
    bool isEdited() const;
public slots:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool save();
    /**
     * @brief saveAs
     * @param filename
     * @return false.
     */
    bool saveAs(const QString filename);

};

#endif // BYTEARRAYDIFFMODEL_H
