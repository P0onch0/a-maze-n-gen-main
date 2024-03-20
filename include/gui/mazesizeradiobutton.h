#pragma once

#include <QRadioButton>
#include <QString>

class MazeSizeRadioButton : public QRadioButton
{
private:
    unsigned int mazeSize_ {};

public:
    explicit MazeSizeRadioButton(unsigned int mazeSize) noexcept;
    ~MazeSizeRadioButton() {};

    unsigned int getMazeSize() const;
};
