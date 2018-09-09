#include "ByteArrayItemDelegate.h"
#include <QApplication>

ByteArrayItemDelegate::ByteArrayItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void ByteArrayItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QByteArray array = index.data().toByteArray();

    for(int i = 0; i < array.size(); i++){
        bool printAsIs = QChar::isLetterOrNumber(array[i]) ||
                          QChar::isPunct(array[i]);
        if(!printAsIs){
            array[i] = '.';
        }
    }

    long hexBytesCount = 16 * 2;
    long spacesCount = 16 - 1;
    long justifiedLen = hexBytesCount + spacesCount;
    QString str = array.toHex(' ').leftJustified(justifiedLen, ' ') +
            "    " + array;
    opt.text = str;

    QApplication::style()->drawControl(QStyle::CE_ItemViewItem,
                                                 &opt, painter);
}

