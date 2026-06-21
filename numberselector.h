#ifndef NUMBERSELECTOR_H
#define NUMBERSELECTOR_H

#include <QTextEdit>
#include <QPushButton>

class NumberSelector : public QTextEdit
{
    Q_OBJECT

    int currentNumber;
    bool locked;

public:
    explicit NumberSelector(QTextEdit *parent = nullptr);

signals:
    bool changeMusic(int num, std::string variant, int verse, bool focusMode);

public slots:
    void onChange();
    void onSelectorChange();
    void onStepChange();
};

#endif // NUMBERSELECTOR_H
