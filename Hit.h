#ifndef HIT_H
#define HIT_H

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsObject>

class Hit: public QGraphicsObject
{
    Q_OBJECT
public:
    Hit();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

private:
    QPixmap *sprite;
};

#endif // HIT_H
