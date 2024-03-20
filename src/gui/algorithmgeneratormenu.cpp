#include "gui/algorithmgeneratormenu.h"

AlgorithmGeneratorMenu::AlgorithmGeneratorMenu(QWidget *parent) noexcept
    : BaseWidgetMenu(parent)
{
    algorithmAldousBroderRadio_ = new QRadioButton("Aldous Broder");
    algorithmRecursiveBacktrackerRadio_ = new QRadioButton("Recursive Backtracker");
    algorithmWilsonRadio_ = new QRadioButton("Wilson");
    startGenerationButton_ = new StartStopPushButton();
    test_ = new QPushButton("test");

    AlgorithmGeneratorMenu::initializeMenu();


    connect(algorithmAldousBroderRadio_, &QRadioButton::toggled,
            this, &AlgorithmGeneratorMenu::slotAldousBroderRadio);
    connect(algorithmRecursiveBacktrackerRadio_, &QRadioButton::toggled,
            this, &AlgorithmGeneratorMenu::slotRecursiveBacktrackerRadio);
    connect(algorithmWilsonRadio_, &QRadioButton::toggled,
            this, &AlgorithmGeneratorMenu::slotWilsonRadio);
    connect(startGenerationButton_, &QPushButton::clicked,
            this, &AlgorithmGeneratorMenu::slotStartGenerationButton);
    connect(test_, &QPushButton::clicked, this ,&AlgorithmGeneratorMenu::loadMaze);
}

/*------------------------------------------------------------------------------------------------*/
void AlgorithmGeneratorMenu::initializeMenu()
{
    setTextLabel("Maze Construction Algorithm");

    //addRadioButton(algorithmAldousBroderRadio_);
    addRadioButton(algorithmRecursiveBacktrackerRadio_);
    //addRadioButton(algorithmWilsonRadio_);

    addPushButton(startGenerationButton_);
    addPushButton(test_);
    startGenerationButton_->setDisabled(true);
}

/*------------------------------------------------------------------------------------------------*/
void AlgorithmGeneratorMenu::slotAldousBroderRadio()
{
    whichAlgorithmWasChosen_ = AlgorithmGeneratorMenu::Algorithm::AldousBroder;
    emit algorithmReadyToGenerate();
}

/*------------------------------------------------------------------------------------------------*/
void AlgorithmGeneratorMenu::slotRecursiveBacktrackerRadio()
{
    whichAlgorithmWasChosen_ = AlgorithmGeneratorMenu::Algorithm::RecursiveBacktracker;
    emit algorithmReadyToGenerate();
}

/*------------------------------------------------------------------------------------------------*/
void AlgorithmGeneratorMenu::slotWilsonRadio()
{
    whichAlgorithmWasChosen_ = AlgorithmGeneratorMenu::Algorithm::Wilson;
    emit algorithmReadyToGenerate();
}

/*------------------------------------------------------------------------------------------------*/
void AlgorithmGeneratorMenu::activateGenerateButton()
{
    /* Данный слот связан с двуями сигналами, один из которых отправляется из этого же объекта при
     * нажатии кнопки выбора алгоритма генерации. Поэтому здесь происходит проверка "если
     * отправитель этот же объект" ..., а если нет, значит вариантов больше нет и это сигнал
     * из fieldSizeMenu */
    if (sender() == this)
        isAlgorithmReadyToGenerate_ = true;
    else
        isFieldReadyToGenerate_ = true;

    if (isFieldReadyToGenerate_ && isAlgorithmReadyToGenerate_)
    {
        startGenerationButton_->setDisabled(false);
        startGenerationButton_->makeStateStart();
    }
}

/*------------------------------------------------------------------------------------------------*/
void AlgorithmGeneratorMenu::slotStartGenerationButton()
{
    if (startGenerationButton_->checkCurrentButtonState() == StartStopPushButton::ButtonState::Start)
    {
        // Выключаем кнопки до момента, пока лабиринт не сгенерируется (или не будет остановлен)
        emit requestToDisableAllButtons();
        emit startGenerationMaze(whichAlgorithmWasChosen_);
    }
    else
    {
        emit interruptGeneration();
    }
}

/*------------------------------------------------------------------------------------------------*/
void AlgorithmGeneratorMenu::setDisabledButtons(bool makeButtonsDisabled)
{
    algorithmAldousBroderRadio_->setDisabled(makeButtonsDisabled);
    algorithmRecursiveBacktrackerRadio_->setDisabled(makeButtonsDisabled);
    algorithmWilsonRadio_->setDisabled(makeButtonsDisabled);

    if (makeButtonsDisabled == false)
        startGenerationButton_->makeStateStart();
    else
        startGenerationButton_->makeStateStop();
}

/*------------------------------------------------------------------------------------------------*/
void AlgorithmGeneratorMenu::loadMaze() {
    // Créez une instance de Maze ou obtenez-la si elle existe déjà
    //Maze maze;
    //maze.LoadFromFile("mazeData2.bin"); // Remplacez avec le chemin correct
    // Vous aurez besoin de gérer la mise à jour de l'affichage du labyrinthe ici
}
