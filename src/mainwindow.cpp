#include "mainwindow.h"

#include <QVBoxLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QFontDialog>

#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new QWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget());

    dataView = new QListView;
    layout->addWidget(dataView);
    dataView->setItemDelegate(&delegate);

    setMenuBar(new QMenuBar);
    {
        {
            QMenu * fileMenu = menuBar()->addMenu("File");
            {
                QAction *readAction = fileMenu->addAction("Read");
                connect(readAction, SIGNAL(triggered(bool)), SLOT(read()));
            }
        }
        {
            QMenu * settingsMenu = menuBar()->addMenu("Settings");
            {
                QAction *fontsAction = settingsMenu->addAction("Fonts");
                connect(fontsAction, SIGNAL(triggered(bool)), SLOT(selectFont()));
            }
        }
    }

    setStatusBar(new QStatusBar);
}

MainWindow::~MainWindow()
{
}

void MainWindow::read()
{
    QString fileName = QFileDialog::getOpenFileName(0, "Open file", "C:/",
                                                    "Executable (*.exe);;"
                                                    "DLL (*.dll);;"
                                                    "Binary (*.bin);;"
                                                    "All (*.*)");
    if(!fileName.isEmpty()){
        ByteArrayListModel *model = new ByteArrayListModel(this);

        if(model->open(fileName)){
            statusBar()->showMessage("File is opened", 10000);
        }

        dataView->setModel(model);
    }else{
        statusBar()->showMessage("File opening error", 10000);
    }
}

void MainWindow::selectFont()
{
    bool got = false;
    QFont font = QFontDialog::getFont(&got, QApplication::font(), 0, QString(),
                                      QFontDialog::MonospacedFonts);
    if(got){
        dataView->setFont(font);
    }
}
