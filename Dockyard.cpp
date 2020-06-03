#include "Dockyard.h"
#include "ui_Dockyard.h"

Dockyard::Dockyard(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Dockyard)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->graphicsView->setFrameShape(QFrame::NoFrame);
    ui->verticalLayout->setSpacing(0);
    ui->verticalLayout->setContentsMargins(0, 0, 0, 0);
    qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);
    ai = new AI();
    this->turns = 0;
    sceneSetup();
}

void Dockyard::sceneSetup()
{
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene->setSceneRect(0, 0, 1371, 812);

    QLabel* placeText = new QLabel("Place your ships:\n(click to rotate)");
    placeText->setStyleSheet("font-family: 'Lucida Sans Unicode', 'Lucida Grande', sans-serif;"
        "background-color: rgba(0,0,0,0);"
        "font-size: 40px;"
        "color: #3060ce;"
        "font-weight: 700;"
        "margin-top: 35 px;"
        "margin-left: 110 px;"
        "text-decoration: none solid rgb(68, 68, 68);"
        "text-transform: uppercase;");
    proxyPlaceText = scene->addWidget(placeText);
    proxyPlaceText->setPos(0, 0);

    QPushButton* autoButton = new QPushButton("Auto");
    autoButton->setStyleSheet("font-family: 'Lucida Sans Unicode', 'Lucida Grande', sans-serif;"
        "background-color: #ffffff;"
        "font-size: 79px;"
        "color: #3060ce;"
        "font-weight: 700;"
        "text-decoration: none solid rgb(68, 68, 68);"
        "text-transform: uppercase;"
        "border:2px solid #3060ce;"
        "padding-left: 107 px;"
        "padding-right: 106 px;");
    connect(autoButton, &QPushButton::clicked, this, &Dockyard::on_autoButton_clicked);
    proxyAutoButton = scene->addWidget(autoButton);
    proxyAutoButton->setPos(98, 668);

    QPushButton* toGameButton = new QPushButton("To Game");
    toGameButton->setStyleSheet("font-family: 'Lucida Sans Unicode', 'Lucida Grande', sans-serif;"
        "background-color: #ffffff;"
        "font-size: 79px;"
        "color: #3060ce;"
        "font-weight: 700;"
        "text-decoration: none solid rgb(68, 68, 68);"
        "text-transform: uppercase;"
        "border:2px solid #3060ce;"
        "padding-left: 61 px;"
        "padding-right: 60 px;");
    connect(toGameButton, &QPushButton::clicked, this, &Dockyard::on_toGameButton_clicked);
    proxyToGameButton = scene->addWidget(toGameButton);
    proxyToGameButton->setPos(784, 668);

    fieldP = new Field();
    fieldP->setPos(686, 32);
    scene->addItem(fieldP);

    fieldArrayP.resize(11);
    for (int i = 0; i < fieldArrayP.size(); i++) {
        fieldArrayP[i].resize(11);
    }
    for (int i = 0; i < fieldArrayP.size(); i++) {
        for (int j = 0; j < fieldArrayP[i].size(); j++) {
            fieldArrayP[i][j] = -1;
        }
    }

    fieldArrayAI.resize(11);
    for (int i = 0; i < fieldArrayAI.size(); i++) {
        fieldArrayAI[i].resize(11);
    }
    for (int i = 0; i < fieldArrayAI.size(); i++) {
        for (int j = 0; j < fieldArrayAI[i].size(); j++) {
            fieldArrayAI[i][j] = -1;
        }
    }

    shipsP.resize(5);
    shipsAI.resize(5);
    int shipsCounter = 1, decks = 5;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < shipsCounter; j++) {
            shipsP[i].push_back(new Ships(decks));
            shipsAI[i].push_back(new Ships(decks));
        }
        shipsCounter++;
        decks--;
    }

    while (!shipsAutoPositioning(fieldArrayAI, shipsAI));

    for (int i = 0; i < shipsP.size(); i++) {
        for (int j = 0; j < shipsP[i].size(); j++) {
            connect(shipsP[i][j], &Ships::release, this, &Dockyard::checkForCorrectShipPos);
        }
    }

    shipsP[0][0]->setStartPos(196, 179);
    scene->addItem(shipsP[0][0]);

    shipsP[1][0]->setStartPos(98, 277);
    scene->addItem(shipsP[1][0]);
    shipsP[1][1]->setStartPos(343, 277);
    scene->addItem(shipsP[1][1]);

    shipsP[2][0]->setStartPos(49, 375);
    scene->addItem(shipsP[2][0]);
    shipsP[2][1]->setStartPos(245, 375);
    scene->addItem(shipsP[2][1]);
    shipsP[2][2]->setStartPos(441, 375);
    scene->addItem(shipsP[2][2]);

    shipsP[3][0]->setStartPos(49, 473);
    scene->addItem(shipsP[3][0]);
    shipsP[3][1]->setStartPos(196, 473);
    scene->addItem(shipsP[3][1]);
    shipsP[3][2]->setStartPos(343, 473);
    scene->addItem(shipsP[3][2]);
    shipsP[3][3]->setStartPos(490, 473);
    scene->addItem(shipsP[3][3]);

    shipsP[4][0]->setStartPos(98, 571);
    scene->addItem(shipsP[4][0]);
    shipsP[4][1]->setStartPos(196, 571);
    scene->addItem(shipsP[4][1]);
    shipsP[4][2]->setStartPos(294, 571);
    scene->addItem(shipsP[4][2]);
    shipsP[4][3]->setStartPos(392, 571);
    scene->addItem(shipsP[4][3]);
    shipsP[4][4]->setStartPos(490, 571);
    scene->addItem(shipsP[4][4]);
}

