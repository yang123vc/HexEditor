#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>

#include "ByteArrayListModel.h"
#include "ByteArrayItemDelegate.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTableView * dataView;
    QAction *saveAction;
    ByteArrayItemDelegate delegate;
    ByteArrayListModel *model;

    void closeEvent(QCloseEvent *event);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void read();
    void selectFont();
    void cacheChanged();
    void cacheSaved();
};

#endif // MAINWINDOW_H
