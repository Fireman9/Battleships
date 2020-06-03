#include "AI.h"

AI::AI()
{
    fieldArray.resize(11);
    for (int i = 0; i < fieldArray.size(); i++) {
        fieldArray[i].resize(11);
    }
    for (int i = 0; i < fieldArray.size(); i++) {
        for (int j = 0; j < fieldArray[i].size(); j++) {
            fieldArray[i][j] = 0;
        }
    }
    std::random_shuffle(set5.begin(), set5.end());
    std::random_shuffle(set4_3.begin(), set4_3.end());
    std::random_shuffle(set2.begin(), set2.end());
}

int AI::getRow()
{
    if (hittedCells.empty()) {
        if (!set5.empty()) {
            currentRow = set5[0].first;
            return currentRow;
        }
        else if (!set4_3.empty()) {
            currentRow = set4_3[0].first;
            return currentRow;
        }
        else if (!set2.empty()) {
            currentRow = set2[0].first;
            return currentRow;
        }
        else {
            if (!cellsLeft.empty()) {
                currentRow = cellsLeft[0].first;
                return currentRow;
            }
        }
    }
    else {
        currentRow = possibleCells[0].first;
        return currentRow;
    }
}

int AI::getCol()
{
    if (hittedCells.empty()) {
        if (!set5.empty()) {
            currentCol = set5[0].second;
            return set5[0].second;
        }
        else if (!set4_3.empty()) {
            currentCol = set4_3[0].second;
            return set4_3[0].second;
        }
        else if (!set2.empty()) {
            currentCol = set2[0].second;
            return set2[0].second;
        }
        else {
            if (!cellsLeft.empty()) {
                currentCol = cellsLeft[0].second;
                return currentCol;
            }
        }
    }
    else {
        currentCol = possibleCells[0].second;
        return currentCol;
    }
}

void AI::shotResult(bool hit, bool killed)
{
    if (hit) {
        fieldArray[currentRow][currentCol] = -1;
        delCell(currentRow, currentCol);
        hittedCells.push_back(qMakePair(currentRow, currentCol));
        if (killed) {
            if (hittedCells.size() == 1) {
                horizontal = true;
            }
            else {
                if (hittedCells[0].first == hittedCells[1].first) {
                    horizontal = true;
                }
                else {
                    horizontal = false;
                }
            }
            QVector<QPair<int, int>> cellsAround;
            if (horizontal) {
                int minCol = hittedCells[0].second;
                for (int i = 0; i < hittedCells.size(); i++) {
                    if (hittedCells[i].second < minCol) minCol = hittedCells[i].second;
                }
                cellsAround = getCellsAroundShip(currentRow, minCol, horizontal, hittedCells.size());
            }
            else {
                int minRow = hittedCells[0].first;
                for (int i = 0; i < hittedCells.size(); i++) {
                    if (hittedCells[i].first < minRow) minRow = hittedCells[i].first;
                }
                cellsAround = getCellsAroundShip(minRow, currentCol, horizontal, hittedCells.size());
            }
            for (int i = 0; i < cellsAround.size(); i++) {
                fieldArray[cellsAround[i].first][cellsAround[i].second] = -1;
                delCell(cellsAround[i].first, cellsAround[i].second);
            }
            hittedCells.clear();
            possibleCells.clear();
        }
        else {
            if (hittedCells.size() > 1) {
                possibleCells.remove(0);
                getPossibleCells(currentRow, currentCol);
                if (hittedCells[0].first == hittedCells[1].first) {
                    horizontal = true;
                }
                else {
                    horizontal = false;
                }
                if (horizontal) {
                    for (int i = 0; i < possibleCells.size(); i++) {
                        if (possibleCells[i].first != hittedCells[0].first) {
                            possibleCells.remove(i);
                            i--;
                        }
                    }
                }
                else {
                    for (int i = 0; i < possibleCells.size(); i++) {
                        if (possibleCells[i].second != hittedCells[0].second) {
                            possibleCells.remove(i);
                            i--;
                        }
                    }
                }
            }
            else {
                getPossibleCells(currentRow, currentCol);
            }
        }


    }
    else {
        fieldArray[currentRow][currentCol] = -1;
        if (hittedCells.empty()) {
            delCell(currentRow, currentCol);
        }
        else {
            delCell(currentRow, currentCol);
            possibleCells.remove(0);
        }
    }
    if (set5.empty() && set4_3.empty() && set2.empty()) {
        cellsLeft = getAllLeftCells();
    }
}

void AI::getPossibleCells(int row, int col)
{
    if (row + 1 <= 10) {
        if (fieldArray[row + 1][col] != -1) {
            possibleCells.push_back(qMakePair(row + 1, col));
        }
    }
    if (row - 1 >= 0) {
        if (fieldArray[row - 1][col] != -1) {
            possibleCells.push_back(qMakePair(row - 1, col));
        }
    }
    if (col + 1 <= 10) {
        if (fieldArray[row][col + 1] != -1) {
            possibleCells.push_back(qMakePair(row, col + 1));
        }
    }
    if (col - 1 >= 0) {
        if (fieldArray[row][col - 1] != -1) {
            possibleCells.push_back(qMakePair(row, col - 1));
        }
    }
    std::random_shuffle(possibleCells.begin(), possibleCells.end());
}

void AI::delCell(int row, int col)
{
    for (int i = 0; i < set5.size(); i++) {
        if (set5[i].first == row && set5[i].second == col) {
            set5.remove(i);
            break;
        }
    }
    for (int i = 0; i < set4_3.size(); i++) {
        if (set4_3[i].first == row && set4_3[i].second == col) {
            set4_3.remove(i);
            break;
        }
    }
    for (int i = 0; i < set2.size(); i++) {
        if (set2[i].first == row && set2[i].second == col) {
            set2.remove(i);
            break;
        }
    }
}

QVector<QPair<int, int> > AI::getAllLeftCells()
{
    QVector<QPair<int, int>> cellsLeft;
    for (int i = 0; i < fieldArray.size(); i++) {
        for (int j = 0; j < fieldArray[i].size(); j++) {
            if (fieldArray[i][j] != -1) {
                cellsLeft.push_back(qMakePair(i, j));
            }
        }
    }
    return cellsLeft;
}

QVector<QPair<int, int> > AI::getCellsAroundShip(int row, int col, bool horizontal, int decks)
{
    QVector<QPair<int, int>> cellsAroundShip;
    if (horizontal) {
        for (int i = 0; i < decks; i++) {
            cellsAroundShip.push_back(qMakePair(row + 1, col + i));
            cellsAroundShip.push_back(qMakePair(row - 1, col + i));
        }
        for (int i = -1; i < 2; i++) {
            cellsAroundShip.push_back(qMakePair(row + i, col - 1));
            cellsAroundShip.push_back(qMakePair(row + i, col + decks));
        }
    }
    else {
        for (int i = 0; i < decks; i++) {
            cellsAroundShip.push_back(qMakePair(row + i, col + 1));
            cellsAroundShip.push_back(qMakePair(row + i, col - 1));
        }
        for (int i = -1; i < 2; i++) {
            cellsAroundShip.push_back(qMakePair(row - 1, col + i));
            cellsAroundShip.push_back(qMakePair(row + decks, col + i));
        }
    }
    for (int i = 0; i < cellsAroundShip.size(); i++) {
        if (cellsAroundShip[i].first < 0 || cellsAroundShip[i].first > 10 || cellsAroundShip[i].second < 0 || cellsAroundShip[i].second > 10) {
            cellsAroundShip.remove(i);
            i--;
        }
    }
    return cellsAroundShip;
}