bool Dockyard::settingShipsToArray(Ships* ship, QVector<QVector<int>>& fieldArray)
{
    if (ship->getCol() == -1)return false;
    bool isEmpty = true;
    if (ship->isHorizontal()) {
        for (int i = 0; i < ship->getDecks(); i++) {
            if (fieldArray[ship->getRow()][ship->getCol() + i] != -1)isEmpty = false;
        }
        if (isEmpty) {
            for (int i = 0; i < ship->getDecks(); i++) {
                fieldArray[ship->getRow()][ship->getCol() + i] = ship->getDecks();
            }
            QVector<QPair<int, int>> pointsAround = ai->getCellsAroundShip(ship->getRow(), ship->getCol(), true, ship->getDecks());
            for (int i = 0; i < pointsAround.size(); i++) {
                fieldArray[pointsAround[i].first][pointsAround[i].second] = 0;
            }
            return true;
        }
        else {
            ship->setCol(-1);
            ship->setRow(-1);
            return false;
        }
    }
    else {
        for (int i = 0; i < ship->getDecks(); i++) {
            if (fieldArray[ship->getRow() + i][ship->getCol()] != -1)isEmpty = false;
        }
        if (isEmpty) {
            for (int i = 0; i < ship->getDecks(); i++) {
                fieldArray[ship->getRow() + i][ship->getCol()] = ship->getDecks();
            }
            QVector<QPair<int, int>> pointsAround = ai->getCellsAroundShip(ship->getRow(), ship->getCol(), false, ship->getDecks());
            for (int i = 0; i < pointsAround.size(); i++) {
                fieldArray[pointsAround[i].first][pointsAround[i].second] = 0;
            }
            return true;
        }
        else {
            ship->setCol(-1);
            ship->setRow(-1);
            return false;
        }
    }
}

Dockyard::~Dockyard()
{
    delete ui;
}

void Dockyard::delPreviousPos(Ships* ship)
{
    if (ship->getRow() == -1) {
        return;
    }
    else {
        ship->setCol(-1);
        ship->setRow(-1);
    }
}

void Dockyard::on_menuButton_clicked()
{
    this->close();
    emit toMainMenu();
}

void Dockyard::on_autoButton_clicked()
{
    for (int i = 0; i < fieldArrayP.size(); i++) {
        for (int j = 0; j < fieldArrayP[i].size(); j++) {
            fieldArrayP[i][j] = -1;
        }
    }
    for (int i = 0; i < shipsP.size(); i++) {
        for (int j = 0; j < shipsP[i].size(); j++) {
            delPreviousPos(shipsP[i][j]);
        }
    }
    while (!shipsAutoPositioning(fieldArrayP, shipsP));
    for (int i = 0; i < shipsP.size(); i++) {
        for (int j = 0; j < shipsP[i].size(); j++) {
            if (shipsP[i][j]->isHorizontal()) {
                shipsP[i][j]->setPos(fieldP->x() + 49 + shipsP[i][j]->getCol() * 49, fieldP->y() + 49 + shipsP[i][j]->getRow() * 49);
                shipsP[i][j]->show();
            }
            else {
                shipsP[i][j]->setPos(fieldP->x() + (49 * 2) + 1 + shipsP[i][j]->getCol() * 49, fieldP->y() + 49 + shipsP[i][j]->getRow() * 49);
                shipsP[i][j]->show();
            }
        }
    }
}

