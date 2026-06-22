#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->variantFrame =this->findChild<QFrame*>("variant_selector_frame");
    this->variantFrame->hide();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onMusicRendered(int numbered, std::string variant, int totalVariant, int totalVerse) {
    std::string variantLabel[3] = {"a", "b", "c"};
    qDeleteAll(this->variantFrame->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    if (this->variantFrame->layout() != nullptr) { delete this->variantFrame->layout(); }
    if (totalVariant == 0) {
        this->variantFrame->hide();
        return;
    }

    QGridLayout* layout = new QGridLayout(this->variantFrame);
    layout->setAlignment(Qt::AlignCenter);
    for (int i = 1; i <= totalVariant; i++) {
        QPushButton* pb = new QPushButton(this->variantFrame);
        pb->setText(QString::fromStdString(variantLabel[i-1]));
        pb->setObjectName("variant_"+variantLabel[i-1]);

        QSizePolicy policy = pb->sizePolicy();
        policy.setHorizontalPolicy(QSizePolicy::Expanding);
        policy.setVerticalPolicy(QSizePolicy::Expanding);        // Or setVerticalPolicy()
        pb->setSizePolicy(policy);
        //pb->setMaximumSize(28, 28);
        layout->addWidget(pb, 1, i, Qt::AlignCenter);
        if (variant == "") { variant = "a"; }
        if (variantLabel[i-1] == variant) { pb->setEnabled(false); }

        pb->show();

        QObject::connect(
            pb, &QPushButton::clicked,
            this, &MainWindow::onVariantClicked,
            static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection)
            );
    }
    this->variantFrame->show();

}

void MainWindow::onVariantClicked()
{
    QPushButton* pb = qobject_cast<QPushButton*>(sender());
    emit this->VariantSelected(pb->text().toStdString());
}
