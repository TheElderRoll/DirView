#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_submitButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_termButton_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel model;
    QString homeDir;
};
#endif // MAINWINDOW_H