void Dockyard::on_toGameButton_clicked()
{
    for (int i = 0; i < shipsP.size(); i++) {
        for (int j = 0; j < shipsP[i].size(); j++) {
            if (shipsP[i][j]->getRow() == -1) {
                return;
            }
        }
    }
    startGame();
}

bool Dockyard::shipsAutoPositioning(QVector<QVector<int>>& fieldArray, QVector<QVector<Ships*>>& ships)
{
    for (int i = 0; i < ships.size(); i++) {
        for (int j = 0; j < ships[i].size(); j++) {
            QVector<int> places1;
            QVector<int> places2;
            ships[i][j]->hide();
            bool isHor = qrand() % 2;
            if (isHor) {
                ships[i][j]->setHorizontal();
            }
            else {
                ships[i][j]->setVertical();
            }
            if (ships[i][j]->isHorizontal()) {
                for (int k = 0; k < 11; k++) {
                    getPossibleShipPos(places2, true, k, ships[i][j]->getDecks(), fieldArray);
                    if (!places2.isEmpty()) {
                        places1.push_back(k);
                    }
                    places2.clear();
                }
            }
            else {
                for (int k = 0; k < 11; k++) {
                    getPossibleShipPos(places2, false, k, ships[i][j]->getDecks(), fieldArray);
                    if (!places2.isEmpty()) {
                        places1.push_back(k);
                    }
                    places2.clear();
                }
            }
            if (places1.isEmpty()) {
                for (int i = 0; i < fieldArray.size(); i++) {
                    for (int j = 0; j < fieldArray[i].size(); j++) {
                        fieldArray[i][j] = -1;
                    }
                }
                for (int i = 0; i < ships.size(); i++) {
                    for (int j = 0; j < ships[i].size(); j++) {
                        delPreviousPos(ships[i][j]);
                    }
                }
                return false;
            }
            else {
                if (isHor) {
                    int row = places1[qrand() % places1.size()];
                    getPossibleShipPos(places2, true, row, ships[i][j]->getDecks(), fieldArray);
                    int col = places2[qrand() % places2.size()];
                    ships[i][j]->setCol(col);
                    ships[i][j]->setRow(row);
                    settingShipsToArray(ships[i][j], fieldArray);
                }
                else {
                    int col = places1[qrand() % places1.size()];
                    getPossibleShipPos(places2, false, col, ships[i][j]->getDecks(), fieldArray);
                    int row = places2[qrand() % places2.size()];
                    ships[i][j]->setCol(col);
                    ships[i][j]->setRow(row);
                    settingShipsToArray(ships[i][j], fieldArray);
                }
            }
        }
    }
    return true;
}

