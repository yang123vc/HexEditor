#ifndef BYTEARRAYITEMDELEGATE_H
#define BYTEARRAYITEMDELEGATE_H

#include <QStyledItemDelegate>

class ByteArrayItemDelegate : public QStyledItemDelegate
{
public:
    explicit ByteArrayItemDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // BYTEARRAYITEMDELEGATE_H
