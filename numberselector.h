#ifndef NUMBERSELECTOR_H
#define NUMBERSELECTOR_H

#include <QLineEdit>
#include <QPushButton>

class NumberSelector : public QLineEdit
{
    Q_OBJECT

    int currentNumber;
    bool locked;

public:
    explicit NumberSelector(QLineEdit *parent = nullptr);

signals:
    bool changeMusic(int num, std::string variant, int verse, bool focusMode);

public slots:
    void onChange();
    void onSelectorChange();
    void onStepChange();
};

#endif // NUMBERSELECTOR_H
