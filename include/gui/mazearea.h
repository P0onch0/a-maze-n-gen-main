#pragma once

#include "algorithmgeneratormenu.h"
#include "maze.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QGroupBox>

class MazeArea : public QWidget
{
    Q_OBJECT

private:
    const unsigned int MAZE_AREA_SIZE {300};
    const unsigned int GRAPHIC_AREA_BORDER_SIZE {7};
    const unsigned int GRAPHIC_VIEW_SIZE {MAZE_AREA_SIZE + GRAPHIC_AREA_BORDER_SIZE};
    const QString MAZE_AREA_STYLE_SHEET {"QGroupBox {border-style: double;"
                                         "border-width: 3px;}"};

    Maze *maze_ {nullptr};

    QGraphicsScene *mazeScene_ {nullptr};
    QGraphicsView *mazeView_ {nullptr};

    QVBoxLayout *mazeAreaLayout_ {nullptr};
    QGroupBox *mazeAreaGroupBox_ {nullptr};

public:
    MazeArea(QWidget *parent) noexcept;
    ~MazeArea() {};

    void initializeMenu();
    void addCellOnScene(Cell& cell);

signals:
    void fieldReadyToGenerate();
    void requestToEnableAllButtons();

public slots:
    void drawMazeGrid(QVector<QVector<Cell>>& cellGrid);
    void startGenerateMazeGrid(unsigned int mazeSize);
    void startGenerationMaze(int whichAlgorithmWasChosen);
    void interruptGenerationHandling();
};
