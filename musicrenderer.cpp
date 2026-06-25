#include "musicrenderer.h"

#include "overlaywidget.h"



MusicRenderer::MusicRenderer(QObject *parent)
    : QObject{parent}
{

}

void MusicRenderer::setRenderer(QSvgRenderer *renderer)
{
    this->svgRenderer = renderer;
}

void MusicRenderer::onChange(int num, std::string variant, int verse, bool focusMode)
{

    try {
        QFrame* area = qobject_cast<QFrame*>(this->parent());
        if (!(!!area)) {
            return;
        }
        int* totalVariant = new int(0);
        int* totalVerse = new int(0);
        char* variantPtr = nullptr;
        if (variant != "") {
            variantPtr = variant.data();
        }

        char *config = nullptr;
        if (verse > 0) {
            std::string cnfStr = "{\"verse\": " + std::to_string(verse) + "}";
            config = cnfStr.data();
        }
        char *fromDll = RenderHymnSVGWithInfo(num, variantPtr, config, totalVerse, totalVariant);
        QString svgString = QString::fromUtf8(fromDll);

        // 2. Convert to QByteArray
        QByteArray svgBytes = svgString.toUtf8();

        if (this->svgRenderer->load(svgBytes)) {

            QWidget *containerArea = new QWidget(area);

            QSvgWidget *svgWidget = new QSvgWidget();
            svgWidget->renderer()->load(svgBytes);
            svgWidget->setStyleSheet("background-color: white; border: 2px solid blue;");

            int internalHeight = svgWidget->renderer()->defaultSize().height();
            QSize targetSize(800, internalHeight);
            svgWidget->setFixedSize(targetSize);

            QGraphicsScene *scene = new QGraphicsScene(containerArea);
            scene->setBackgroundBrush(Qt::lightGray);
            scene->setSceneRect(0, 0, targetSize.width(), targetSize.height());

            QGraphicsView *view = new QGraphicsView(scene, containerArea);
            view->setStyleSheet("QGraphicsView { border: 2px solid red; }");

            QSize containerSize(800, 600);
            view->setFixedSize(containerSize);
            view->setMinimumSize(800, 600);
            view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

            // Do NOT call svgWidget->setParent(view) after this!
            QGraphicsProxyWidget *proxy = scene->addWidget(svgWidget);
            proxy->setPos(0, 0);

            view->scale(0.7, 0.7);
            view->centerOn(QPointF(500.0, 500.0));

            // 3. Fix the Overlay
            // If the overlay needs to zoom/scroll WITH the SVG, add it to the scene too:
            OverlayWidget *overlay = new OverlayWidget();
            overlay->resize(targetSize);

            // what is this magic that allows the overlay to be stick and same relavtive with the svg even the view is scaled.
            QGraphicsProxyWidget *overlayProxy = scene->addWidget(overlay);
            overlayProxy->setPos(0, 0);

            overlayProxy->setZValue(1); // Forces it to stack on top of the SVG proxy
            QPushButton* button = new QPushButton(containerArea);
            button->setText("Zoom!");
            button->setGeometry(750, 10, 50, 25);
            button->show();


            area->show();
            containerArea->show();
            this->hymnNum = num;
            this->variant = variant;
            this->verse = verse;
            this->focusMode = focusMode;
            emit this->musicRendered(num, variant, *totalVariant, *totalVerse);

        }
        // FreeRenderedString(fromDll);
        delete fromDll;

        // Clean up your dynamic pointers if they aren't managed elsewhere
        delete totalVariant;
        delete totalVerse;
    } catch (const std::exception &e) {
        qInfo() << "failed" << e.what();
    }
}

void MusicRenderer::onVariantChange(std::string selected)
{
    this->onChange(this->hymnNum, selected, this->verse, this->focusMode);
}

void MusicRenderer::onVerseChange(int selected, bool focusMode)
{
    this->verse = selected;
    this->focusMode = focusMode;
    this->onChange(this->hymnNum, this->variant, selected, focusMode);
}
