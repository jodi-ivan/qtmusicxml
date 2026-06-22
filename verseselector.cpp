#include "verseselector.h"

#include <QVBoxLayout>

VerseSelector::VerseSelector(QObject *parent, QGroupBox* container)
    : QObject{parent}
{

    if (!!container) {
        this->verseGroupBox = container;
        this->verseGroupBox->hide();
        this->buttonContainers = container->findChild<QScrollArea*>("verse_selector_area");
        this->buttonContainers->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->buttonContainers->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QPushButton* nextScroll = container->findChild<QPushButton*>("btn_next_scroll");
        QPushButton* prevScroll = container->findChild<QPushButton*>("btn_prev_scroll");
        QObject::connect(nextScroll,
                         &QPushButton::clicked,
                         this,
                         &VerseSelector::onScrollClicked,
                         static_cast<Qt::ConnectionType>(Qt::QueuedConnection
                                                         | Qt::UniqueConnection));
        QObject::connect(prevScroll,
                         &QPushButton::clicked,
                         this,
                         &VerseSelector::onScrollClicked,
                         static_cast<Qt::ConnectionType>(Qt::QueuedConnection
                                                         | Qt::UniqueConnection));
    }
}

void VerseSelector::onScrollClicked()
{
    QPushButton* senderWidget = qobject_cast<QPushButton*>(sender());
    if (!(!!senderWidget)) {
        return;
    }

    QString name = senderWidget->objectName();
    bool isIncrement = name.compare("btn_next_scroll") == 0;


    // Instead of counter-based, track pixel offset directly
    (isIncrement) ? this->scrollCounter += 90 : this->scrollCounter -= 90;

    int maxOffset = (this->totalVerse * 30) - 150; // total content width - visible width
    this->scrollCounter = std::max(0, std::min(this->scrollCounter, maxOffset));

    this->buttonContainers->horizontalScrollBar()->setValue(this->scrollCounter);

}

void VerseSelector::onMusicRendered(int numbered, std::string variant,int totalVariant,int totalVerse)
{
    if (totalVerse == 0) {
        this->verseGroupBox->hide();
        return;
    }

    if (this->currentHymn == numbered) { return; }

    this->currentHymn = numbered;

    QWidget *oldWidget = this->buttonContainers->takeWidget();
    if (oldWidget) { oldWidget->deleteLater(); }

    QWidget *containerWidget = new QWidget();
    containerWidget->setFixedHeight(30);

    QHBoxLayout *layout = new QHBoxLayout(containerWidget);
    layout->setContentsMargins(0, 0, 0, 0); // Removes outer padding/margins
    layout->setSpacing(3);
    layout->setAlignment(Qt::AlignLeft);

    for (int i = 1; i <= totalVerse+1; i++) {
        QPushButton* pb = new QPushButton();
        pb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        pb->setFixedSize(28, 28);
        pb->setMinimumSize(28,28);

        pb->setText(QString::number(i));
        pb->show();
        QObject::connect(pb,&QPushButton::clicked,
                         this, &VerseSelector::onVerseSelectorClicked,
                         static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
        layout->addWidget(pb);
    }

    this->buttonContainers->setWidget(containerWidget);
    this->buttonContainers->setWidgetResizable(true);
    this->totalVerse = totalVerse;
    this->verseGroupBox->show();
}

void VerseSelector::onVerseSelectorClicked()
{
    QPushButton *senderWidget = qobject_cast<QPushButton *>(sender());
    if (!(!!senderWidget)) { return; }

    bool ok; int selected = senderWidget->text().toInt(&ok);
    if (!ok) { return; }

    this->selectedVerse = selected;

    emit this->verseSelected(selected, false);
}
