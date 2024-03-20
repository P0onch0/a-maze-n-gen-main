#include "gui/startstoppushbutton.h"

StartStopPushButton::StartStopPushButton() noexcept
    : QPushButton("Start Generation")
{
    setStyleSheet(inactiveStateColor_);
}

/*------------------------------------------------------------------------------------------------*/
void StartStopPushButton::makeStateStart()
{
    currentButtonState_ = ButtonState::Start;
    setStyleSheet(startStateColor_);
    setText("Start Generation");
}

/*------------------------------------------------------------------------------------------------*/
void StartStopPushButton::makeStateStop()
{
    currentButtonState_ = ButtonState::Stop;
    setStyleSheet(stopStateColor_);
    setText("STOP");
}

/*------------------------------------------------------------------------------------------------*/
const int& StartStopPushButton::checkCurrentButtonState() const
{
    return currentButtonState_;
}

void StartStopPushButton::LoadFromFile()
{
    currentButtonState_ = ButtonState::Start;
    setStyleSheet(stopStateColor_);
    setText("Start Load");
}
