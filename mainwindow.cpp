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
      false);
  ui->treeFileView->setRootIndex(model.index(homeDir));
  ui->lineEdit->setPlaceholderText("Фильтр поиска");
  ui->checkBox->setChecked(
      true);
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
  }
  if (fi.isFile()) {
    selectedPath = fi.absolutePath();
  }
  std::unique_ptr<QProcess> proc(new QProcess());
  const auto term =
        QProcessEnvironment::systemEnvironment().value("TERM", "xterm");
    proc->startDetached(term, {}, selectedPath, nullptr);

}

void MainWindow::on_submitButton_clicked() {
  QString filter = ui->lineEdit->text();

  if (filter.count() != 0) {
    QStringList filterList;
    filterList.append(filter);
    model.setNameFilters(filterList);
  } else {
    model.setNameFilters(
        {});
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


