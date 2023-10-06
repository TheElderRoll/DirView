#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QProcess>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);

  model.setRootPath(QDir::homePath());

  ui->treeFileView->setModel(&model);
  homeDir = QDir::homePath();
  ui->treeFileView->setRootIndex(model.index(homeDir));
  model.setNameFilterDisables(
      false); // --- переместить сюда, т.к. работает не только для фильтров
              // текста, но и для QDir::NoDotAndDotDot
  ui->treeFileView->setRootIndex(model.index(homeDir));
  ui->lineEdit->setPlaceholderText("Фильтр поиска");
  ui->checkBox->setChecked(
      true); // --  активация тоггла, заодно фиксит не работающий фильтр по
             // имени, если он был произведен до setFilter в
             // on_checkBox_stateChanged
  ui->treeFileView->setSelectionMode(QAbstractItemView::SingleSelection);
  const auto header = ui->treeFileView->header();
  header->setSectionResizeMode(0, QHeaderView::Stretch);
  header->setSectionResizeMode(1, QHeaderView::Interactive);
  header->setSectionResizeMode(2, QHeaderView::Interactive);
  header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  header->setStretchLastSection(false);
}

void MainWindow::on_termButton_clicked() {
  qInfo()<<"term";
  const auto selected = ui->treeFileView->selectionModel()->selectedIndexes();
  if (selected.size() == 0)
    return;
  QString selectedPath = model.filePath(selected[0]);
  QFileInfo fi(selectedPath);
  if (!fi.exists()) {
    return;
    // какую нить ошибку еще вывести
  }
  if (fi.isFile()) {
    selectedPath = fi.absolutePath();
  }
  std::unique_ptr<QProcess> proc(new QProcess());
  //  по хорошему вместо konsole должно быть $TERM, но у меня не заработало
  const auto term =
        QProcessEnvironment::systemEnvironment().value("TERM", "xterm");
    proc->startDetached(term, {}, selectedPath, nullptr);

}

void MainWindow::on_submitButton_clicked() {
  QString filter = ui->lineEdit->text();

  if (filter.count() != 0) {
    QStringList filterList; // поменять на объект
    filterList.append(filter);
    model.setNameFilters(filterList);
  } else {
    model.setNameFilters(
        {}); // присвоить нулевой фильтр, тем самым отключить его
  }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_checkBox_stateChanged(int arg1) {
  if (arg1 == 0)
    model.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files |
                    QDir::Hidden);
  else
    model.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files);
}


