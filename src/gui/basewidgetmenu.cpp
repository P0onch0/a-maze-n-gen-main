#include "gui/basewidgetmenu.h"

BaseWidgetMenu::BaseWidgetMenu(QWidget *parent) noexcept
    : QWidget(parent)
{
    titleLabel_ = new QLabel;
    titleLabel_->setAlignment(Qt::AlignHCenter);
    titleLabel_->setFixedWidth(TITLE_LABEL_SIZE);


    // Передача this в QLayout делает layout верхнеуровневым, без этого не отображаются внутренности
    menuLayout_ = new QVBoxLayout(this);
    menuLayout_->addStretch();
    menuLayout_->addWidget(titleLabel_);


    menuGroupBox_ = new QGroupBox(this);
    menuGroupBox_->setStyleSheet(MENU_STYLE_SHEET);
    menuGroupBox_->setLayout(menuLayout_);

    // Запрещает делать окно размером меньше данного виджета
    setMinimumSize(menuGroupBox_->sizeHint());
}

/*------------------------------------------------------------------------------------------------*/
void BaseWidgetMenu::setTextLabel(const QString textLabel)
{
    titleLabel_->setText(textLabel);
}

/*------------------------------------------------------------------------------------------------*/
void BaseWidgetMenu::addRadioButton(QRadioButton *radioButton)
{
    menuLayout_->addWidget(radioButton);

    // Необходимо вызвать эту функцию для обновления размера окна после добавления объекта
    setMinimumSize(menuGroupBox_->sizeHint());
}

/*------------------------------------------------------------------------------------------------*/
void BaseWidgetMenu::addPushButton(QPushButton *pushButton)
{
    QVBoxLayout *radioButtonLayout = new QVBoxLayout();
    radioButtonLayout->setAlignment(Qt::AlignHCenter);
    radioButtonLayout->addWidget(pushButton);
    pushButton->setFixedWidth(PUSH_BUTTON_SIZE);

    menuLayout_->addLayout(radioButtonLayout);

    setMinimumSize(menuGroupBox_->sizeHint());
}
