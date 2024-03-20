#pragma once

#include <QPushButton>
#include <QString>

class StartStopPushButton : public QPushButton
{
private:
    QString inactiveStateColor_ {"background-color: rgba(0, 0, 0, 25%)"};
    QString startStateColor_ {"background-color: rgba(0, 255, 0, 50%)"};
    QString stopStateColor_ {"background-color: rgba(255, 0, 0, 50%)"};

    int currentButtonState_ {-1};

public:
    explicit StartStopPushButton() noexcept;
    ~StartStopPushButton() {};

    void makeStateStart();
    void makeStateStop();
    void LoadFromFile();

    const int& checkCurrentButtonState() const;
    enum ButtonState {Start, Stop};
};
