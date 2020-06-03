#ifndef AI_H
#define AI_H

#include <QVector>
#include <QPair>

#include <algorithm>

class AI
{
public:
    AI();
    int getRow();
    int getCol();
    void shotResult(bool hit, bool killed);
    void delCell(int row, int col);
    void getPossibleCells(int row, int col);
    QVector<QPair<int, int>> getAllLeftCells();
    QVector<QPair<int, int>> getCellsAroundShip(int row, int col, bool horizontal, int decks);

private:
    int currentRow;
    int currentCol;
    bool horizontal;
    QVector<QVector<int>> fieldArray;
    QVector<QPair<int, int>> hittedCells;
    QVector<QPair<int, int>> possibleCells;
    QVector<QPair<int, int>> cellsLeft;
    QVector<QPair<int, int>> set5 = {qMakePair(0, 4), qMakePair(1, 3), qMakePair(2, 2), qMakePair(3, 1), qMakePair(4, 0),
                                    qMakePair(0, 9), qMakePair(1, 8), qMakePair(2, 7), qMakePair(3, 6), qMakePair(4, 5),
                                    qMakePair(5, 4), qMakePair(6, 3), qMakePair(7, 2), qMakePair(8, 1), qMakePair(9, 0),
                                    qMakePair(5, 9), qMakePair(6, 8), qMakePair(7, 7), qMakePair(8, 6), qMakePair(9, 5),
                                    qMakePair(4, 10), qMakePair(9, 10), qMakePair(10, 4), qMakePair(10, 9)};
    QVector<QPair<int, int>> set4_3 = {qMakePair(0, 2), qMakePair(1, 1), qMakePair(2, 0), qMakePair(2, 4), qMakePair(3, 3), qMakePair(4, 2),
                                      qMakePair(0, 8), qMakePair(1, 6), qMakePair(2, 5), qMakePair(2, 9), qMakePair(1, 8), qMakePair(0, 7),
                                      qMakePair(5, 2), qMakePair(6, 1), qMakePair(7, 0), qMakePair(7, 4), qMakePair(8, 3), qMakePair(9, 2),
                                      qMakePair(5, 7), qMakePair(6, 6), qMakePair(7, 5), qMakePair(7, 9), qMakePair(8, 8), qMakePair(9, 7),
                                      qMakePair(2, 10), qMakePair(7, 10)};
    QVector<QPair<int, int>> set2 = {qMakePair(0, 0), qMakePair(0, 6), qMakePair(1, 5), qMakePair(5, 1), qMakePair(6, 0),
                                    qMakePair(3, 4), qMakePair(4, 3), qMakePair(10, 1), qMakePair(6, 5), qMakePair(5, 6),
                                    qMakePair(1, 10), qMakePair(9, 3), qMakePair(8, 4), qMakePair(4, 8), qMakePair(3, 9),
                                    qMakePair(8, 9), qMakePair(9, 8), qMakePair(10, 6), qMakePair(6, 10)};
};

#endif // AI_H
