#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>

#include "ByteArrayListModel.h"
#include "ByteArrayItemDelegate.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QListView * dataView;
    QAction *saveAction;
    ByteArrayItemDelegate delegate;

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
