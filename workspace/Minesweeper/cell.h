#ifndef CELL_H
#define CELL_H

#include <QPushButton>
#include <QMouseEvent>

class Cell : public QPushButton {
    Q_OBJECT

public:
    enum CellState { Closed, Marked, Questionable, Opened };
    Cell(QWidget *parent = nullptr);

    void setMine(bool mine);
    bool isMine() const;
    void setAdjacentMines(int count);
    int getAdjacentMines() const;
    void reveal();
    void toggleMark();
    void resetState();
    bool isRevealed() const;
    void revealMine();

signals:
    void mineTriggered();
    void revealAdjacent();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool mine;
    int adjacentMines;
    CellState state;
};

#endif // CELL_H
