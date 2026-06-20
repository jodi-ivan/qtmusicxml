// #include <QGuiApplication>
#include <QApplication>
#include <QMainWindow>
#include <QQmlApplicationEngine>
#include <QSvgRenderer>
#include <QByteArray>
#include <QSvgWidget>
#include <QVBoxLayout>
#include <QFontDatabase>
#include <QDir>
#include <QScrollArea>
#include <QPushButton>


#include "libhymn_renderer.h"
#include "mainwindow.h"
#include "musicrenderer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    // engine.loadFromModule("qtmusicxml", "Main");



    QDir appDir(QCoreApplication::applicationDirPath());

    // Go up two folders
    appDir.cdUp();
    appDir.cdUp();

    std::string fonts[5] = {"caladea.ttf", "figtree.ttf", "mozart11.ttf", "noto-music.ttf", "old-standard-tt.ttf"};

    for (int i = 0; i < 5; i++) {
        QString fullpath = appDir.absolutePath() + QString("/resource/") + QString::fromStdString(fonts[i]);

        int fontId = QFontDatabase::addApplicationFont(fullpath);
        qInfo() << "Loaded " << fontId << fonts[i] << "path" << fullpath;
        if (fontId != -1) {
            QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
            qInfo() << fontFamilies;
        }
    }

    MusicRenderer musicRenderer;
    MainWindow mainWindow;

    QObject::connect(&mainWindow, &MainWindow::changeMusic, &musicRenderer, &MusicRenderer::onChange, Qt::QueuedConnection);

    mainWindow.setStyleSheet(" .QWidget { border: 1px solid red; } ");

    mainWindow.setWindowTitle("Embedded SVG Example");
   //  mainWindow.resize(800, 1080);

    // 3. Create a layout container to hold our interface pieces
    // QRect layoutCWid;
   // QWidget *centralWidget = new QWidget(&mainWindow);

    // QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    // // QBoxLayout::SizeConstraint h;

    // layout->setSizeConstraints(QBoxLayout::SetFixedSize, QBoxLayout::SetFixedSize);

    QGridLayout* buttonHolder = mainWindow.findChild<QGridLayout*>("btn_selector_layout");

    QList<QPushButton*> allButtons = buttonHolder->findChildren<QPushButton*>();


    char* fromDll  = RenderHymnSVG(434, nullptr, nullptr);
    // 1. Your raw SVG string content
    QString svgString =  QString::fromUtf8(fromDll);

    // 2. Convert to QByteArray
    QByteArray svgBytes = svgString.toUtf8();

    // 3. Load via QSvgRenderer
    QSvgRenderer* renderer = new QSvgRenderer(&app);

    // Syntax: this->findChild<WidgetType*>("objectName");
    QScrollArea* area = mainWindow.findChild<QScrollArea*>("scrollArea");
    area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    area->setAlignment(Qt::AlignHCenter);
    qInfo() << "area found" << area;

    if (renderer->load(svgBytes)) {
        // Successfully loaded!

        // Example A: Use it inside a QSvgWidget
        QSvgWidget* svgWidget = new QSvgWidget();
        svgWidget->setStyleSheet("background-color:white;");
        svgWidget->setFixedSize(800, 3000);
        svgWidget->renderer()->load(svgBytes);
        if (area) {
            area->setWidget(svgWidget);
        }
    }
    // 7. Call show ONLY on the root window frame.
    // Children track parent visibility states dynamically and display automatically.
    mainWindow.show();
    delete fromDll;
    return app.exec();
}



