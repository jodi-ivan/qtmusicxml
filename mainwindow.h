#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QFrame* variantFrame ;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void VariantSelected(std::string selected);

public slots:
    void onMusicRendered(int numbered, std::string variant , int totalVariant, int totalVerse);
    void onVariantClicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
