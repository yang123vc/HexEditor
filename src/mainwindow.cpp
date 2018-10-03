#include "mainwindow.h"

#include <QVBoxLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QHeaderView>

#include <QFileDialog>
#include <QFontDialog>
#include <QFont>
#include <QMessageBox>

#include <QApplication>

#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      model(nullptr)
{
    setCentralWidget(new QWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget());

    {
        QFont f("Monospace");
        f.setStyleHint(QFont::Monospace);
        dataView = new QTableView;
        layout->addWidget(dataView);
        dataView->setItemDelegate(&delegate);
        dataView->horizontalHeader()->hide();
        dataView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        dataView->setFont(f);
    }

    setMenuBar(new QMenuBar);
    {
        {
            QMenu * fileMenu = menuBar()->addMenu("File");
            {
                QAction *openAction = fileMenu->addAction("Open");
                connect(openAction, SIGNAL(triggered(bool)), SLOT(read()));
            }
            {
                QAction *openAction = fileMenu->addAction("Diff");
                connect(openAction, SIGNAL(triggered(bool)), SLOT(diff()));
            }
            {
                saveAction = fileMenu->addAction("Save");
            }
            {
                QAction *saveAsAction = fileMenu->addAction("Save as...");
                connect(saveAsAction, SIGNAL(triggered(bool)), SLOT(saveAs()));
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
    setMinimumSize(800, 400);
}

MainWindow::~MainWindow()
{
}

void MainWindow::read()
{
    QString fileName = QFileDialog::getOpenFileName(0, "Open file", QString(),
                                                    "Executable (*.exe);;"
                                                    "DLL (*.dll);;"
                                                    "Binary (*.bin);;"
                                                    "All (*.*)");
    if(!fileName.isEmpty()){
        ByteArrayListModel *model = new ByteArrayListModel(this);
        this->model = model;

        if(model->open(fileName)){

            if(dataView->model()){
                dataView->model()->deleteLater();
            }

            dataView->setModel(model);

            setWindowTitle(titleBase());
            statusBar()->showMessage(QString("File %1 is opened").arg(
                                         model->getFilename()), 10000);

            connect(model, SIGNAL(cacheChanged()), SLOT(cacheChanged()));
            connect(model, SIGNAL(cacheSaved()), SLOT(cacheSaved()));
            connect(saveAction, SIGNAL(triggered(bool)),
                    model, SLOT(save()));
        }else{
            model->deleteLater();
            model = nullptr;
        }
    }else{
        statusBar()->showMessage("File opening error", 10000);
    }
}

void MainWindow::diff()
{
    QString fileName1 = QFileDialog::getOpenFileName(0, "Open FIRST file", QString(),
                                                    "Executable (*.exe);;"
                                                    "DLL (*.dll);;"
                                                    "Binary (*.bin);;"
                                                    "All (*.*)");
    QString fileName2 = QFileDialog::getOpenFileName(0, "Open SECOND file", QString(),
                                                    "Executable (*.exe);;"
                                                    "DLL (*.dll);;"
                                                    "Binary (*.bin);;"
                                                    "All (*.*)");
    if((!fileName1.isEmpty()) && (!fileName2.isEmpty())){

        ByteArrayListModel *model1 = new ByteArrayListModel(this);
        ByteArrayListModel *model2 = new ByteArrayListModel(this);
        ByteArrayDiffModel *model = new ByteArrayDiffModel(model1, model2);
        this->model = model;

        if(model1->open(fileName1) &&
                model2->open(fileName2) &&
                (fileName1 != fileName2)){

            if(dataView->model()){
                dataView->model()->deleteLater();
            }

            dataView->setModel(model);

            setWindowTitle(titleBase());
            statusBar()->showMessage(
                        QString("Files %1, %2 is opened").arg(
                            model1->getFilename()).arg(
                            model2->getFilename()), 10000);

            connect(model, SIGNAL(cacheChanged()), SLOT(cacheChanged()));
            connect(model, SIGNAL(cacheSaved()), SLOT(cacheSaved()));
            connect(saveAction, SIGNAL(triggered(bool)),
                    model, SLOT(save()));
        }else{
            model1->deleteLater();
            model1 = nullptr;
        }
    }else{
        statusBar()->showMessage("File opening error", 10000);
    }
}

void MainWindow::saveAs()
{
    const QString filename = QFileDialog::getSaveFileName();

    if(!filename.isEmpty()){
        model->saveAs(filename);
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

void MainWindow::cacheChanged()
{
    setWindowTitle(titleBase() + "*");
}

void MainWindow::cacheSaved()
{
    setWindowTitle(titleBase());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if ((model != nullptr) && model->isEdited()) {
        int button = QMessageBox::question(nullptr, "Close request",
                              "There are unsaved changes.\r\nSure to close the application?",
                              QMessageBox::Ok,
                              QMessageBox::Cancel);
        if(button == QMessageBox::Ok){
            event->accept();
        }else {
            event->ignore();
        }
    }
}

QString MainWindow::titleBase() const
{
    QString str = QApplication::applicationName();

    if(model != nullptr){
        str+= QString(" - ") + model->getFilename();
    }

    return str;
}
