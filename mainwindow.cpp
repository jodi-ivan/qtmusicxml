#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_textEdit_textChanged()
{
    // 1. Get the text directly from the UI pointer
    QString currentText = ui->textEdit->toPlainText();

    // Alternative: Get the widget via sender() if needed dynamically
    QTextEdit* senderWidget = qobject_cast<QTextEdit*>(sender());

    QString value = senderWidget->toPlainText();

    qInfo() << value;
    emit this->changeMusic(1, "", 0, false);
}

