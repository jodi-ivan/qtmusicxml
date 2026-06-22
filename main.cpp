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
#include <QGroupBox>

#include "libhymn_renderer.h"
#include "mainwindow.h"
#include "musicrenderer.h"
#include "numberselector.h"
#include "verseselector.h"

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

    QGroupBox* verseGroupBox = mainWindow.findChild<QGroupBox*>("verseGroupBox");
    VerseSelector* verseSelector = new VerseSelector(&mainWindow, verseGroupBox);

    QScrollArea* area = mainWindow.findChild<QScrollArea*>("scrollArea");
    MusicRenderer* musicRenderer = new MusicRenderer(area);

    QObject::connect(musicRenderer, &MusicRenderer::musicRendered,
                     &mainWindow, &MainWindow::onMusicRendered , // for the variant selector
                     static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection) );

    QObject::connect(musicRenderer, &MusicRenderer::musicRendered,
                     verseSelector, &VerseSelector::onMusicRendered , // for verse selector
                     static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection) );


    QObject::connect(&mainWindow, &MainWindow::VariantSelected,
                     musicRenderer, &MusicRenderer::onVariantChange,
                     static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection) );

    QObject::connect(verseSelector,
                     &VerseSelector::verseSelected,
                     musicRenderer,
                     &MusicRenderer::onVerseChange,
                     static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));

    QPushButton* decrementBtn = mainWindow.findChild<QPushButton*>("btn_decrement");
    QPushButton* incrementBtn = mainWindow.findChild<QPushButton*>("btn_increment");

    mainWindow.setStyleSheet(" .QWidget { border: 1px solid red; } ");

    mainWindow.setWindowTitle("Embedded SVG Example");
    //  mainWindow.resize(800, 1080);


    QFrame* numSelectorFrame = mainWindow.findChild<QFrame*>("num_selector_frame");
    if (numSelectorFrame) {
        NumberSelector* numSelector = new NumberSelector(new QLineEdit(numSelectorFrame));
        numSelector->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        numSelector->setText("1");
        numSelector->setMinimumHeight(27);
        numSelector->setMinimumWidth(60);
        numSelector->setMaxLength(3);

        numSelector->setAlignment(Qt::AlignVCenter);

        QFrame* buttonHolder = mainWindow.findChild<QFrame*>("frame_2");
        QList<QPushButton*> allButtons = buttonHolder->findChildren<QPushButton*>();

        for (int i = 0; i < allButtons.length(); i++) {
            QObject::connect(allButtons[i], &QPushButton::clicked, numSelector, &NumberSelector::onSelectorChange,
                             static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
        }


        QObject::connect(numSelector, &QLineEdit::textChanged,  numSelector, &NumberSelector::onChange,  static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
        QObject::connect(numSelector, &NumberSelector::changeMusic, musicRenderer, &MusicRenderer::onChange, Qt::QueuedConnection);
        if (!!incrementBtn) {
            QObject::connect(incrementBtn, &QPushButton::clicked, numSelector, &NumberSelector::onStepChange,  static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
        }

        if (!!decrementBtn) {
            QObject::connect(decrementBtn, &QPushButton::clicked, numSelector, &NumberSelector::onStepChange,  static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
        }
    }


    QSvgRenderer* renderer = new QSvgRenderer(&app);
    musicRenderer->setRenderer(renderer);

    area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    area->setAlignment(Qt::AlignHCenter);


    musicRenderer->onChange(1, "", 0, false);
    mainWindow.show();

    return app.exec();
}



