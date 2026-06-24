#include "overlaywidget.h"

OverlayWidget::OverlayWidget(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_TranslucentBackground, true);

    // OPTIONAL: Ignore mouse clicks so they pass through to the background
    // setAttribute(Qt::WA_TransparentForMouseEvents, true);

    if (parent) {
        parent->installEventFilter(this);
        raise(); // Keep it on top of other sibling widgets
    }
}

// bool OverlayWidget::eventFilter(QObject *obj, QEvent *event)
// {
//     if (obj == parent() && event->type() == QEvent::Resize) {
//         resize(static_cast<QWidget*>(parent())->size());
//     }
//     return QWidget::eventFilter(obj, event);
// }

void OverlayWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // this->setStyleSheet("border: ")

    // Draw a semi-transparent dark dimming layer
    // painter.fillRect(rect(), QColor(0, 0, 80, 65)); // 127 = 50% opacity
    QColor color(255, 0, 0, 77);

    // Draw the filled rectangle
    painter.fillRect(131, 110, 150, 110, color);

    // Draw overlay elements (e.g., a white message box or loading indicator)
    painter.setPen(Qt::white);
    painter.drawText(rect(), Qt::AlignCenter, "Loading Data...");
}
