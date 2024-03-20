#include "maze.h"
#include "cell.h"
#include <fstream>
#include <vector>
#include <QDataStream>
#include <QFile>
#include <iostream>
#include <QDebug> // Assurez-vous que cette en-tête est incluse pour qDebug()

Maze::Maze(unsigned int mazeGridSizePx) noexcept
    : mazeGridSizePx_(mazeGridSizePx)
{
}

/*------------------------------------------------------------------------------------------------*/
QVector<QVector<Cell>>& Maze::getCellGrid()
{
    return cellGrid_;
}

/*------------------------------------------------------------------------------------------------*/
void Maze::generateMazeGrid(unsigned int mazeSize) {
    cellGrid_.clear();
    mazeSize_ = 5; // Ou utilisez mazeSize pour une taille dynamique.
    unsigned int cellSize = mazeGridSizePx_ / mazeSize_;

    std::vector<char> mazeData; // Pour stocker les données du labyrinthe.

    for (unsigned int col = 0; col < mazeSize_; ++col) {
        QVector<Cell> curColCells;
        for (unsigned int row = 0; row < mazeSize_; ++row) {
            Cell cell(cellSize, row, col);
            curColCells.push_back(cell);

            // Convertir chaque cellule en bytes et les ajouter à mazeData.
            auto cellBytes = cell.toBytes();
            mazeData.insert(mazeData.end(), cellBytes.begin(), cellBytes.end());
        }
        cellGrid_.push_back(curColCells);
    }

    emit requestToDrawMazeGrid(getCellGrid());
}

/*------------------------------------------------------------------------------------------------*/
void Maze::resetGrid()
{
    for (auto row = cellGrid_.begin(); row != cellGrid_.end(); row++)
    {
        for (auto col = row->begin(); col != row->end(); col++)
        {
            col->resetCell();
        }
    }
}

/*------------------------------------------------------------------------------------------------*/
void Maze::interruptReceived()
{
    interruptFlag_ = true;
}

/*------------------------------------------------------------------------------------------------*/
bool Maze::generationLoopExitCondition(unsigned int &visitedCells)
{
    return (!interruptFlag_ && visitedCells < mazeSize_ * mazeSize_);
}

/*------------------------------------------------------------------------------------------------*/
void Maze::generateMaze(int whichAlgorithmWasChosen)
{
    Coordinate currentCoordinates {0, 0};
    unsigned int visitedCells {1};
    cellGrid_[0][0].wasVisited();
    cellGrid_[0][0].getRectForShowCurrentCell()->setVisible(true);
    delay(DELAY_MS_IN_GENERATION_CYCLE);

    switch (whichAlgorithmWasChosen)
    {
    case AlgorithmGeneratorMenu::Algorithm::AldousBroder :
        generateAldousBroder(visitedCells, currentCoordinates);
        break;
    case AlgorithmGeneratorMenu::Algorithm::RecursiveBacktracker :
        generateRecursiveBacktracker(visitedCells, currentCoordinates);
        break;
    case AlgorithmGeneratorMenu::Algorithm::Wilson :
        generateWilson(visitedCells, currentCoordinates);
        break;
    }

    cellGrid_[currentCoordinates.x][currentCoordinates.y].getRectForShowCurrentCell()->setVisible(false);
    interruptFlag_ = false;
    emit mazeWasGenerated();
}

/*------------------------------------------------------------------------------------------------*/
void Maze::generateAldousBroder(unsigned int &visitedCells, Coordinate &currentCoordinates)
{
    while (generationLoopExitCondition(visitedCells))
    {
        int whichWayToGo = QRandomGenerator::global()->generate() % Direction::Count;
        if (isLegitimateStep(currentCoordinates, whichWayToGo))
            makeStep(currentCoordinates, whichWayToGo, visitedCells);
    }
}