void Dockyard::startGame()
{
    for (int i = 0; i < fieldArrayP.size(); i++) {
        for (int j = 0; j < fieldArrayP[i].size(); j++) {
            if (fieldArrayP[i][j] == -1)fieldArrayP[i][j] = 0;
            if (fieldArrayAI[i][j] == -1)fieldArrayAI[i][j] = 0;
        }
    }

    ui->dockyardLabel->setText("SeaBattle");

    scene->removeItem(proxyAutoButton);
    scene->removeItem(proxyToGameButton);
    scene->removeItem(proxyPlaceText);

    fieldP->setPos(49, 32);
    fieldAI = new Field();
    fieldAI->setGameStart(true);
    connect(fieldAI, &Field::giveCoord, this, &Dockyard::gameProcess);
    fieldAI->setPos(686, 32);
    scene->addItem(fieldAI);
    placeText = new QLabel("Turns: " + QString::number(turns));
    placeText->setStyleSheet("font-family: 'Lucida Sans Unicode', 'Lucida Grande', sans-serif;"
        "background-color: rgba(0,0,0,0);"
        "font-size: 100px;"
        "color: #3060ce;"
        "font-weight: 700;"
        "text-decoration: none solid rgb(68, 68, 68);"
        "text-transform: uppercase;");
    placeText->resize(600, 100);
    proxyPlaceText = scene->addWidget(placeText);
    proxyPlaceText->setPos(420, 673);


    for (int i = 0; i < shipsP.size(); i++) {
        for (int j = 0; j < shipsP[i].size(); j++) {
            if (shipsP[i][j]->isHorizontal()) {
                shipsP[i][j]->setPos(fieldP->x() + 49 + shipsP[i][j]->getCol() * 49, fieldP->y() + 49 + shipsP[i][j]->getRow() * 49);
            }
            else {
                shipsP[i][j]->setPos(fieldP->x() + (49 * 2) + 1 + shipsP[i][j]->getCol() * 49, fieldP->y() + 49 + shipsP[i][j]->getRow() * 49);
            }
            shipsP[i][j]->setInDockyard(false);
        }
    }

    for (int i = 0; i < shipsAI.size(); i++) {
        for (int j = 0; j < shipsAI[i].size(); j++) {
            if (shipsAI[i][j]->isHorizontal()) {
                scene->addItem(shipsAI[i][j]);
                shipsAI[i][j]->setPos(fieldAI->x() + 49 + shipsAI[i][j]->getCol() * 49, fieldAI->y() + 49 + shipsAI[i][j]->getRow() * 49);
            }
            else {
                scene->addItem(shipsAI[i][j]);
                shipsAI[i][j]->setPos(fieldAI->x() + (49 * 2) + 1 + shipsAI[i][j]->getCol() * 49, fieldAI->y() + 49 + shipsAI[i][j]->getRow() * 49);
            }
            shipsAI[i][j]->setInDockyard(false);
        }
    }

}

void Dockyard::getPossibleShipPos(QVector<int>& places, bool isHor, int rowOrCol, int decks, QVector<QVector<int>>& fieldArray)
{
    bool can = true;
    if (isHor) {
        for (int i = 0; i < 12 - decks; i++) {
            for (int j = i; j < i + decks; j++) {
                if (fieldArray[rowOrCol][j] != -1) {
                    can = false;
                    break;
                }
            }
            if (can) {
                places.push_back(i);
            }
            can = true;
        }
    }
    else {
        for (int i = 0; i < 12 - decks; i++) {
            for (int j = i; j < i + decks; j++) {
                if (fieldArray[j][rowOrCol] != -1) {
                    can = false;
                    break;
                }
            }
            if (can) {
                places.push_back(i);
            }
            can = true;
        }
    }
}

bool Dockyard::checkForPHit(int row, int col)
{
    if (fieldArrayAI[row][col] == 0) {
        Miss* miss = new Miss();
        miss->setPos(fieldAI->x() + col * 49 + 50, fieldAI->y() + row * 49 + 50);
        scene->addItem(miss);
        fieldArrayAI[row][col] = -1;
        return false;
    }
    else {
        Hit* hit = new Hit();
        hit->setPos(fieldAI->x() + col * 49 + 50, fieldAI->y() + row * 49 + 50);
        scene->addItem(hit);
        int temp = qFabs(fieldArrayAI[row][col] - 5);
        for (int i = 0; i < shipsAI[temp].size(); i++) {
            if (shipsAI[temp][i]->isHorizontal()) {
                if (shipsAI[temp][i]->getRow() == row) {
                    for (int j = 0; j < shipsAI[temp][i]->getDecks(); j++) {
                        if (shipsAI[temp][i]->getCol() + j == col) {
                            shipsAI[temp][i]->wasHitted();
                            if (!shipsAI[temp][i]->isAlive()) {
                                QVector<QPair<int, int>> pointsArround = ai->getCellsAroundShip(shipsAI[temp][i]->getRow(), shipsAI[temp][i]->getCol(),
                                    shipsAI[temp][i]->isHorizontal(), shipsAI[temp][i]->getDecks());
                                for (int i = 0; i < pointsArround.size(); i++) {
                                    if (fieldArrayAI[pointsArround[i].first][pointsArround[i].second] != -1) {
                                        Miss* miss = new Miss();
                                        miss->setPos(fieldAI->x() + pointsArround[i].second * 49 + 50, fieldAI->y() + pointsArround[i].first * 49 + 50);
                                        scene->addItem(miss);
                                        fieldArrayAI[pointsArround[i].first][pointsArround[i].second] = -1;
                                    }
                                }
                                shipsAI[temp][i]->show();
                            }
                        }
                    }
                }
            }
            else {
                if (shipsAI[temp][i]->getCol() == col) {
                    for (int j = 0; j < shipsAI[temp][i]->getDecks(); j++) {
                        if (shipsAI[temp][i]->getRow() + j == row) {
                            shipsAI[temp][i]->wasHitted();
                            if (!shipsAI[temp][i]->isAlive()) {
                                QVector<QPair<int, int>> pointsArround = ai->getCellsAroundShip(shipsAI[temp][i]->getRow(), shipsAI[temp][i]->getCol(),
                                    shipsAI[temp][i]->isHorizontal(), shipsAI[temp][i]->getDecks());
                                for (int i = 0; i < pointsArround.size(); i++) {
                                    if (fieldArrayAI[pointsArround[i].first][pointsArround[i].second] != -1) {
                                        Miss* miss = new Miss();
                                        miss->setPos(fieldAI->x() + pointsArround[i].second * 49 + 50, fieldAI->y() + pointsArround[i].first * 49 + 50);
                                        scene->addItem(miss);
                                        fieldArrayAI[pointsArround[i].first][pointsArround[i].second] = -1;
                                    }
                                }
                                shipsAI[temp][i]->show();
                            }
                        }
                    }
                }
            }
        }
        fieldArrayAI[row][col] = -1;
        return true;
    }
}

