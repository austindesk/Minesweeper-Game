#include "cell.h"
#include <QDebug>

Cell::Cell(QWidget *parent)
    : QPushButton(parent), mine(false), adjacentMines(0), state(Closed) {

}

void Cell::setMine(bool mineVal) {
    mine = mineVal;
}

bool Cell::isMine() const {
    return mine;
}

void Cell::setAdjacentMines(int count) {
    adjacentMines = count;
}

int Cell::getAdjacentMines() const {
    return adjacentMines;
}

#include "cell.h"

void Cell::reveal() {
     if (state == Opened) return;

    state = Opened;
    setDisabled(true); // Disable the cell to prevent further clicks

    if (isMine()) {
        setIcon(QIcon(":/new/prefix1/minesweeper_icons/bomb_explode.png")); // Use your resource path
        emit mineTriggered();
    } else {
        int count = getAdjacentMines();
        if (count > 0) {
            setText(QString::number(count));
        } else {
            // If there are no adjacent mines, emit the revealAdjacent signal
            emit revealAdjacent();
        }
        setIcon(QIcon()); // Clear any existing icon
    }
}


bool Cell::isRevealed() const {
    return state == Opened;
}

void Cell::revealMine() {
    if (mine) {
        setIcon(QIcon(":/new/prefix1/minesweeper_icons/bomb.png"));
    }
}


void Cell::toggleMark() {
    if (state == Opened) return;

    if (state == Closed) {
        state = Marked;
        setIcon(QIcon(":/new/prefix1/minesweeper_icons/mine_flag.png")); // Set the flag icon
        setText(""); // Clear any text
    } else if (state == Marked) {
        state = Questionable;
        setIcon(QIcon()); // Clear the flag icon
        setText("?"); // Set text to a question mark
    } else if (state == Questionable) {
        state = Closed;
        setIcon(QIcon()); // Clear any icon
        setText(""); // Clear the text
    }
}


void Cell::resetState() {
    setMine(false);
    setAdjacentMines(0);
    setText("");
    setEnabled(true);
    state = Closed;
}

void Cell::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        toggleMark();
    } else if (event->button() == Qt::LeftButton) {
        reveal();  // Directly call reveal without conditions
    }
}



