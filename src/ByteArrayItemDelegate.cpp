#include "ByteArrayItemDelegate.h"

#include <assert.h>

#include <QApplication>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QPainter>

ByteArrayItemDelegate::ByteArrayItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QString ByteArrayItemDelegate::getHexRepresentation(const QByteArray &array, bool justified) const
{
    const long hexBytesCount = 16 * 2;
    const long spacesCount = 16 - 1;
    const long justifiedLen = hexBytesCount + spacesCount;

    QString str = array.toHex(' ');

    if(justified){
        str = str.leftJustified(justifiedLen, ' ');
    }

    return str;
}

QString ByteArrayItemDelegate::getAsciiRepresentation(const QByteArray &array) const
{
    QString str;
    str.resize(array.size());

    for(int i = 0; i < array.size(); i++){
        const bool printAsIs = QChar::isLetterOrNumber(array[i]) ||
                QChar::isPunct(array[i]);

        if(!printAsIs){
            str[i] = '.';
        }else{
            str[i] = array[i];
        }
    }

    return str;
}

void ByteArrayItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QString thisStr;
    getText(index, thisStr);

    painter->fillRect(option.rect, QBrush(Qt::white));
    painter->setFont(option.font);
    {
        int siblingCol = (index.column() == 1) ? 0 : 1;
        const QModelIndex siblingIndex = index.sibling(index.row(), siblingCol);
        if(siblingIndex.isValid()){
            QString thatStr;
            getText(siblingIndex, thatStr);

            int i = 0;
            while((i < thisStr.length())){
                i = drawHighlighting(painter, thisStr, thatStr, i, option.rect);
            }
        }
    }

    painter->drawText(option.rect, Qt::AlignVCenter, thisStr);
}

int ByteArrayItemDelegate::drawHighlighting(QPainter *painter, const QString &thisStr,
                                            const QString &thatStr, const int pos,
                                            const QRect &rect) const
{
    int i = pos;
    bool diff = false;

    while((i < thisStr.length())){
        if((thatStr.length() <= i) ||
                (thatStr[i] != thisStr[i])){
            diff = true;
            i++;
        }else{
            i++;
            break;
        }
    }

    if(diff){
        QFontMetrics fontMetrics(painter->font());
        int begin = rect.left() + fontMetrics.width(thisStr.left(pos));
        int end = rect.left() + fontMetrics.width(thisStr.left(i));
        painter->save();
        painter->setBrush(QBrush(Qt::yellow));
        painter->setPen(QPen(Qt::NoPen));

        QRect redrawRect(QPoint(begin, rect.top()), QPoint(end, rect.bottom()));
        painter->drawRect(redrawRect);
        painter->restore();
    }
    return i;
}

QWidget *ByteArrayItemDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &/* option */,
                                       const QModelIndex &/* index */) const
{
    QLineEdit *const editor = new QLineEdit(parent);
    editor->setFrame(false);

    const QRegExp regexp("[0-f]{1,2}(\\s[0-f]{1,2})*$");
    editor->setValidator(new QRegExpValidator(regexp, editor));

    return editor;
}

void ByteArrayItemDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    const QByteArray array = index.data().toByteArray();

    QLineEdit *const lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(getHexRepresentation(array));
}

void ByteArrayItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    const QVariant originalValue = model->data(index, Qt::DisplayRole);

    assert(originalValue.type() == QVariant::ByteArray);

    const QByteArray originalArray = originalValue.toByteArray();
    const int originalArraySize = originalArray.size();

    const QLineEdit *const lineEdit = static_cast<QLineEdit*>(editor);

    const QRegExp regexp("[0-f]{2}(\\s[0-f]{2})*$");
    QString hexString = lineEdit->text();
    if(regexp.exactMatch(hexString)){
        const QByteArray array = QByteArray::fromHex(hexString.toLatin1());

        if(array.size() == originalArraySize){
            model->setData(index, array, Qt::EditRole);
        }
    }
}

void ByteArrayItemDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

QSize ByteArrayItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QString text;
    getText(index, text);

    return sizeHint(text, option.font);
}

QSize ByteArrayItemDelegate::sizeHint(const QString &text, const QFont &font) const
{
    QFontMetrics fontMetrics(font);
    QSize s(fontMetrics.width(text), fontMetrics.height());

    return s;
}

void ByteArrayItemDelegate::getText(const QModelIndex &index, QString &text) const
{
    const QByteArray thisArray = index.data().toByteArray();

    text = getHexRepresentation(thisArray, true) +
            "    " + getAsciiRepresentation(thisArray);
}
