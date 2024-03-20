#pragma once

#include "algorithmgeneratormenu.h"
#include "fieldsizemenu.h"
#include "mazearea.h"


#include <QWidget>
#include <QLayout>

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    FieldSizeMenu *fieldSizeWidget_ {nullptr};
    AlgorithmGeneratorMenu *algorithmGeneratorWidget_ {nullptr};
    QVBoxLayout *sidebarLayout_ {nullptr};

    MazeArea *mazeGrid_ {nullptr};

    QHBoxLayout *mainLayout_ {nullptr};


public:
    MainWindow(QWidget *parent = nullptr) noexcept;
    virtual ~MainWindow() {};

    void initializeMazeSettingsMenu();
    void initializeMazeArea();
    void initializeMainWindow();

public slots:
    void setDisabledAllButtons();

};
