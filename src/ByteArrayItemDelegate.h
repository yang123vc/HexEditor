#ifndef BYTEARRAYITEMDELEGATE_H
#define BYTEARRAYITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QTextCodec>

class ByteArrayItemDelegate : public QStyledItemDelegate
{
    static constexpr long bytesPerLine = 16;
    static constexpr long hexBytesCount = bytesPerLine * 2;
    static constexpr long spacesCount = bytesPerLine - 1;
    static constexpr long justifiedLen = hexBytesCount + spacesCount;
    static constexpr char representationSeparator[] = "    ";
    static constexpr long separatorLen = sizeof(representationSeparator) - 1;
    static constexpr long maximumLen = justifiedLen + separatorLen + bytesPerLine;

    mutable QTextCodec * codec;
private:
    QString getHexRepresentation(const QByteArray &array,
                                 bool justified = false) const;
    QString getAsciiRepresentation(const QByteArray &array) const;

    int drawHighlighting(QPainter *painter,
                         const QByteArray &thisArray, const QByteArray &thatArray,
                         const int pos, const QRect &rect) const;
    QString getText(const QModelIndex &index) const;
    QString getText(const QByteArray &thisArray) const;

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
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
    QSize sizeHint(const QString &text, const QFont &font) const;

    void setCodec(QTextCodec * codec) const;
};

#endif // BYTEARRAYITEMDELEGATE_H
