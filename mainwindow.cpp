#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    setWindowTitle("Chip Vision");
    connect(ui->connectButton, SIGNAL(clicked(bool)), this, SLOT(connectToChip()));
    connect(ui->selectImgButton, SIGNAL(clicked(bool)), this, SLOT(sendPicture()));
}

MainWindow::~MainWindow() {
    closeSocket(_socketId);
    delete ui;
}

void MainWindow::connectToChip() {
    _socketId = connectToServer();
    if (_socketId != -1) {
        ui->stackedWidget->setCurrentIndex(1);
        writeMessage(_socketId, "Success");
    }
    else {
        QMessageBox::warning(this, "Error connection",  "Error connect to server");
    }
}

void MainWindow::sendPicture() {
    QString file_name = QFileDialog::getOpenFileName(this, "Image", "~", "Images (*.jpg *.png)");
    char* char_file_name = file_name.toLatin1().data();

    // send image
    int n = sendImage(_socketId, char_file_name);
    if (n != -1) {
        ui->logsText->appendPlainText("sending successful");
    }
}
