#ifndef VERSESELECTOR_H
#define VERSESELECTOR_H

#include <QCheckBox>
#include <QGroupBox>
#include <QObject>
#include <QScrollBar>
#include <QScrollArea>
#include <QPushButton>

class VerseSelector : public QObject
{
    Q_OBJECT

    QCheckBox* focusModeCheckbox;
    QScrollArea* buttonContainers;
    QGroupBox* verseGroupBox;
    QPushButton* prevBtn; QPushButton* nextBtn;
    int scrollCounter = 0;
    int totalVerse = 0;
    int selectedVerse = 1;
    int currentHymn = 0;

public:
    explicit VerseSelector(QObject *parent = nullptr,  QGroupBox* container = nullptr);


public slots:
    void onScrollClicked();
    void onMusicRendered(int numbered, std::string variant, int totalVariant, int totalVerse);
    void onVerseSelectorClicked();

signals:
    void verseSelected(int selected, bool focusMode);
};

#endif // VERSESELECTOR_H
