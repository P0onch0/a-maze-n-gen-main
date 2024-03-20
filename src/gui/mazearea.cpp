#include "gui/mazearea.h"

MazeArea::MazeArea(QWidget *parent) noexcept
    : QWidget(parent)
{
    initializeMenu();

    maze_ = new Maze(MAZE_AREA_SIZE);
    connect(maze_, &Maze::requestToDrawMazeGrid, this, &MazeArea::drawMazeGrid);
    connect(maze_, &Maze::mazeWasGenerated, this, &MazeArea::requestToEnableAllButtons);
}

/*------------------------------------------------------------------------------------------------*/
void MazeArea::initializeMenu()
{
    mazeScene_ = new QGraphicsScene;
    mazeView_ = new QGraphicsView;
    mazeView_->setFixedSize(GRAPHIC_VIEW_SIZE, GRAPHIC_VIEW_SIZE);
    mazeView_->setScene(mazeScene_);

    mazeAreaLayout_ = new QVBoxLayout(this);
    mazeAreaLayout_->setAlignment(Qt::AlignCenter);
    mazeAreaLayout_->addWidget(mazeView_);

    mazeAreaGroupBox_ = new QGroupBox(this);
    mazeAreaGroupBox_->setStyleSheet(MAZE_AREA_STYLE_SHEET);
    mazeAreaGroupBox_->setLayout(mazeAreaLayout_);

    setMinimumSize(mazeAreaGroupBox_->sizeHint());
}

/*------------------------------------------------------------------------------------------------*/
void MazeArea::drawMazeGrid(QVector<QVector<Cell>>& cellGrid)
{
    mazeScene_->clear();
    for (auto row = cellGrid.begin(); row != cellGrid.end(); row++)
    {
        for (auto col = row->begin(); col != row->end(); col++)
        {
            addCellOnScene(*col);
        }
    }

    emit fieldReadyToGenerate();
}

/*------------------------------------------------------------------------------------------------*/
void MazeArea::addCellOnScene(Cell& cell)
{
    mazeScene_->addItem(cell.getTopWall());
    mazeScene_->addItem(cell.getBotWall());
    mazeScene_->addItem(cell.getLeftWall());
    mazeScene_->addItem(cell.getRightWall());

    mazeScene_->addItem(cell.getRectForShowCurrentCell());
}

/*------------------------------------------------------------------------------------------------*/
void MazeArea::startGenerateMazeGrid(unsigned int mazeSize)
{
    maze_->generateMazeGrid(mazeSize);
}

/*------------------------------------------------------------------------------------------------*/
void MazeArea::startGenerationMaze(int whichAlgorithmWasChosen)
{
    /* Вынужден вызывать функцию resetGrid перед каждой новой генерацией для перестраховки.
     * Были проблемы с генерацией лабиринта без переключения размера.
     * Вариант с перегенерацией через startGenerateMazeGrid с тем же размером по неизвестной
     * причине вызывал неотлавливаемый баг и сам метод был костыльным и некрасивым. Текущее
     * решение лучше и оно работает на 100%. */

    maze_->resetGrid();
    maze_->generateMaze(whichAlgorithmWasChosen);
}

/*------------------------------------------------------------------------------------------------*/
void MazeArea::interruptGenerationHandling()
{
    maze_->interruptReceived();
}
