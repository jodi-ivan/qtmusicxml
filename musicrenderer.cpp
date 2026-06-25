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
        QScrollArea* area = qobject_cast<QScrollArea*>(this->parent());
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

            QSvgWidget *svgWidget = new QSvgWidget();

            svgWidget->setStyleSheet("background-color:white;");
            svgWidget->renderer()->load(svgBytes);

            int internalHeight = svgWidget->renderer()->defaultSize().height();
            QSize targetSize(800, internalHeight);
            svgWidget->setFixedSize(targetSize);

            QWidget *scrollContainer = new QWidget();
            scrollContainer->setFixedSize(targetSize); // Size it exactly to match your SVG

            area->setWidget(scrollContainer);

            svgWidget->move(0, 0); // Align to top-left
            svgWidget->show();


            QGraphicsScene *scene = new QGraphicsScene(scrollContainer);
            QGraphicsView *view = new QGraphicsView(scene, scrollContainer);
            view->setStyleSheet("QGraphicsView { border: 2px solid red; }");
;
            view->scale(1.2, 1.2); // zoom
            view->centerOn(QPointF(500.0, 500.0)); // scroll


            scene->setBackgroundBrush(Qt::lightGray);

            svgWidget->setStyleSheet("border: 2px solid blue;");

            // scene->setStyle()
            view->setFixedSize(targetSize);
            scene->addWidget(svgWidget);
            svgWidget->setParent(view);
            // view->show();

            // 5. Place the Overlay Widget inside the container
            // Ensure you have defined your OverlayWidget class (from the first response)
            OverlayWidget *overlay = new OverlayWidget(scrollContainer);

            overlay->resize(targetSize); // Match the container/SVG boundaries
            overlay->raise();            // Force it to stack above the SVG
            overlay->show();

            QPushButton* button = new QPushButton(scrollContainer);
            button->setText("Zoom!");
            button->setGeometry(750, 10, 50, 25);
            button->show();
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
