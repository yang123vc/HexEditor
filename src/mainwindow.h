#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QComboBox>

#include "ByteArrayListModel.h"
#include "ByteArrayDiffModel.h"
#include "ByteArrayItemDelegate.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTableView * dataView;
    ByteArrayItemDelegate delegate;
    AbstractByteArrayModel *model;
    QComboBox *codecComboBox;

    void closeEvent(QCloseEvent *event);

    QString titleBase() const;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void read();
    void diff();
    void saveAs();
    void save();
    void selectFont();
    void cacheChanged();
    void cacheSaved();
    void updateDelegateCodec();
};

#endif // MAINWINDOW_H
