#include "mainwindow.h"
#include <QGridLayout>
#include <random>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupBoard();
    placeMines();
    calculateAdjacentMines();
}

MainWindow::~MainWindow() {
    for (auto &row : cells) {
        for (Cell *cell : row) {
            delete cell;
        }
    }
}
void MainWindow::cellClicked() {
    Cell *cell = qobject_cast<Cell *>(sender());
    if (cell) {
        // Perform actions when a cell is clicked
        // For example, revealing the cell
        cell->reveal();
        checkGameState();
    }
}

void MainWindow::setupBoard() {
    QGridLayout *layout = new QGridLayout();
    layout->setSpacing(0);                // Remove spacing between cells
    layout->setContentsMargins(0, 0, 0, 0); // Set the margins to zero

    QWidget *central = new QWidget(this);
    central->setLayout(layout);
    setCentralWidget(central);

    const QSize cellSize = QSize(20, 20); // Fixed size for each cell

    cells.resize(16, std::vector<Cell*>(30, nullptr));

    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 30; ++col) {
            Cell *cell = new Cell(this);
            cell->setFixedSize(cellSize);
            cells[row][col] = cell;
            layout->addWidget(cell, row, col);

            // Existing connections for clicked and mineTriggered signals
            connect(cell, &QPushButton::clicked, this, &MainWindow::cellClicked);
            connect(cell, &Cell::mineTriggered, this, &MainWindow::gameOver);
            // Connect the revealAdjacent signal to the revealAdjacentCells slot
            connect(cell, &Cell::revealAdjacent, [this, row, col]() {
                revealAdjacentCells(row, col);
            });
        }
    }


}




void MainWindow::placeMines() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 29);

    int minesPlaced = 0;
    while (minesPlaced < mineCount) {
        int row = dis(gen) % 16;
        int col = dis(gen) % 30;

        Cell *cell = cells[row][col];
        if (cell && !cell->isMine()) {
            cell->setMine(true);
            minesPlaced++;
        }
    }
}

void MainWindow::calculateAdjacentMines() {
    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 30; ++col) {
            Cell *cell = cells[row][col];
            if (!cell->isMine()) {
                int mineCount = 0;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        int adjRow = row + i;
                        int adjCol = col + j;
                        if (adjRow >= 0 && adjRow < 16 && adjCol >= 0 && adjCol < 30 && cells[adjRow][adjCol]->isMine()) {
                            mineCount++;
                        }
                    }
                }
                cell->setAdjacentMines(mineCount);
            }
        }
    }
}

void MainWindow::checkGameState() {
    for (const auto &row : cells) {
        for (const Cell *cell : row) {
            if (!cell->isMine() && cell->text().isEmpty()) {
                return;  // Game continues
            }
        }
    }
    gameWon();
}

void MainWindow::gameWon() {
    QMessageBox::information(this, "Minesweeper", "Congratulations, you won!");
    restartGame();
}

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

void MainWindow::gameOver() {
    for (auto &row : cells) {
        for (Cell *cell : row) {
            if (cell->isMine()) {
                cell->revealMine();  // New method to reveal mine without triggering gameOver
            }
        }
    }
    // Create a custom dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Game Over");

    // Use QLabel with rich text to show text and image
    QLabel *label = new QLabel(&dialog);
    QString labelText = "<html><body>"
                        "<h3>Boom! Game Over.</h3>"
                        "<p><img src=':/new/prefix1/minesweeper_icons/bomb_explode_sm.png'/>" // Correct path to your image
                        " Do you want to restart?</p></body></html>";
    label->setText(labelText);

    // Create buttons for the dialog
    QPushButton *yesButton = new QPushButton("Yes", &dialog);
    QPushButton *noButton = new QPushButton("No", &dialog);

    // Connect the Yes button to restart the game and close the dialog
    connect(yesButton, &QPushButton::clicked, &dialog, [this, &dialog]() {
        restartGame();
        dialog.accept(); // Close the dialog
    });

    // Connect the No button to exit the application
    connect(noButton, &QPushButton::clicked, &dialog, [&dialog]() {
        QApplication::quit(); // Exit the application
    });

    // Arrange widgets in the dialog
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->addWidget(label);
    layout->addWidget(yesButton);
    layout->addWidget(noButton);

    // Show the dialog
    dialog.exec();
}




void MainWindow::revealAdjacentCells(int row, int col) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newRow = row + i;
            int newCol = col + j;

            // Check for bounds
            if (newRow >= 0 && newRow < 16 && newCol >= 0 && newCol < 30) {
                Cell *adjCell = cells[newRow][newCol];

                // Only reveal if the cell is not a mine and is not already revealed
                if (!adjCell->isMine() && !adjCell->isRevealed()) {
                    adjCell->reveal();

                    // If the adjacent cell has no neighboring mines, recurse
                    if (adjCell->getAdjacentMines() == 0) {
                        revealAdjacentCells(newRow, newCol);
                    }
                }
            }
        }
    }
}



void MainWindow::restartGame() {
    for (auto &row : cells) {
        for (Cell *cell : row) {
            cell->resetState();  // Resets the mine status and adjacent mines count
            cell->setIcon(QIcon());  // Clears the icon
            cell->setText("");  // Clears the text
            cell->setEnabled(true);  // Re-enables the cell for clicking
        }
    }
    placeMines();  // Place mines again for the new game
    calculateAdjacentMines();  // Recalculate adjacent mines
}

