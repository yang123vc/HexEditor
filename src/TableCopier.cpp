#include "TableCopier.h"

#include <assert.h>

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QShortcut>
#include <QKeyEvent>

TableCopier::TableCopier(QTableView * const parent) :
    QObject(parent),
    table(parent)
{    
    assert(parent != nullptr);
    // Создать всплывающее контекстное меню.
    QAction *const copyAction = new QAction("Copy", this);
    parent->addAction(copyAction);
    parent->setContextMenuPolicy(Qt::ActionsContextMenu);

    // Установить данный объект как фильтр событий для таблицы.
    parent->installEventFilter(this);

    connect(copyAction, SIGNAL(triggered(bool)), SLOT(copyTableToClipboard()));
}

bool TableCopier::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        const QKeyEvent *const keyEvent = static_cast<QKeyEvent *>(event);
        // Горячая клавиша копирования
        if((keyEvent->key() == Qt::Key_C) && (keyEvent->modifiers() == Qt::CTRL)){
            copyTableToClipboard();
        }
        return true;
    }else{ // Передать событие дальше
        return QObject::eventFilter(obj, event);
    }
}

void TableCopier::copyTableToClipboard() const
{
    const QModelIndexList selectedIndexes =
            table->selectionModel()->selectedIndexes();
    QString clipBoardText;

    if(!selectedIndexes.isEmpty()){
        int row = selectedIndexes.first().row();
        foreach (QModelIndex index, selectedIndexes) {
            if(row != index.row()){// Если новая строка - отделим ее
                clipBoardText.append("\n");
                row = index.row();
            }else if(!clipBoardText.isEmpty()){// Если на этой строке есть столбец - отделим его
                clipBoardText.append("\t");
            }

            clipBoardText.append(table->model()->data(index).toString());
        }

        QApplication::clipboard()->setText(clipBoardText);
    }
}
