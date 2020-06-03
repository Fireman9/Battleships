#include "Miss.h"

Miss::Miss()
{

}

void Miss::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    sprite = new QPixmap("://images/Miss.png");
    painter->drawPixmap(0, 0, *sprite);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRectF Miss::boundingRect() const
{
    return QRectF(0, 0, 48, 48);
}