bool Dockyard::checkForAIHit(int row, int col, bool& killed)
{
    if (fieldArrayP[row][col] == 0) {
        Miss* miss = new Miss();
        miss->setPos(fieldP->x() + col * 49 + 50, fieldP->y() + row * 49 + 50);
        scene->addItem(miss);
        fieldArrayP[row][col] = -1;
        return false;
    }
    else {
        Hit* hit = new Hit();
        hit->setPos(fieldP->x() + col * 49 + 50, fieldP->y() + row * 49 + 50);
        scene->addItem(hit);
        int temp = qFabs(fieldArrayP[row][col] - 5);
        for (int i = 0; i < shipsAI[temp].size(); i++) {
            if (shipsP[temp][i]->isHorizontal()) {
                if (shipsP[temp][i]->getRow() == row) {
                    for (int j = 0; j < shipsP[temp][i]->getDecks(); j++) {
                        if (shipsP[temp][i]->getCol() + j == col) {
                            shipsP[temp][i]->wasHitted();
                            if (!shipsP[temp][i]->isAlive()) {
                                QVector<QPair<int, int>> pointsArround = ai->getCellsAroundShip(shipsP[temp][i]->getRow(), shipsP[temp][i]->getCol(),
                                    shipsP[temp][i]->isHorizontal(), shipsP[temp][i]->getDecks());
                                for (int i = 0; i < pointsArround.size(); i++) {
                                    if (fieldArrayP[pointsArround[i].first][pointsArround[i].second] != -1) {
                                        Miss* miss = new Miss();
                                        miss->setPos(fieldP->x() + pointsArround[i].second * 49 + 50, fieldP->y() + pointsArround[i].first * 49 + 50);
                                        scene->addItem(miss);
                                        fieldArrayP[pointsArround[i].first][pointsArround[i].second] = -1;
                                    }
                                }
                                killed = true;
                            }
                        }
                    }
                }
            }
            else {
                if (shipsP[temp][i]->getCol() == col) {
                    for (int j = 0; j < shipsP[temp][i]->getDecks(); j++) {
                        if (shipsP[temp][i]->getRow() + j == row) {
                            shipsP[temp][i]->wasHitted();
                            if (!shipsP[temp][i]->isAlive()) {
                                QVector<QPair<int, int>> pointsArround = ai->getCellsAroundShip(shipsP[temp][i]->getRow(), shipsP[temp][i]->getCol(),
                                    shipsP[temp][i]->isHorizontal(), shipsP[temp][i]->getDecks());
                                for (int i = 0; i < pointsArround.size(); i++) {
                                    if (fieldArrayP[pointsArround[i].first][pointsArround[i].second] != -1) {
                                        Miss* miss = new Miss();
                                        miss->setPos(fieldP->x() + pointsArround[i].second * 49 + 50, fieldP->y() + pointsArround[i].first * 49 + 50);
                                        scene->addItem(miss);
                                        fieldArrayP[pointsArround[i].first][pointsArround[i].second] = -1;
                                    }
                                }
                                killed = true;
                            }
                        }
                    }
                }
            }
        }
        fieldArrayP[row][col] = -1;
        return true;
    }
}

