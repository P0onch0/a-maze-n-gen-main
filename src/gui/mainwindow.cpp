#include "gui/mainwindow.h"

MainWindow::MainWindow(QWidget *parent) noexcept
    : QWidget(parent)
{
    initializeMazeSettingsMenu();
    initializeMazeArea();
    initializeMainWindow();

    connect(fieldSizeWidget_, &FieldSizeMenu::sendMazeSize, mazeGrid_, &MazeArea::startGenerateMazeGrid);

    connect(mazeGrid_, &MazeArea::fieldReadyToGenerate,
            algorithmGeneratorWidget_, &AlgorithmGeneratorMenu::activateGenerateButton);
    connect(algorithmGeneratorWidget_, &AlgorithmGeneratorMenu::algorithmReadyToGenerate,
            algorithmGeneratorWidget_, &AlgorithmGeneratorMenu::activateGenerateButton);

    connect(algorithmGeneratorWidget_, &AlgorithmGeneratorMenu::requestToDisableAllButtons,
            this, &MainWindow::setDisabledAllButtons);
    connect(mazeGrid_, &MazeArea::requestToEnableAllButtons, this, &MainWindow::setDisabledAllButtons);

    connect(algorithmGeneratorWidget_, &AlgorithmGeneratorMenu::startGenerationMaze,
            mazeGrid_, &MazeArea::startGenerationMaze);

    connect(algorithmGeneratorWidget_, &AlgorithmGeneratorMenu::interruptGeneration,
            mazeGrid_, &MazeArea::interruptGenerationHandling);
  /*  connect(saveButton, &QPushButton::clicked, this, [this]() {
        maze.saveToFile("path/to/save/file");
    });

    connect(loadButton, &QPushButton::clicked, this, [this]() {
        maze.loadFromFile("path/to/load/file");
    });
*/
   // loadMazeButton = new QPushButton("Load Maze", this);
    //QHBoxLayout *buttonLayout = new QHBoxLayout;
    //buttonLayout->addWidget(loadMazeButton); // Ajoutez le nouveau bouton
    //connect(loadMazeButton, &QPushButton::clicked, this, &MainWindow::onLoadMazeClicked);



}

/*------------------------------------------------------------------------------------------------*/
void MainWindow::initializeMazeSettingsMenu()
{
    fieldSizeWidget_ = new FieldSizeMenu(this);
    algorithmGeneratorWidget_ = new AlgorithmGeneratorMenu(this);

    sidebarLayout_ = new QVBoxLayout;
    sidebarLayout_->setSpacing(5);
    sidebarLayout_->setAlignment(Qt::AlignRight | Qt::AlignTop);

    sidebarLayout_->addWidget(fieldSizeWidget_);
    sidebarLayout_->addWidget(algorithmGeneratorWidget_);
}


/*------------------------------------------------------------------------------------------------*/
void MainWindow::initializeMazeArea()
{
    mazeGrid_ = new MazeArea(this);
}

/*------------------------------------------------------------------------------------------------*/
void MainWindow::initializeMainWindow()
{
    setWindowTitle("A-Maze-n-Gen v1.0");
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    mainLayout_ = new QHBoxLayout(this);
    mainLayout_->setSpacing(5);
    mainLayout_->addWidget(mazeGrid_);
    mainLayout_->addLayout(sidebarLayout_);
    setLayout(mainLayout_);
}

/*------------------------------------------------------------------------------------------------*/
void MainWindow::setDisabledAllButtons()
{
    /* Данный слот связан с двумя сигналами, поэтому производим проверку. Если отправитель mazeArea,
     * значит, что сигнал пришел из лабиринта, который сообщает, что он закончил генерацию и можно
     * снова дать пользователю возможность нажимать кнопки. Если обратное (в данном случае без
     * уточнениния), значит отправитель - AlgoGenerator, который сообщает, что была нажата кнопка
     * начала генерации, а значит надо заблокировать пользователю возможность нажимать кнопки. */
    if (sender() == mazeGrid_)
    {
        algorithmGeneratorWidget_->setDisabledButtons(false);
        fieldSizeWidget_->setDisabledButtons(false);
    }
    else
    {
        algorithmGeneratorWidget_->setDisabledButtons(true);
        fieldSizeWidget_->setDisabledButtons(true);
    }
}
