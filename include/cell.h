#pragma once

#include <QPoint>
#include <QLineF>
#include <QGraphicsLineItem>
#include <QRect>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>

class Cell
{
private:
    unsigned int cellSize_ {};
    bool visited_ {false};

    QGraphicsLineItem *topWall_ {nullptr};
    QGraphicsLineItem *botWall_ {nullptr};
    QGraphicsLineItem *leftWall_ {nullptr};
    QGraphicsLineItem *rightWall_ {nullptr};

    QGraphicsRectItem *rectForShowCurrentCell_ {nullptr};

public:
    explicit Cell(unsigned int cellSize, unsigned int row, unsigned int col) noexcept;
    ~Cell() {};

    QGraphicsLineItem* getTopWall() const;
    QGraphicsLineItem* getBotWall() const;
    QGraphicsLineItem* getLeftWall() const;
    QGraphicsLineItem* getRightWall() const;

    void destroyTopWall();
    void destroyBotWall();
    void destroyLeftWall();
    void destroyRightWall();
    //char getCellStateAsByte() const; // Déclaration de la méthode

    QGraphicsRectItem* getRectForShowCurrentCell() const;

    bool isVisited() const;
    void wasVisited();
    void setUnvisited();
    std::vector<char> toBytes() const;
    void resetCell();
};
