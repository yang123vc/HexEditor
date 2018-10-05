#ifndef BYTEARRAYITEMDELEGATE_H
#define BYTEARRAYITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QTextCodec>

class ByteArrayItemDelegate : public QStyledItemDelegate
{
    mutable QTextCodec * codec;
private:
    QString getHexRepresentation(const QByteArray &array,
                                 bool justified = false) const;
    QString getAsciiRepresentation(const QByteArray &array) const;

    int drawHighlighting(QPainter *painter, const QString &thisStr, const QString &thatStr, const int pos, const QRect &rect) const;

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

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
    QSize sizeHint(const QString &text, const QFont &font) const;
    void getText(const QModelIndex &index, QString &text) const;

    void setCodec(QTextCodec * codec) const;
};

#endif // BYTEARRAYITEMDELEGATE_H
