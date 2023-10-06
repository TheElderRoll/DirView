#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    model.setRootPath(QDir::homePath());

    ui->treeFileView->setModel(&model);
    homeDir = QDir::homePath();
    ui->treeFileView->setRootIndex(model.index(homeDir));
}

void MainWindow::on_submitButton_clicked()
{   
    QString filter = ui->lineEdit->text();

    if(filter.count() != 0){
        QStringList* filterList = new QStringList();
        model.setNameFilterDisables(false);
        filterList->append(filter);
        model.setNameFilters(*filterList);
    }
    else
        model.setNameFilterDisables(true);

    ui->treeFileView->setRootIndex(model.index(homeDir));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 0)
        model.setFilter(QDir::Dirs|QDir::Files|QDir::Hidden);
    else
        model.setFilter(QDir::Dirs|QDir::Files);

    ui->treeFileView->setRootIndex(model.index(homeDir));
}