/*------------------------------------------------------------------------------------------------*/
void Maze::generateRecursiveBacktracker(unsigned int &visitedCells, Coordinate &currentCoordinates)
{
    QStack<Coordinate> backtrackingStack {};
    backtrackingStack.push(currentCoordinates);

    std::vector<char> mazeBytes; // Vecteur pour stocker les bytes du labyrinthe.

    while (generationLoopExitCondition(visitedCells))
    {
        if (backtrackingStack.isEmpty()) {
            qDebug() << "Erreur: backtrackingStack est vide!";
            break; // Sortir de la boucle pour éviter un crash.
        }

        Coordinate currentCell = backtrackingStack.top();
        int whichWayToGo = checkNeighborsAndDecideWhichWayToGo(currentCell);

        if (whichWayToGo != Direction::Forbidden)
        {
            makeStep(currentCell, whichWayToGo, visitedCells);
            currentCoordinates = currentCell; // Mise à jour après le déplacement.
            backtrackingStack.push(currentCoordinates);

            auto cellBytes = cellGrid_[currentCoordinates.x][currentCoordinates.y].toBytes();
            qDebug() << "Taille de cellBytes pour la cellule (" << currentCoordinates.x << "," << currentCoordinates.y << ") :" << cellBytes.size();

            if (!cellBytes.empty()) {
                mazeBytes.insert(mazeBytes.end(), cellBytes.begin(), cellBytes.end());
            } else {
                qDebug() << "Erreur: cellBytes est vide pour la cellule (" << currentCoordinates.x << "," << currentCoordinates.y << ").";
            }

            qDebug() << "Taille actuelle de mazeBytes :" << mazeBytes.size();
        }
        else
        {
            cellGrid_[currentCell.x][currentCell.y].getRectForShowCurrentCell()->setVisible(false);
            backtrackingStack.pop();
        }
    }

    if (!mazeBytes.empty()) {
        std::ofstream outFile("mazeData2.bin", std::ios::binary);
        if (outFile.is_open())
        {
            outFile.write(reinterpret_cast<const char*>(mazeBytes.data()), mazeBytes.size());
            outFile.close();
            qDebug() << "Le fichier mazeData2.bin a été écrit avec succès.";
        }
        else
        {
            qDebug() << "Erreur: Impossible d'ouvrir le fichier pour écrire!";
        }
    } else {
        qDebug() << "Erreur: mazeBytes est vide, rien à écrire dans le fichier.";
    }
}

/*------------------------------------------------------------------------------------------------*/
void Maze::generateWilson(unsigned int &visitedCells, Coordinate &currentCoordinates)
{
    QStack<Coordinate> currentPathStack {};
    QVector<Coordinate> cellsAlreadyInMaze {};
    cellsAlreadyInMaze.push_back(currentCoordinates);
    cellGrid_[0][0].getRectForShowCurrentCell()->setVisible(false);

    while (generationLoopExitCondition(visitedCells))
    {
        chooseRandomNonAddedCoordinates(currentCoordinates, cellsAlreadyInMaze);
        currentPathStack.push(currentCoordinates);

        /* Данный цикл строит ветку лабиринта из случайной клетки до включенных в лабиринт клеток.
         * Цикл может длиться очень долго и необходимо иметь возможность его прервать,
         * поэтому здесь также отслеживаем флаг прерывания */
        while (!cellsAlreadyInMaze.contains(currentCoordinates) && !interruptFlag_)
        {
            int whichWayToGo = QRandomGenerator::global()->generate() % Direction::Count;
            if (isLegitimateStep(currentCoordinates, whichWayToGo))
            {
                makeStep(currentCoordinates, whichWayToGo, visitedCells);
                if (!currentPathStack.contains(currentCoordinates))
                {
                    currentPathStack.push(currentCoordinates);
                }
                else
                {
                    while (currentCoordinates != currentPathStack.top())
                    {
                        Coordinate savedCoordinates = currentPathStack.top();
                        currentPathStack.pop();
                        makeStepBack(savedCoordinates, currentPathStack.top(), true);
                    }
                }
            }
        }

        /* Во избежание краша, вызванного нажатием стоп при пустом стеке (pop-аем пустой стек), что
         * возможно, если в текущий путь не успела добавиться ни одна ячейка*/
        if (currentPathStack.size() != 0)
        {
            cellGrid_[currentCoordinates.x][currentCoordinates.y].getRectForShowCurrentCell()->setVisible(false);
            currentPathStack.pop();
            // Для корректной отрисовки необходимо вызвать данную функцию
            makeStepBack(currentCoordinates, currentPathStack.top(), false);

            // Добавляем ячейки из пути в основной лабиринт
            while (currentPathStack.size() != 0)
            {
                cellsAlreadyInMaze.push_back(currentPathStack.top());
                currentPathStack.pop();
            }
            currentPathStack.clear();
        }
        visitedCells = cellsAlreadyInMaze.size();
    }
}

