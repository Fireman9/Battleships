#include "Ships.h"

Ships::Ships(int decks)
{
    this->decks = decks;
    this->horizontal = true;
    this->wasMoved = false;
    this->row = -1;
    this->col = -1;
    this->inDockyard = true;
    this->decksHitted = 0;
    this->alive = true;
}

void Ships::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (decks == 1) {
        sprite = new QPixmap("://images/Ship1.png");
        painter->drawPixmap(0, 0, *sprite);
    }
    else if (decks == 2) {
        sprite = new QPixmap("://images/Ship2.png");
        painter->drawPixmap(0, 0, *sprite);
    }
    else if (decks == 3) {
        sprite = new QPixmap("://images/Ship3.png");
        painter->drawPixmap(0, 0, *sprite);
    }
    else if (decks == 4) {
        sprite = new QPixmap("://images/Ship4.png");
        painter->drawPixmap(0, 0, *sprite);
    }
    else {
        sprite = new QPixmap("://images/Ship5.png");
        painter->drawPixmap(0, 0, *sprite);
    }
    Q_UNUSED(option)
        Q_UNUSED(widget)
}

QRectF Ships::boundingRect() const
{
    if (decks == 1) {
        return QRectF(0, 0, 50, 50);
    }
    else if (decks == 2) {
        return QRectF(0, 0, 99, 50);
    }
    else if (decks == 3) {
        return QRectF(0, 0, 148, 50);
    }
    else if (decks == 4) {
        return QRectF(0, 0, 197, 50);
    }
    else {
        return QRectF(0, 0, 246, 50);
    }
}

void Ships::setStartPos(int x, int y)
{
    this->x = x;
    this->y = y;
    this->setPos(this->x, this->y);
}

void Ships::toStartPos()
{
    this->setPos(this->x, this->y);
}

void Ships::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (inDockyard) {
        this->mouseDx = static_cast<int>(event->pos().x());
        this->mouseDy = static_cast<int>(event->pos().y());
        this->setCursor(QCursor(Qt::ClosedHandCursor));
        Q_UNUSED(event)
    }
}

void Ships::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (inDockyard) {
        this->wasMoved = true;
        QPointF temp = event->pos();
        temp.setX(temp.x() - mouseDx);
        temp.setY(temp.y() - mouseDy);
        this->setPos(mapToScene(temp));
    }
}

void Ships::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (inDockyard) {
        if (wasMoved) {
            this->wasMoved = false;
            this->setCursor(QCursor(Qt::ArrowCursor));
            emit release(this);
            Q_UNUSED(event)
        }
        else {
            if (horizontal) {
                setVertical();
            }
            else {
                setHorizontal();
            }
            this->setCursor(QCursor(Qt::ArrowCursor));
            emit release(this);
            Q_UNUSED(event)
        }
    }
}

int Ships::getDecks()
{
    return this->decks;
}

bool Ships::isHorizontal()
{
    return this->horizontal;
}

void Ships::setHorizontal()
{
    this->horizontal = true;
    this->setRotation(0);
    this->setX(this->pos().x() - 49);
}

void Ships::setVertical()
{
    this->horizontal = false;
    this->setRotation(90);
    this->setX(this->pos().x() + 49);
}

void Ships::setRow(int row)
{
    this->row = row;
}

void Ships::setCol(int col)
{
    this->col = col;
}

int Ships::getRow()
{
    return this->row;
}

int Ships::getCol()
{
    return this->col;
}

void Ships::setInDockyard(bool dockyard)
{
    this->inDockyard = dockyard;
}

bool Ships::isAlive()
{
    return this->alive;
}

void Ships::wasHitted()
{
    this->decksHitted++;
    if (this->decksHitted == this->decks) this->alive = false;
}
