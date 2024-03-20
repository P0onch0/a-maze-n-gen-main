#pragma once

#include "basewidgetmenu.h"
#include "startstoppushbutton.h"

#include <QRadioButton>
#include <QPushButton>

class AlgorithmGeneratorMenu : public BaseWidgetMenu
{
    Q_OBJECT

private:
    QRadioButton *algorithmAldousBroderRadio_ {nullptr};
    QRadioButton *algorithmRecursiveBacktrackerRadio_ {nullptr};
    QRadioButton *algorithmWilsonRadio_ {nullptr};

    QPushButton *test_ {nullptr};
    StartStopPushButton *startGenerationButton_ {nullptr};


    int whichAlgorithmWasChosen_ {};
    bool isFieldReadyToGenerate_ {false};
    bool isAlgorithmReadyToGenerate_ {false};

public:
    AlgorithmGeneratorMenu(QWidget *parent = nullptr) noexcept;
    ~AlgorithmGeneratorMenu() {};

    void initializeMenu();
    void setDisabledButtons(bool makeButtonsDisabled);
    enum Algorithm {AldousBroder, RecursiveBacktracker, Wilson};

signals:
    void algorithmReadyToGenerate();
    void requestToDisableAllButtons();
    void interruptGeneration();
    void startGenerationMaze(int whichAlgorithmWasChosen);
    /* Ce n'est probablement pas la meilleure implémentation pour transmettre des informations sur l'algorithme de génération sélectionné,
      * parce que une telle solution implique la nécessité de réécrire le code dans le récepteur (labyrinthe)
      * s'il est nécessaire de remplacer l'algorithme. Cependant, pour le moment, avec "l'architecture" actuelle, je ne pense pas
      * Je sais comment cela peut être mis en œuvre de manière plus élégante */
private slots:
    void slotAldousBroderRadio();
    void slotRecursiveBacktrackerRadio();
    void slotWilsonRadio();
    void slotStartGenerationButton();
    void loadMaze();

public slots:
    void activateGenerateButton();
};
