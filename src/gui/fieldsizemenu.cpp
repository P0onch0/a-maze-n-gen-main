#include "gui/fieldsizemenu.h"

FieldSizeMenu::FieldSizeMenu(QWidget *parent) noexcept
    : BaseWidgetMenu(parent)
{
    FieldSizeMenu::initializeMenu();

    connect(sizeFirstRadio_, &MazeSizeRadioButton::toggled, this, &FieldSizeMenu::slotFirstRadio);
    connect(sizeSecondRadio_, &MazeSizeRadioButton::toggled, this, &FieldSizeMenu::slotSecondRadio);
    connect(sizeThirdRadio_, &MazeSizeRadioButton::toggled, this, &FieldSizeMenu::slotThirdRadio);
}

/*------------------------------------------------------------------------------------------------*/
void FieldSizeMenu::initializeMenu()
{
    setTextLabel("Field Size");

    sizeFirstRadio_ = new MazeSizeRadioButton(5);
    //sizeSecondRadio_ = new MazeSizeRadioButton(20);
    //sizeThirdRadio_ = new MazeSizeRadioButton(300);

    addRadioButton(sizeFirstRadio_);
    //addRadioButton(sizeSecondRadio_);
    //addRadioButton(sizeThirdRadio_);
}

/*------------------------------------------------------------------------------------------------*/
void FieldSizeMenu::setDisabledButtons(bool makeButtonsDisabled)
{
    sizeFirstRadio_->setDisabled(makeButtonsDisabled);
    //sizeSecondRadio_->setDisabled(makeButtonsDisabled);
    //sizeThirdRadio_->setDisabled(makeButtonsDisabled);
}

/*------------------------------------------------------------------------------------------------*/
void FieldSizeMenu::slotFirstRadio()
{
    emit sendMazeSize(sizeFirstRadio_->getMazeSize());
}

/*------------------------------------------------------------------------------------------------*/
void FieldSizeMenu::slotSecondRadio()
{
    emit sendMazeSize(sizeSecondRadio_->getMazeSize());
}

/*------------------------------------------------------------------------------------------------*/
void FieldSizeMenu::slotThirdRadio()
{
    emit sendMazeSize(sizeThirdRadio_->getMazeSize());
}
