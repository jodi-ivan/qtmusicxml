#include "musicrenderer.h"



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
            svgWidget->setFixedSize(800, internalHeight);

            if (area) {
                area->setWidget(svgWidget);
                this->hymnNum = num;
                this->variant = variant;
                this->verse = verse;
                this->focusMode = focusMode;
                emit this->musicRendered(num, variant, *totalVariant, *totalVerse);
            }
        }
        delete fromDll;
        // FreeRenderedString(fromDll);
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
