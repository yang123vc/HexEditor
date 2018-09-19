#include "ByteArrayItemDelegate.h"

#include <assert.h>

#include <QApplication>
#include <QLineEdit>
#include <QRegExpValidator>

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

    const QByteArray array = index.data().toByteArray();

    const QString str = getHexRepresentation(array) +
            "    " + getAsciiRepresentation(array);
    opt.text = str;

    QApplication::style()->drawControl(QStyle::CE_ItemViewItem,
                                       &opt, painter);
}

QWidget *ByteArrayItemDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &/* option */,
                                       const QModelIndex &/* index */) const
{
    QLineEdit *const editor = new QLineEdit(parent);
    editor->setFrame(false);

    const QRegExp regexp("[0-f]{2}(\\s[0-f]{2})*$");
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
    const QString hexString = lineEdit->text().remove(' ');
    const QByteArray array = QByteArray::fromHex(hexString.toLatin1());

    if(array.size() == originalArraySize){
        model->setData(index, array, Qt::EditRole);
    }
}

void ByteArrayItemDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
