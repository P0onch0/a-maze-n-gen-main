#include "gui/mazesizeradiobutton.h"

MazeSizeRadioButton::MazeSizeRadioButton(unsigned int mazeSize) noexcept
    : QRadioButton(QString::number(mazeSize) + "x" + QString::number(mazeSize)),
      mazeSize_(mazeSize)
{
}

/*------------------------------------------------------------------------------------------------*/
unsigned int MazeSizeRadioButton::getMazeSize() const
{
    return mazeSize_;
}
