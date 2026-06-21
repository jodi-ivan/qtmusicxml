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
#include <QButtonGroup>


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
        if (fontId != -1) {
           QFontDatabase::applicationFontFamilies(fontId);
        }
    }
    MainWindow mainWindow;

    QScrollArea* area = mainWindow.findChild<QScrollArea*>("scrollArea");
    MusicRenderer* musicRenderer = new MusicRenderer(area);


    QObject::connect(&mainWindow, &MainWindow::changeMusic, musicRenderer, &MusicRenderer::onChange, Qt::QueuedConnection);

    mainWindow.setStyleSheet(" .QWidget { border: 1px solid red; } ");

    mainWindow.setWindowTitle("Embedded SVG Example");
   //  mainWindow.resize(800, 1080);

    QFrame* buttonHolder = mainWindow.findChild<QFrame*>("frame_2");
    qInfo() << buttonHolder << "holder layout";
    QList<QPushButton*> allButtons = buttonHolder->findChildren<QPushButton*>();

    qInfo() << allButtons << "all buttons";
    for (int i = 0; i < allButtons.length(); i++) {
        // qInfo() << allButtons[i]->objectName();
        QObject::connect(allButtons[i], &QPushButton::clicked, &mainWindow, &MainWindow::on_selecteor_pressed,
                static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
    }

    char* fromDll  = RenderHymnSVG(1, nullptr, nullptr);
    // 1. Your raw SVG string content
    QString svgString =  QString::fromUtf8(fromDll);

    // 2. Convert to QByteArray
    QByteArray svgBytes = svgString.toUtf8();

    // 3. Load via QSvgRenderer
    QSvgRenderer* renderer = new QSvgRenderer(&app);
    musicRenderer->setRenderer(renderer);

    // Syntax: this->findChild<WidgetType*>("objectName");

    area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    area->setAlignment(Qt::AlignHCenter);

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