/*------------------------------------------------------------------------------------------------*/
int Maze::checkNeighborsAndDecideWhichWayToGo(Coordinate currentCoordinates)
{
    QBitArray cellNeighborsState(Direction::Count);

    if (isLegitimateStep(currentCoordinates, Direction::Top) &&
            !cellGrid_[currentCoordinates.x][currentCoordinates.y - 1].isVisited())
        cellNeighborsState.setBit(Direction::Top, true);

    if (isLegitimateStep(currentCoordinates, Direction::Right) &&
            !cellGrid_[currentCoordinates.x + 1][currentCoordinates.y].isVisited())
        cellNeighborsState.setBit(Direction::Right, true);

    if (isLegitimateStep(currentCoordinates, Direction::Bot) &&
            !cellGrid_[currentCoordinates.x][currentCoordinates.y + 1].isVisited())
        cellNeighborsState.setBit(Direction::Bot, true);

    if (isLegitimateStep(currentCoordinates, Direction::Left) &&
            !cellGrid_[currentCoordinates.x - 1][currentCoordinates.y].isVisited())
        cellNeighborsState.setBit(Direction::Left, true);


    if (cellNeighborsState == QBitArray(Direction::Count, false))
        return Direction::Forbidden;
    else
    {
        int whichWayToGo = QRandomGenerator::global()->generate() % Direction::Count;
        while (cellNeighborsState[whichWayToGo] != true)
            whichWayToGo = QRandomGenerator::global()->generate() % Direction::Count;
        return whichWayToGo;
    }
}

/*------------------------------------------------------------------------------------------------*/
void Maze::chooseRandomNonAddedCoordinates(Coordinate &currentCoordinates, QVector<Coordinate> &cellsAlreadyInMaze)
{
    currentCoordinates.x = 0;
    currentCoordinates.y = 0;

    while (cellsAlreadyInMaze.contains(currentCoordinates))
    {
        currentCoordinates = Coordinate(QRandomGenerator::global()->generate() % mazeSize_,
                                        QRandomGenerator::global()->generate() % mazeSize_);
    }

    cellGrid_[currentCoordinates.x][currentCoordinates.y].
            getRectForShowCurrentCell()->setVisible(true);
    cellGrid_[currentCoordinates.x][currentCoordinates.y].wasVisited();
}

/*------------------------------------------------------------------------------------------------*/
void Maze::makeStepBack(Coordinate currentCoordinates, Coordinate previousCoordinates, bool isWallsNeedToRebuild)
{
    cellGrid_[currentCoordinates.x][currentCoordinates.y].setUnvisited();

    Coordinate difference = Coordinate(currentCoordinates.x - previousCoordinates.x,
                                       currentCoordinates.y - previousCoordinates.y);
    if (difference.x == 1)
    {
        cellGrid_[currentCoordinates.x][currentCoordinates.y].getLeftWall()->setVisible(isWallsNeedToRebuild);
        cellGrid_[previousCoordinates.x][previousCoordinates.y].getRightWall()->setVisible(isWallsNeedToRebuild);
    }
    if (difference.x == -1)
    {
        cellGrid_[currentCoordinates.x][currentCoordinates.y].getRightWall()->setVisible(isWallsNeedToRebuild);
        cellGrid_[previousCoordinates.x][previousCoordinates.y].getLeftWall()->setVisible(isWallsNeedToRebuild);
    }
    if (difference.y == 1)
    {
        cellGrid_[currentCoordinates.x][currentCoordinates.y].getTopWall()->setVisible(isWallsNeedToRebuild);
        cellGrid_[previousCoordinates.x][previousCoordinates.y].getBotWall()->setVisible(isWallsNeedToRebuild);
    }
    if (difference.y == -1)
    {
        cellGrid_[currentCoordinates.x][currentCoordinates.y].getBotWall()->setVisible(isWallsNeedToRebuild);
        cellGrid_[previousCoordinates.x][previousCoordinates.y].getTopWall()->setVisible(isWallsNeedToRebuild);
    }
}