bool Dockyard::gameEnd(QVector<QVector<Ships*>>& ships)
{
    for (int i = 0; i < ships.size(); i++) {
        for (int j = 0; j < ships[i].size(); j++) {
            if (ships[i][j]->isAlive())return false;
        }
    }
    return true;
}

void Dockyard::checkForCorrectShipPos(Ships* ship)
{
    for (int i = 0; i < fieldArrayP.size(); i++) {
        for (int j = 0; j < fieldArrayP[i].size(); j++) {
            fieldArrayP[i][j] = -1;
        }
    }
    delPreviousPos(ship);
    for (int i = 0; i < shipsP.size(); i++) {
        for (int j = 0; j < shipsP[i].size(); j++) {
            settingShipsToArray(shipsP[i][j], fieldArrayP);
        }
    }
    if (ship->isHorizontal()) {
        if (!(fieldP->x() + 25 <= ship->pos().x() && ship->pos().x() <= fieldP->x() + 589 &&
            fieldP->y() + 25 <= ship->pos().y() && ship->pos().y() <= fieldP->y() + 589)) {
            ship->toStartPos();
        }
        else {
            ship->setCol(qFloor((ship->pos().x() - 735) / 49 + 0.5));
            ship->setRow(qFloor((ship->pos().y() - 81) / 49 + 0.5));
            if (ship->getRow() > 10) ship->setRow(10);
            if (ship->getCol() > 10) ship->setCol(10);
            if ((11 - ship->getDecks()) < ship->getCol()) {
                delPreviousPos(ship);
                ship->toStartPos();
                return;
            }
            if (!settingShipsToArray(ship, fieldArrayP)) {
                delPreviousPos(ship);
                ship->toStartPos();
            }
            else {
                ship->setPos(fieldP->x() + 49 + ship->getCol() * 49, fieldP->y() + 49 + ship->getRow() * 49);
            }
        }
    }
    else {
        if (!(fieldP->x() + 25 <= ship->pos().x() - 49 && ship->pos().x() - 49 <= fieldP->x() + 589 &&
            fieldP->y() + 25 <= ship->pos().y() && ship->pos().y() <= fieldP->y() + 589)) {
            ship->setHorizontal();
            ship->toStartPos();
        }
        else {
            ship->setCol(qFloor((ship->pos().x() - 784) / 49 + 0.5));
            ship->setRow(qFloor((ship->pos().y() - 81) / 49 + 0.5));
            if (ship->getRow() > 10) ship->setRow(10);
            if (ship->getCol() > 10) ship->setCol(10);
            if ((11 - ship->getDecks()) < ship->getRow()) {
                ship->setHorizontal();
                delPreviousPos(ship);
                ship->toStartPos();
                return;
            }
            if (!settingShipsToArray(ship, fieldArrayP)) {
                ship->setHorizontal();
                delPreviousPos(ship);
                ship->toStartPos();
            }
            else {
                ship->setPos(fieldP->x() + (49 * 2) + 1 + ship->getCol() * 49, fieldP->y() + 49 + ship->getRow() * 49);
            }
        }
    }
}

void Dockyard::gameProcess(QPointF pos)
{
    if (pos.x() < 49 || pos.y() < 49) {
        return;
    }
    int row = (pos.y() - 49) / 49;
    int col = (pos.x() - 49) / 49;

    if (row > 10)row = 10;
    if (col > 10)col = 10;

    if (fieldArrayAI[row][col] == -1) return;
    this->turns++;
    if (checkForPHit(row, col)) {
        this->turns--;
        if (gameEnd(shipsAI)) {
            fieldAI->setGameEnd(true);
            ui->dockyardLabel->setText("You won!");
        }
        placeText->setText("Turns: " + QString::number(turns));
        return;
    }
    else {
        bool AISucessShoot = true;
        while (AISucessShoot) {
            if (gameEnd(shipsP)) {
                fieldAI->setGameEnd(true);
                for (int i = 0; i < shipsAI.size(); i++) {
                    for (int j = 0; j < shipsAI[i].size(); j++) {
                        shipsAI[i][j]->show();
                    }
                }
                ui->dockyardLabel->setText("You lose!");
                break;
            }
            bool killed = false;
            if (checkForAIHit(ai->getRow(), ai->getCol(), killed)) {
                ai->shotResult(true, killed);
            }
            else {
                ai->shotResult(false, killed);
                AISucessShoot = false;
            }
        }
    }
    placeText->setText("Turns: " + QString::number(turns));
}
