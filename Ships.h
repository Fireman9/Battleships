#ifndef SHIPS_H
#define SHIPS_H

#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsObject>

class Ships : public QGraphicsObject
{
    Q_OBJECT
public:
    Ships(int decks);
    int getRow();
    int getCol();
    int getDecks();
    bool isHorizontal();
    bool isAlive();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void setStartPos(int x, int y);
    void toStartPos();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void setHorizontal();
    void setVertical();
    void setRow(int row);
    void setCol(int col);
    void setInDockyard(bool inDockyard);
    void wasHitted();
    QRectF boundingRect() const override;

private:
    int decks;
    int x, y;
    int row, col;
    int mouseDx, mouseDy;
    int decksHitted;
    bool horizontal;
    bool wasMoved;
    bool inDockyard;
    bool alive;
    QPixmap *sprite;

signals:
    void release(Ships *ship);
};

#endif // SHIPS_H
