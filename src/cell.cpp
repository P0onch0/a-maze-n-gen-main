#include "cell.h"
#include <QDebug>
#include <vector>

Cell::Cell(unsigned int cellSize, unsigned int row, unsigned int col) noexcept
    : cellSize_(cellSize),
      visited_(false)
{
    QPoint leftTopPoint {};
    QPoint rightTopPoint {};
    QPoint leftBotPoint {};
    QPoint rightBotPoint {};

    leftTopPoint.setX((col + 0) * cellSize_);
    leftTopPoint.setY((row + 0) * cellSize_);

    rightTopPoint.setX((col + 1) * cellSize_);
    rightTopPoint.setY((row + 0) * cellSize_);

    leftBotPoint.setX((col + 0) * cellSize_);
    leftBotPoint.setY((row + 1) * cellSize_);

    rightBotPoint.setX((col + 1) * cellSize_);
    rightBotPoint.setY((row + 1) * cellSize_);


    topWall_ = new QGraphicsLineItem;
    botWall_ = new QGraphicsLineItem;
    leftWall_ = new QGraphicsLineItem;
    rightWall_ = new QGraphicsLineItem;

    topWall_->setLine(QLineF(leftTopPoint, rightTopPoint));
    botWall_->setLine(QLineF(leftBotPoint, rightBotPoint));
    leftWall_->setLine(QLineF(leftTopPoint, leftBotPoint));
    rightWall_->setLine(QLineF(rightTopPoint, rightBotPoint));

    /* Координаты второй точки необходимо уменьшить на 1, т.к. квадрат отображается на 1 пиксель
     * больше, чем узлы сетки, построенной на линиях */
    rightBotPoint.setX(rightBotPoint.x() - 1);
    rightBotPoint.setY(rightBotPoint.y() - 1);

    /* Идея в том, чтобы сразу добавить все квадраты, которые отображают текущую ячейку лабиринта,
     * на сцену. Квадрат сразу прячется и убирается на второй план, чтобы не мешать стенкам */
    rectForShowCurrentCell_ = new QGraphicsRectItem(QRectF(QRect(leftTopPoint, rightBotPoint)));
    rectForShowCurrentCell_->setVisible(false);
    rectForShowCurrentCell_->setZValue(-1);
    rectForShowCurrentCell_->setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    rectForShowCurrentCell_->setPen(Qt::NoPen);
}

/*------------------------------------------------------------------------------------------------*/
QGraphicsLineItem* Cell::getTopWall() const
{
    return topWall_;
}

/*------------------------------------------------------------------------------------------------*/
QGraphicsLineItem* Cell::getBotWall() const
{
    return botWall_;
}

/*------------------------------------------------------------------------------------------------*/
QGraphicsLineItem* Cell::getLeftWall() const
{
    return leftWall_;
}

/*------------------------------------------------------------------------------------------------*/
QGraphicsLineItem* Cell::getRightWall() const
{
    return rightWall_;
}

/*------------------------------------------------------------------------------------------------*/
void Cell::destroyTopWall()
{
    topWall_->setVisible(false);
}

/*------------------------------------------------------------------------------------------------*/
void Cell::destroyBotWall()
{
    botWall_->setVisible(false);
}

/*------------------------------------------------------------------------------------------------*/
void Cell::destroyLeftWall()
{
    leftWall_->setVisible(false);
}

/*------------------------------------------------------------------------------------------------*/
void Cell::destroyRightWall()
{
    rightWall_->setVisible(false);
}

/*------------------------------------------------------------------------------------------------*/
QGraphicsRectItem* Cell::getRectForShowCurrentCell() const
{
    return rectForShowCurrentCell_;
}

/*------------------------------------------------------------------------------------------------*/
bool Cell::isVisited() const
{
    return visited_;
}

/*------------------------------------------------------------------------------------------------*/
void Cell::wasVisited()
{
    visited_ = true;
}

/*------------------------------------------------------------------------------------------------*/
void Cell::setUnvisited()
{
    visited_ = false;
}

/*------------------------------------------------------------------------------------------------*/
void Cell::resetCell()
{
    topWall_->setVisible(true);
    botWall_->setVisible(true);
    leftWall_->setVisible(true);
    rightWall_->setVisible(true);
    visited_ = false;
}
std::vector<char> Cell::toBytes() const {
    std::vector<char> bytes;

    // Conversion de l'état visité de la cellule en bytes.
    // Ajoute 1 pour visité, 0 pour non visité.
    bytes.push_back(visited_ ? 1 : 0);

    // Exemple pour inclure l'état des murs si nécessaire.
    // Vous devrez implémenter la logique pour déterminer si un mur existe.
    // bytes.push_back(topWall_->isVisible() ? 0 : 1); // Supposons que isVisible() retourne false si le mur est détruit.
    // bytes.push_back(rightWall_->isVisible() ? 0 : 1);
    // bytes.push_back(botWall_->isVisible() ? 0 : 1);
    // bytes.push_back(leftWall_->isVisible() ? 0 : 1);

    // Affichage du contenu de bytes pour le débogage
    qDebug() << "Cell toBytes:";
    for (char byte : bytes) {
        qDebug() << static_cast<int>(byte);
    }

    return bytes;
}
/*
char Cell::getCellStateAsByte() const {
    // Implémentez la logique pour convertir l'état de la cellule en un byte.
    // Cela pourrait être une combinaison des états de paroi de la cellule, visité ou non, etc.
    char state = 0;
    // Exemple : supposons que nous avons 4 bits pour représenter les murs dans les 4 directions
    // et que vous avez des méthodes comme hasTopWall(), hasBottomWall(), etc.
    if (hasTopWall()) state |= 0b0001;
    if (hasRightWall()) state |= 0b0010;
    if (hasBottomWall()) state |= 0b0100;
    if (hasLeftWall()) state |= 0b1000;

    return state;
}*/


