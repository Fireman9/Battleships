#ifndef DOCKYARD_H
#define DOCKYARD_H

#include <QWidget>
#include <QGraphicsScene>
#include <QLabel>
#include <QString>
#include <QVector>
#include <QPair>
#include <QtMath>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QDateTime>

#include "Ships.h"
#include "Field.h"
#include "Miss.h"
#include "Hit.h"
#include "AI.h"

namespace Ui {
    class Dockyard;
}

class Dockyard : public QWidget
{
    Q_OBJECT
public:
    explicit Dockyard(QWidget *parent = nullptr);
    bool settingShipsToArray(Ships* ship, QVector<QVector<int>>& fieldArray);
    bool shipsAutoPositioning(QVector<QVector<int>>& fieldArray, QVector<QVector<Ships*>>& ships);
    bool checkForPHit(int row, int col);
    bool checkForAIHit(int row, int col, bool& killed);
    bool gameEnd(QVector<QVector<Ships*>>& ships);
    void delPreviousPos(Ships* ship);
    void sceneSetup();
    void startGame();
    void getPossibleShipPos(QVector<int>& places, bool isHor, int rowOrCol, int decks, QVector<QVector<int>>& fieldArray);
    ~Dockyard();

private:
    Ui::Dockyard *ui;
    int turns;
    Field* fieldP;
    Field* fieldAI;
    AI *ai;
    QGraphicsScene *scene;
    QVector<QVector<Ships*>> shipsP;
    QVector<QVector<Ships*>> shipsAI;
    QVector<QVector<int>> fieldArrayP;
    QVector<QVector<int>> fieldArrayAI;
    QGraphicsProxyWidget *proxyAutoButton;
    QGraphicsProxyWidget *proxyToGameButton;
    QGraphicsProxyWidget *proxyPlaceText;
    QLabel *placeText;

signals:
    void toMainMenu();

private slots:
    void on_menuButton_clicked();
    void on_autoButton_clicked();
    void on_toGameButton_clicked();
    void checkForCorrectShipPos(Ships *ship);
    void gameProcess(QPointF pos);
};

#endif // DOCKYARD_H
