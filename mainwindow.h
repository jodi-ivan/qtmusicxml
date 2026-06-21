#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    int currentHymn;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
     bool changeMusic(int num, std::string variant, int verse, bool focusMode);

public slots:
    void on_selecteor_pressed();

private slots:
    void on_textEdit_textChanged();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
