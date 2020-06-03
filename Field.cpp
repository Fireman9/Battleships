#include "Field.h"

Field::Field()
{
    this->gameStarted = false;
    this->gameEnd = false;
}

void Field::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    sprite = new QPixmap("://images/Field.png");
    painter->drawPixmap(0, 0, *sprite);
}

QRectF Field::boundingRect() const
{
    return QRectF(0, 0, 589, 589);
}

void Field::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

}

void Field::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (!this->gameEnd) {
        if (this->gameStarted) {
            emit giveCoord(event->pos());
        }
    }
}

void Field::setGameStart(bool game)
{
    this->gameStarted = game;
}

void Field::setGameEnd(bool gameEnd)
{
    this->gameEnd = gameEnd;
}
