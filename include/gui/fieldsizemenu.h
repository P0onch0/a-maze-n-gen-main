#pragma once

#include "basewidgetmenu.h"
#include "mazesizeradiobutton.h"

class FieldSizeMenu : public BaseWidgetMenu
{
    Q_OBJECT

private:
    /* Вероятно, следует сделать кнопки и ассоциированные с ними слоты с помощью векторов
     * и указателей на функции (если возможно таким образом создать слоты), ибо сейчас выглядит
     * не очень. Возможно вообще переделать на вводимый юзером размер клеток */
    MazeSizeRadioButton *sizeFirstRadio_ {nullptr};
    MazeSizeRadioButton *sizeSecondRadio_ {nullptr};
    MazeSizeRadioButton *sizeThirdRadio_ {nullptr};

public:
    FieldSizeMenu(QWidget *parent = nullptr) noexcept;
    ~FieldSizeMenu() {};

    void initializeMenu();
    void setDisabledButtons(bool makeButtonsDisabled);

signals:
    void sendMazeSize(unsigned int mazeSize);

private slots:
    void slotFirstRadio();
    void slotSecondRadio();
    void slotThirdRadio();
};
