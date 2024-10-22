#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "cell.h"  // Ensure this includes your Cell class definition

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void cellClicked();  // Slot for handling cell clicks
    void checkGameState();  // Check the current state of the game
    void gameWon();  // Handle winning the game
    void gameOver();  // Handle game over scenario
    void revealAdjacentCells(int row, int col);  // Method for flood fill feature

private:
    std::vector<std::vector<Cell*>> cells;  // 2D vector to hold cells
    void setupBoard();  // Set up the Minesweeper game board
    void placeMines();  // Place mines randomly on the board
    void calculateAdjacentMines();  // Calculate adjacent mines for each cell
    void restartGame();  // Restart the game
    static constexpr int mineCount = 99;  // Total number of mines

};

#endif // MAINWINDOW_H
