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
#include <QTextEdit>
#include <QStyleFactory>
#include <QStyleHints>

#include "libhymn_renderer.h"
#include "mainwindow.h"
#include "musicrenderer.h"
#include "numberselector.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QGuiApplication::styleHints()->setColorScheme(Qt::ColorScheme::Light);
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

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

    QPushButton* decrementBtn = mainWindow.findChild<QPushButton*>("btn_decrement");
    QPushButton* incrementBtn = mainWindow.findChild<QPushButton*>("btn_increment");

    mainWindow.setStyleSheet(" .QWidget { border: 1px solid red; } ");

    mainWindow.setWindowTitle("Embedded SVG Example");
   //  mainWindow.resize(800, 1080);


    QFrame* numSelectorFrame = mainWindow.findChild<QFrame*>("num_selector_frame");
    if (numSelectorFrame) {
        NumberSelector* numSelector = new NumberSelector(new QTextEdit(numSelectorFrame));
        numSelector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        numSelector->setText("1");
        numSelector->setLineWidth(1);

        QFrame* buttonHolder = mainWindow.findChild<QFrame*>("frame_2");
        QList<QPushButton*> allButtons = buttonHolder->findChildren<QPushButton*>();

        qInfo() << allButtons << "all buttons";
        for (int i = 0; i < allButtons.length(); i++) {
            // qInfo() << allButtons[i]->objectName();
            QObject::connect(allButtons[i], &QPushButton::clicked, numSelector, &NumberSelector::onSelectorChange,
                             static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
        }


        QObject::connect(numSelector, &QTextEdit::textChanged,  numSelector, &NumberSelector::onChange,  static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
        QObject::connect(numSelector, &NumberSelector::changeMusic, musicRenderer, &MusicRenderer::onChange, Qt::QueuedConnection);
        if (!!incrementBtn) {
            qInfo() << "incrementBtn" << incrementBtn;
            QObject::connect(incrementBtn, &QPushButton::clicked, numSelector, &NumberSelector::onStepChange,  static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
        }

        if (!!decrementBtn) {
            qInfo() << "decrementBtn" << decrementBtn;
            QObject::connect(decrementBtn, &QPushButton::clicked, numSelector, &NumberSelector::onStepChange,  static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
        }
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