/*------------------------------------------------------------------------------------------------*/
bool Maze::isLegitimateStep(Coordinate coordinate, int stepDirection)
{
    bool isLegitimateStep {false};
    switch (stepDirection)
    {
    case Direction::Top :
        coordinate.y <= 0 ? isLegitimateStep = false : isLegitimateStep = true;
        break;
    case Direction::Right :
        coordinate.x >= static_cast<int> (mazeSize_ - 1) ? isLegitimateStep = false : isLegitimateStep = true;
        break;
    case Direction::Bot :
        coordinate.y >= static_cast<int> (mazeSize_ - 1) ? isLegitimateStep = false : isLegitimateStep = true;
        break;
    case Direction::Left :
        coordinate.x <= 0 ? isLegitimateStep = false : isLegitimateStep = true;
        break;
    }
    return isLegitimateStep;
}

/*------------------------------------------------------------------------------------------------*/
void Maze::makeStep(Coordinate &currentCoordinates, int stepDirection, unsigned int &visitedCellsCounter)
{
    bool cellWasVisitedOnThisStep {false};
    Coordinate newCoordinates = currentCoordinates;

    switch (stepDirection)
    {
    case Direction::Top :
        goTop(currentCoordinates, newCoordinates, cellWasVisitedOnThisStep);
        break;
    case Direction::Right :
        goRight(currentCoordinates, newCoordinates, cellWasVisitedOnThisStep);
        break;
    case Direction::Bot :
        goBot(currentCoordinates, newCoordinates, cellWasVisitedOnThisStep);
        break;
    case Direction::Left :
        goLeft(currentCoordinates, newCoordinates, cellWasVisitedOnThisStep);
        break;
    }

    markCellAfterStep(currentCoordinates, newCoordinates);
    delay(DELAY_MS_IN_GENERATION_CYCLE);

    currentCoordinates = newCoordinates;
    if (cellWasVisitedOnThisStep)
        visitedCellsCounter++;
}

/*------------------------------------------------------------------------------------------------*/
void Maze::markCellAfterStep(Coordinate currentCoordinates, Coordinate newCoordinates)
{
    cellGrid_[currentCoordinates.x][currentCoordinates.y].getRectForShowCurrentCell()->setVisible(false);
    cellGrid_[newCoordinates.x][newCoordinates.y].getRectForShowCurrentCell()->setVisible(true);
}

/*------------------------------------------------------------------------------------------------*/
void Maze::goTop(Coordinate &currentCoordinates, Coordinate &newCoordinates, bool &cellWasVisitedOnThisStep)
{
    newCoordinates.y = newCoordinates.y - 1;
    if (cellGrid_[newCoordinates.x][newCoordinates.y].isVisited() == false)
    {
        cellGrid_[currentCoordinates.x][currentCoordinates.y].destroyTopWall();
        cellGrid_[newCoordinates.x][newCoordinates.y].destroyBotWall();
        cellGrid_[newCoordinates.x][newCoordinates.y].wasVisited();
        cellWasVisitedOnThisStep = true;
    }
}

