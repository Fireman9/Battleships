#ifndef FIELD_H
#define FIELD_H

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsObject>
#include <QPointF>

class Field: public QGraphicsObject
{
    Q_OBJECT
public:
    Field();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void setGameStart(bool game);
    void setGameEnd(bool gameEnd);
    QRectF boundingRect() const override;

private:
    bool gameStarted;
    bool gameEnd;
    QPixmap *sprite;

signals:
    void giveCoord(QPointF pos);
};

#endif // FIELD_H
