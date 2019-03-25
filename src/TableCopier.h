#ifndef TABLECOPIER_H
#define TABLECOPIER_H

#include <QObject>
#include <QTableView>

/**
 * @brief Класс, обеспечивающий возможность копирования из объекта QTableViewю
 */
class TableCopier : public QObject
{
    Q_OBJECT

    /// Таблица, на которую установлен данный объект.
    QTableView * const table;
    /**
     * @brief Реализует функционал фильтра событий.
     * Перехватывает горячую клавишу копирования.
     * @param obj - объект события.
     * @param event - Случившееся событие.
     * @return true если событие перехвачено, false, если обработка требуется.
     */
    bool eventFilter(QObject *obj, QEvent *event);

    /**
     * Не даем вызывать конструктор по-умолчанию. Таким образом заботимся,
     * чтобы конструктор производного класса обязательно вызвал нужный
     * конструктор
     */
    TableCopier();
public:
    explicit TableCopier(QTableView * const parent);

private slots:
    /**
     * @brief Копирует выделенное содержимое таблицы в буфер обмена.
     * Разделитель между строками '\n', между столбцами - '\t'.
     * Элементы располагаются построчно.
     */
    void copyTableToClipboard() const;
};

#endif // TABLECOPIER_H
