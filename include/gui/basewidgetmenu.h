#pragma once

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>

class BaseWidgetMenu : public QWidget
{
    Q_OBJECT

private:
    const unsigned int TITLE_LABEL_SIZE {250};
    const unsigned int PUSH_BUTTON_SIZE {150};
    const QString MENU_STYLE_SHEET {
        "QGroupBox {border-style: double;"
                   "border-width: 3px;}"
        "QLabel    {border-bottom-width: 1px;"
                   "border-bottom-style: solid;"
                   "border-radius: 0px;"
                   "font-size: 16pt;}"
        "*         {font-family: Centaur;"
                   "font-size: 14pt;}"};


    QLabel *titleLabel_ {nullptr};
    QVBoxLayout *menuLayout_ {nullptr};
    QGroupBox *menuGroupBox_ {nullptr};

public:
    BaseWidgetMenu(QWidget *parent = nullptr) noexcept;
    virtual ~BaseWidgetMenu() {};

    virtual void initializeMenu() = 0;

    void setTextLabel(const QString textLabel);
    void addRadioButton(QRadioButton *radioButton);
    void addPushButton(QPushButton *pushButton);

    virtual void setDisabledButtons(bool makeButtonsDisabled) = 0;
};
