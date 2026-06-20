// #include <QGuiApplication>
#include <QApplication>
#include <QMainWindow>
#include <QQmlApplicationEngine>
#include <QSvgRenderer>
#include <QByteArray>
#include <QSvgWidget>
#include <QVBoxLayout>


#include "libhymn_renderer.h"

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

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Embedded SVG Example");
    mainWindow.resize(800, 600);

    // 3. Create a layout container to hold our interface pieces
    QWidget *centralWidget = new QWidget(&mainWindow);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);


    // 1. Your raw SVG string content
    QString svgString =  QString::fromUtf8(RenderHymnSVG(1, nullptr, nullptr));

    // 2. Convert to QByteArray
    QByteArray svgBytes = svgString.toUtf8();

    // 3. Load via QSvgRenderer
    QSvgRenderer* renderer = new QSvgRenderer(&app);
    if (renderer->load(svgBytes)) {
        // Successfully loaded!

        // Example A: Use it inside a QSvgWidget
        QSvgWidget* svgWidget = new QSvgWidget();
        svgWidget->renderer()->load(svgBytes);
        layout->addWidget(svgWidget);
    }


    // 6. Tie the parent central widget framework back to the main window container
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
    mainWindow.setCentralWidget(centralWidget);

    // 7. Call show ONLY on the root window frame.
    // Children track parent visibility states dynamically and display automatically.
    mainWindow.show();

    return app.exec();
}



