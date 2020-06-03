#include "Hit.h"

Hit::Hit()
{

}

void Hit::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    sprite = new QPixmap("://images/Hit.png");
    painter->drawPixmap(0, 0, *sprite);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRectF Hit::boundingRect() const
{
    return QRectF(0, 0, 48, 48);
}
