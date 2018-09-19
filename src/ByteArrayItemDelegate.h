#ifndef BYTEARRAYITEMDELEGATE_H
#define BYTEARRAYITEMDELEGATE_H

#include <QStyledItemDelegate>

class ByteArrayItemDelegate : public QStyledItemDelegate
{
private:
    QString getHexRepresentation(const QByteArray &array,
                                 bool justified = false) const;
    QString getAsciiRepresentation(const QByteArray &array) const;

public:
    explicit ByteArrayItemDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // BYTEARRAYITEMDELEGATE_H
