#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->currentHymn = 1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_textEdit_textChanged()
{

    // Alternative: Get the widget via sender() if needed dynamically
    QTextEdit* senderWidget = qobject_cast<QTextEdit*>(sender());

    QString value = senderWidget->toPlainText();

    qInfo() << value;
    if (!(!!value.toInt())) {
        if (!value.isEmpty()) {
            value.remove(value.length() - 1, 1);
            senderWidget->setText(value);
            senderWidget->moveCursor(QTextCursor::End);
        }
        return;
    }

    int newVal = value.toInt();
    if (newVal == this->currentHymn) {
        return ;
    }
    this->currentHymn = newVal;
    emit this->changeMusic(newVal, "", 0, false);

    // validate

}

void MainWindow::on_selecteor_pressed()
{
    // Alternative: Get the widget via sender() if needed dynamically
    QPushButton* senderWidget = qobject_cast<QPushButton*>(sender());
    qInfo() << "clicked" << senderWidget;
    if (senderWidget) {
        qInfo() << senderWidget->objectName();
    }

    // unlock
    // editing mode
    // focus
    // if editing ? append : replaced --> the text editor
    // lock

    // if enter
    // emit changed

    // if backspace
    // cursor management stuff

    // enter
    // emit:wq

}

