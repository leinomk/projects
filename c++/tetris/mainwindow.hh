/* TIE-02201 Ohjelmointi 2
 * Mauri Leino, mauri.leino@tuni.fi
 * Tetris
 */


#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QTimer>
#include <memory>
#include <random>

using Tetromino = std::vector<QGraphicsRectItem*>;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    /**
     * @brief on_startButton_clicked game is started if it is not currently
     * running, otherwise game is paused
     */
    void on_startButton_clicked();

    /**
     * @brief on_resetButton_clicked game is re-initialized by setting time and
     * points to zero, clearing game-area, initializing other variables and
     * setting necessary buttons disabled
     */
    void on_resetButton_clicked();

    /**
     * @brief on_downButton_clicked tetromino in play is moved down as many
     * steps as possible, and points are rewarded accordingly
     */
    void on_downButton_clicked();

    /**
     * @brief on_moveLeftButton_clicked call move_tetromino_vertically and give
     * "left" as parameter
     */
    void on_moveLeftButton_clicked();

    /**
     * @brief on_moveRightButton_clicked call move_tetromino_vertically and give
     * "right" as parameter
     */
    void on_moveRightButton_clicked();

    /**
     * @brief on_rotateButton_clicked tetromino is rotated 90 degrees clockwise
     * if possible
     */
    void on_rotateButton_clicked();

    /**
     * @brief on_timeout advance time
     */
    void on_timeout();

    /**
     * @brief on_tetrominoTimer_timeout call necessary methods to advance game
     */
    void on_tetrominoTimer_timeout();

    /**
     * @brief update_display update time and other numbers shown on screen
     */
    void update_display();

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene_;

    // Constants describing scene coordinates
    // Copied from moving circle example and modified a bit
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 480; // 260; (in moving circle)
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 240; // 680; (in moving circle)

    // Size of a tetromino component
    const int SQUARE_SIDE = 20;
    // Number of horizontal cells (places for tetromino components)
    const int COLUMNS = BORDER_RIGHT / SQUARE_SIDE;
    // Number of vertical cells (places for tetromino components)
    const int ROWS = BORDER_DOWN / SQUARE_SIDE;

    // Constants for different tetrominos and the number of them
    enum Tetromino_kind {HORIZONTAL,
                         LEFT_CORNER,
                         RIGHT_CORNER,
                         SQUARE,
                         STEP_UP_RIGHT,
                         PYRAMID,
                         STEP_UP_LEFT,
                         NUMBER_OF_TETROMINOS};
    // From the enum values above, only the last one is needed in this template.
    // Recall from enum type that the value of the first enumerated value is 0,
    // the second is 1, and so on.
    // Therefore the value of the last one is 7 at the moment.
    // Remove those tetromino kinds above that you do not implement,
    // whereupon the value of NUMBER_OF_TETROMINOS changes, too.
    // You can also remove all the other values, if you do not need them,
    // but most probably you need a constant value for NUMBER_OF_TETROMINOS.


    // For randomly selecting the next dropping tetromino
    std::default_random_engine randomEng;
    std::uniform_int_distribution<int> distr;

    /**
     * @brief move_tetromino_down
     * @return
     */
    bool move_tetromino_down();

    /**
     * @brief move_tetromino_vertically
     * @param direction
     * move tetromino to direction given if possible
     */
    void move_tetromino_vertically(std::string direction);

    /**
     * @brief save_tetromino store tetromino to squares_ vector
     */
    void save_tetromino();

    /**
     * @brief remove_full_rows if there are full rows, they are removed
     */
    void remove_full_rows();

    /**
     * @brief fill_empty_rows if some rows can be moved lower, they are moved
     * @return true if rows were moved, false otherwise
     */
    bool move_rows_down();

    /**
     * @brief generate_tetromino generate a random tetromino and check if it
     * ends the game
     */
    void generate_tetromino();

    /**
     * @brief game_over
     * @return true if player has lost, false otherwise
     */
    bool game_over();

    /**
     * @brief end_game stop game and tell user they lost.
     */
    void end_game();

    //stores the tetromino in play
    std::shared_ptr<Tetromino> currentTetromino_;

    //stores squares played earlier in vectors each representing a row on the
    //playing area
    std::vector<std::vector<QGraphicsRectItem*>> squares_;

    //timer for tracking time used per game
    QTimer* timer_;

    //store the time used for the current game of tetris
    int seconds_;
    int minutes_;

    //timer used to control tetrominos
    QTimer* tetrominoTimer_;

    //difficulty level of the game
    int difficulty_;

    //amount of lines cleared by player
    int removedLines_;

    //default drop rate of tetrominos is one step per 1000ms, or one step per second
    const int DEFAULT_DROPRATE = 1000;

    //how many msecs is taken off from tetromino drop rate per difficulty level
    const int DIFFICULTY_DECREASE = 100;

    //true if the game is running
    bool gameOn_;

    //used to check if current tetromino is square and should not be rotated
    bool isSquare_;

    //store points of the current game
    int points_;

    //number of times player has achieved a tetris (remove 4 rows at once) in
    //current game
    int tetrisCount_;

};

#endif // MAINWINDOW_HH
