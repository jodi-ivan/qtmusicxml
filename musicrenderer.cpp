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
    qInfo() << "changed" << num;
     try {
        QScrollArea* area = qobject_cast<QScrollArea*>(this->parent());
          if (!(!!area)) {
            return;
       }

        char* fromDll = RenderHymnSVG(num, nullptr, nullptr);
        QString svgString =  QString::fromUtf8(fromDll);

        // 2. Convert to QByteArray
        QByteArray svgBytes = svgString.toUtf8();

        if (this->svgRenderer->load(svgBytes)) {
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
    } catch (const std::exception& e) {
        qInfo() << "failed" << e.what();
    }

}