/*------------------------------------------------------------------------------------------------*/
void Maze::goRight(Coordinate &currentCoordinates, Coordinate &newCoordinates, bool &cellWasVisitedOnThisStep)
{
    newCoordinates.x = newCoordinates.x + 1;
    if (cellGrid_[newCoordinates.x][newCoordinates.y].isVisited() == false)
    {
        cellGrid_[currentCoordinates.x][currentCoordinates.y].destroyRightWall();
        cellGrid_[newCoordinates.x][newCoordinates.y].destroyLeftWall();
        cellGrid_[newCoordinates.x][newCoordinates.y].wasVisited();
        cellWasVisitedOnThisStep = true;
    }
}

/*------------------------------------------------------------------------------------------------*/
void Maze::goBot(Coordinate &currentCoordinates, Coordinate &newCoordinates, bool &cellWasVisitedOnThisStep)
{
    newCoordinates.y = newCoordinates.y + 1;
    if (cellGrid_[newCoordinates.x][newCoordinates.y].isVisited() == false)
    {
        cellGrid_[currentCoordinates.x][currentCoordinates.y].destroyBotWall();
        cellGrid_[newCoordinates.x][newCoordinates.y].destroyTopWall();
        cellGrid_[newCoordinates.x][newCoordinates.y].wasVisited();
        cellWasVisitedOnThisStep = true;
    }
}

/*------------------------------------------------------------------------------------------------*/
void Maze::goLeft(Coordinate &currentCoordinates, Coordinate &newCoordinates, bool &cellWasVisitedOnThisStep)
{
    newCoordinates.x = newCoordinates.x - 1;
    if (cellGrid_[newCoordinates.x][newCoordinates.y].isVisited() == false)
    {
        cellGrid_[currentCoordinates.x][currentCoordinates.y].destroyLeftWall();
        cellGrid_[newCoordinates.x][newCoordinates.y].destroyRightWall();
        cellGrid_[newCoordinates.x][newCoordinates.y].wasVisited();
        cellWasVisitedOnThisStep = true;
    }
}

/*------------------------------------------------------------------------------------------------*/
void Maze::delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}
/*void Maze::saveToFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        // Gérer l'erreur
        return;
    }

    QDataStream out(&file);
    // Supposons que mazeSize est un attribut de votre classe Maze
    out << mazeSize; // Exemple de sérialisation d'un attribut simple

    // Sérialisez ici d'autres parties de votre labyrinthe
    file.close();
}

void Maze::loadFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        // Gérer l'erreur
        return;
    }

    QDataStream in(&file);
    // Lire et reconstruire les attributs de votre labyrinthe
    in >> mazeSize; // Exemple de désérialisation

    // Désérialisez ici d'autres parties de votre labyrinthe
    file.close();
}*/

void Maze::loadMazeFromFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for reading.");
    }

    std::vector<char> mazeData((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
    file.close();

    // Assurez-vous que mazeData contient un nombre correct de bytes pour reconstruire le labyrinthe.
    // Par exemple, si chaque cellule est représentée par 2 bytes et que votre labyrinthe est 5x5, vous devriez avoir 50 bytes.
    if (mazeData.size() % 2 != 0) { // Simple vérification, ajustez selon votre structure de données.
        throw std::runtime_error("Corrupted maze file.");
    }

    cellGrid_.clear();
    unsigned int mazeSize = std::sqrt(mazeData.size() / 2); // Ajustez cette logique selon la structure de votre labyrinthe.
    mazeSize_ = mazeSize;
    unsigned int cellSize = mazeGridSizePx_ / mazeSize_;

    for (size_t i = 0; i < mazeData.size(); i += 2) {
        unsigned int row = static_cast<unsigned int>(mazeData[i]);
        unsigned int col = static_cast<unsigned int>(mazeData[i + 1]);
        // Supposons que chaque cellule est représentée par ses coordonnées (row, col).
        // Ajoutez la logique pour reconstruire chaque cellule ici.
        if (col == 0) { // Nouvelle colonne
            cellGrid_.push_back(QVector<Cell>());
        }
        cellGrid_.back().push_back(Cell(cellSize, row, col));
    }

    // Après avoir reconstruit le labyrinthe, vous pouvez émettre un signal ou appeler une fonction pour le dessiner.
    emit requestToDrawMazeGrid(getCellGrid());
}
