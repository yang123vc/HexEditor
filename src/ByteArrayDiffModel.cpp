#include "ByteArrayDiffModel.h"

#include <QBrush>
#include <QColor>

#include <algorithm>
#include <assert.h>

ByteArrayDiffModel::ByteArrayDiffModel(ByteArrayListModel *child1, ByteArrayListModel *child2, QObject *parent) :
    AbstractByteArrayModel(parent),
    child1(child1),
    child2(child2)
{
    child1->setParent(this);
    child2->setParent(this);

    connect(child1, SIGNAL(cacheChanged()), SIGNAL(cacheChanged()));
    connect(child2, SIGNAL(cacheChanged()), SIGNAL(cacheChanged()));

    connect(child1, SIGNAL(cacheSaved()), SIGNAL(cacheSaved()));
    connect(child2, SIGNAL(cacheSaved()), SIGNAL(cacheSaved()));
}

QString ByteArrayDiffModel::getFilename() const
{
    return child1->getFilename() + " " + child2->getFilename();
}

bool ByteArrayDiffModel::isEdited() const
{
    return child1->isEdited() || child2->isEdited();
}

int ByteArrayDiffModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return std::max(child1->rowCount(), child2->rowCount());
}

int ByteArrayDiffModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ByteArrayDiffModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;

    //if(file.isOpen()){
        int row = index.row();
        int col = index.column();

        if(row < rowCount()){
            switch (role) {
            case Qt::DisplayRole:
            {
                if(col == 0){
                    ret = child1->data(child1->index(row, 0));
                }else{
                    ret = child2->data(child2->index(row, 0));
                }
            }
                break;
            default:
                break;
            }
        }
    //}

    return ret;
}

QVariant ByteArrayDiffModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool ByteArrayDiffModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool ret = false;

//    if(file.isOpen()){
        int row = index.row();
        int col = index.column();

        if(row < rowCount()){
            switch (role) {
            case Qt::EditRole:
            {
                if(col == 0){
                    child1->setData(child1->index(row, 0), value);
                }else{
                    child2->setData(child2->index(row, 0), value);
                }
            }
                break;
            default:
                break;
            }
        }
//    }

    return ret;
}

Qt::ItemFlags ByteArrayDiffModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

bool ByteArrayDiffModel::save()
{
    return child1->save() &&
            child2->save();
}
