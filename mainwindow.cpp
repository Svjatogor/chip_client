#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToChip() {
    _socketId = connectToServer();
    if (_socketId != -1) {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else {
        QMessageBox::warning(this, "Error connection",  "Error connect to server");
    }
}
