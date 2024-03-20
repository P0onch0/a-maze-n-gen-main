#pragma once

#include "cell.h"
#include "coordinate.h"
#include "gui/algorithmgeneratormenu.h"

#include <QVector>
#include <QStack>
#include <QRandomGenerator>
#include <QEventLoop>
#include <QTimer>
#include <QBitArray>

struct Coordinate;

class Maze : public QObject
{
    Q_OBJECT

private:
    unsigned int mazeGridSizePx_ {};
    unsigned int mazeSize_ {};

    QVector<QVector<Cell>> cellGrid_;
    bool interruptFlag_ {false};

    enum Direction {Forbidden = -1, Top, Right, Bot, Left, Count};
    const int DELAY_MS_IN_GENERATION_CYCLE {1};

public:
    explicit Maze(unsigned int mazeGridSizePx) noexcept;
    ~Maze() {};

    QVector<QVector<Cell>>& getCellGrid();

    void generateMazeGrid(unsigned int mazeSize);
    void resetGrid();

    void interruptReceived();
    bool generationLoopExitCondition(unsigned int &visitedCells);

    void generateMaze(int whichAlgorithmWasChosen);
    void generateAldousBroder(unsigned int &visitedCells, Coordinate &currentCoordinates);
    void generateRecursiveBacktracker(unsigned int &visitedCells, Coordinate &currentCoordinates);
    void generateWilson(unsigned int &visitedCells, Coordinate &currentCoordinates);

    int checkNeighborsAndDecideWhichWayToGo(Coordinate currentCoordinates);
    void chooseRandomNonAddedCoordinates(Coordinate &currentCoordinates, QVector<Coordinate> &cellsAlreadyInMaze);
    void makeStepBack(Coordinate currentCoordinates, Coordinate previousCoordinates, bool isWallsNeedToRebuild);

    bool isLegitimateStep(Coordinate coordinate, int stepDirection);
    void makeStep(Coordinate &currentCoordinates, int stepDirection, unsigned int &visitedCellsCounter);
    void markCellAfterStep(Coordinate currentCoordinates, Coordinate newCoordinates);

    void goTop(Coordinate &currentCoordinates, Coordinate &newCoordinates, bool &cellWasVisitedOnThisStep);
    void goRight(Coordinate &currentCoordinates, Coordinate &newCoordinates, bool &cellWasVisitedOnThisStep);
    void goBot(Coordinate &currentCoordinates, Coordinate &newCoordinates, bool &cellWasVisitedOnThisStep);
    void goLeft(Coordinate &currentCoordinates, Coordinate &newCoordinates, bool &cellWasVisitedOnThisStep);
    void loadMazeFromFile(const std::string& filePath);
    void loadFromFile();
    void saveToFile();
    // https://stackoverflow.com/questions/3752742/how-do-i-create-a-pause-wait-function-using-qt/43003223#43003223
    void delay(int millisecondsWait);

signals:
    void requestToDrawMazeGrid(QVector<QVector<Cell>>& cellGrid);
    void mazeWasGenerated();
};



