#include "numberselector.h"

NumberSelector::NumberSelector(QLineEdit *parent)
    : QLineEdit{parent}
{
    this->currentNumber=1;
}

void NumberSelector::onChange()
{

    // Alternative: Get the widget via sender() if needed dynamically
    QLineEdit* senderWidget = qobject_cast<QLineEdit*>(sender());

    QString value = senderWidget->text();

    qInfo() << value;
    if (!(!!value.toInt())) {
        if (!value.isEmpty()) {
            value.remove(value.length() - 1, 1);
            senderWidget->setText(value);
            senderWidget->setCursorPosition(value.length());
        }
        return;
    }

    int newVal = value.toInt();
    if (newVal == this->currentNumber) {
        return ;
    }
    this->currentNumber = newVal;
    emit this->changeMusic(newVal, "", 0, false);
    this->locked = true;

    // validate

}

void NumberSelector::onSelectorChange()
{
    // Alternative: Get the widget via sender() if needed dynamically
    QPushButton* senderWidget = qobject_cast<QPushButton*>(sender());
    if (!(!!senderWidget)) {
        return;
    }

    qInfo() << senderWidget->objectName();
    this->setFocus();

    QString nameRaw = senderWidget->text();

    bool ok;
    nameRaw.toInt(&ok);

    if (ok) {
        // Conversion succeeded
        this->blockSignals(true);
        this->setText(this->text() + nameRaw);
        this->blockSignals(false);

        this->setCursorPosition(this->text().length() + nameRaw.length());
        return;
    }

    QString name = senderWidget->objectName();
    if (name.compare("btn_selector_enter") == 0) {
        // press enter
        QString value = this->text();
        int newVal = value.toInt();
        if (newVal == this->currentNumber) {
            return ;
        }
        this->currentNumber = newVal;
        // this->locked = true;
        this->clearFocus();
        emit this->changeMusic(newVal, "", 0, false);

    } else {
        this->blockSignals(true);
        this->setText("");
        this->blockSignals(false);
        this->clearFocus();
    }
}

void NumberSelector::onStepChange()
{

    QPushButton* stepButton = qobject_cast<QPushButton*>(sender());

    if (!(!!stepButton)) {
        return;
    }

    int newVal = this->currentNumber;
    qInfo() << "clicked step" << stepButton->objectName() << "val" << newVal << "raw";
    QString name = stepButton->objectName();
    (name.compare("btn_increment") == 0) ? newVal++ : newVal--;

    if (newVal == 0) {
        return;
    }
    this->setText(QString::number(newVal));
}



