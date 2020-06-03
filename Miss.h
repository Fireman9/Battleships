#ifndef MISS_H
#define MISS_H

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsObject>

class Miss: public QGraphicsObject
{
    Q_OBJECT
public:
    Miss();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

private:
    QPixmap *sprite;
};

#endif // MISS_H
