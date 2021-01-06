/* TIE-02201 Ohjelmointi 2
 * Mauri Leino, mauri.leino@tuni.fi
 * Tetris
 */

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QPushButton>
#include <QTimer>
#include <QtWidgets>
#include <memory>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // We need a graphics scene in which to draw rectangles.
    scene_ = new QGraphicsScene(this);

    // The graphicsView object is placed on the window
    // at the following coordinates, but if you want
    // different placement, you can change their values.
    int left_margin = 100; // x coordinate
    int top_margin = 150; // y coordinate

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui->graphicsView->setGeometry(left_margin, top_margin,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a tetromino is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    //Add a 2D grid to the scene_
    for (int x = SQUARE_SIDE;x <= BORDER_RIGHT -1; x += SQUARE_SIDE){
        scene_->addLine(x, 0, x, BORDER_DOWN - 1, QPen(Qt::lightGray));
    }
    for (int y = SQUARE_SIDE;y <= BORDER_DOWN -1; y += SQUARE_SIDE){
        scene_->addLine(0, y, BORDER_RIGHT - 1, y, QPen(Qt::lightGray));
    }

    // Setting random engine ready for the first real call.
    int seed = time(0); // You can change seed value for testing purposes
    randomEng.seed(seed);
    distr = std::uniform_int_distribution<int>(0, NUMBER_OF_TETROMINOS - 1);
    distr(randomEng); // Wiping out the first random number (which is almost always 0)
    // After the above settings, you can use randomEng to draw the next falling
    // tetromino by calling: distr(randomEng) in a suitable method.

    gameOn_ = false;

    //timer for tracking time used in game
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MainWindow::on_timeout);

    //timer used for controlling tetrominos
    tetrominoTimer_ = new QTimer(this);
    connect(tetrominoTimer_, &QTimer::timeout, this,
            &MainWindow::on_tetrominoTimer_timeout);

    difficulty_ = 0;
    tetrisCount_ = 0;
    removedLines_ = 0;
    points_ = 0;
    seconds_ = 0;
    minutes_ = 0;
    update_display();

    ui->moveLeftButton->setDisabled(true);
    ui->moveRightButton->setDisabled(true);
    ui->rotateButton->setDisabled(true);
    ui->downButton->setDisabled(true);
    ui->resetButton->setDisabled(true);

    QPalette timeDisplay = palette();
    timeDisplay.setColor(QPalette::Background, Qt::blue);
    timeDisplay.setColor(QPalette::WindowText, Qt::white);
    ui->lcdNumberSec->setAutoFillBackground(true);
    ui->lcdNumberMin->setAutoFillBackground(true);
    ui->lcdNumberSec->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumberMin->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumberSec->setPalette(timeDisplay);
    ui->lcdNumberMin->setPalette(timeDisplay);

    QPalette pointsDisplay = palette();
    pointsDisplay.setColor(QPalette::Background, Qt::green);
    pointsDisplay.setColor(QPalette::WindowText, Qt::white);
    ui->lcdNumberPoints->setAutoFillBackground(true);
    ui->lcdNumberPoints->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumberPoints->setPalette(pointsDisplay);

    QPalette linesDisplay = palette();
    linesDisplay.setColor(QPalette::Background, Qt::cyan);
    linesDisplay.setColor(QPalette::WindowText, Qt::white);
    ui->lcdNumberLines->setAutoFillBackground(true);
    ui->lcdNumberLines->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumberLines->setPalette(linesDisplay);

    QPalette levelDisplay = palette();
    levelDisplay.setColor(QPalette::Background, Qt::red);
    levelDisplay.setColor(QPalette::WindowText, Qt::white);
    ui->lcdNumberLevel->setAutoFillBackground(true);
    ui->lcdNumberLevel->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumberLevel->setPalette(levelDisplay);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Q){
        MainWindow::close();
    }
    if (event->key() == Qt::Key_A){
        if (gameOn_){
            MainWindow::on_moveLeftButton_clicked();
        }
    }
    if (event->key() == Qt::Key_D){
        if (gameOn_){
            MainWindow::on_moveRightButton_clicked();
        }
    }
    if (event->key() == Qt::Key_S){
        if (gameOn_){
            MainWindow::on_downButton_clicked();
        }
    }
    if (event->key() == Qt::Key_W){
        if (gameOn_){
            MainWindow::on_rotateButton_clicked();
        }
    }
}

void MainWindow::on_startButton_clicked()
{
    //game is not yet started, or it is currently paused, and it is started or
    //continued by starting timers, enabling buttons and generating tetromino
    //if necessary
    if (not gameOn_){
        timer_->start(1000);
        tetrominoTimer_->start(DEFAULT_DROPRATE - (difficulty_ * DIFFICULTY_DECREASE));

        ui->startButton->setText("Pause Game");

        //enable movement control buttons and reset button
        ui->moveLeftButton->setDisabled(false);
        ui->moveRightButton->setDisabled(false);
        ui->rotateButton->setDisabled(false);
        ui->downButton->setDisabled(false);

        ui->resetButton->setDisabled(false);

        gameOn_ = true;

        //if  there is no tetromino in play, a new one is generated
        if (currentTetromino_ == nullptr){
            generate_tetromino();
        }
    }
    //game is running, and it is paused by stopping timers and disabling buttons
    else {
        timer_->stop();
        tetrominoTimer_->stop();
        gameOn_ = false;
        ui->startButton->setText("Continue");

        //disable movement control buttons
        ui->moveLeftButton->setDisabled(true);
        ui->moveRightButton->setDisabled(true);
        ui->rotateButton->setDisabled(true);
        ui->downButton->setDisabled(true);
    }
}

void MainWindow::on_resetButton_clicked()
{
    //stop timer, and set time, points, and other game-related variables to zero
    if (timer_->isActive()){
        timer_->stop();
        tetrominoTimer_->stop();
    }
    seconds_ = 0;
    minutes_ = 0;
    points_ = 0;
    tetrisCount_ = 0;
    removedLines_ = 0;
    difficulty_ = 0;
    update_display();

    //if there are previously placed squares on the screen, they are removed
    if (squares_.size() > 0){
        for (std::vector<QGraphicsRectItem*> row: squares_){
            for (QGraphicsRectItem* square: row){
                scene_->removeItem(square);
                delete square;
            }
        }
        squares_ = {};
    }

    //if there is a tetromino in play, it is removed and currentTetromino_
    //is set to nullptr
    if (currentTetromino_ != nullptr){
        for (QGraphicsRectItem* square: *currentTetromino_){
            scene_->removeItem(square);
            delete square;
        }
        currentTetromino_ = nullptr;
    }

    gameOn_ = false;

    //disable movement control buttons and reset button
    ui->moveLeftButton->setDisabled(true);
    ui->moveRightButton->setDisabled(true);
    ui->rotateButton->setDisabled(true);
    ui->downButton->setDisabled(true);
    ui->resetButton->setDisabled(true);

    ui->resetButton->setText("Reset Game");

    ui->startButton->setText("Start Game");
    ui->startButton->setDisabled(false);

    //in case there is text on top right of screen, it is cleared
    ui->gameOverLabel->setText("");
    ui->tetrisCountLabel->setText("");
}

void MainWindow::on_downButton_clicked()
{
    //move current tetromino down as many steps as it can be moved
    while (move_tetromino_down()){
        points_ += 2;
        continue;
    }
}

void MainWindow::on_moveLeftButton_clicked()
{
    move_tetromino_vertically("left");
}

void MainWindow::on_moveRightButton_clicked()
{
    move_tetromino_vertically("right");
}

void MainWindow::on_rotateButton_clicked()
{
    //square shaped tetrominos are not rotated, as it would make no difference
    if (isSquare_){
        return;
    }

    QGraphicsRectItem* rotationAxis = currentTetromino_->at(1);
    qreal referenceX = rotationAxis->x();
    qreal referenceY = rotationAxis->y();

    //loop is done two time, first time it is checked if the tetromino can be
    //rotated, and second time is done only if yes, and the blocks are moved
    for (int loop = 1; loop <= 2; ++loop){
        for (QGraphicsRectItem* square: *currentTetromino_){
            //rotation axis is not moved when rotating, so it requires no attention
            if (square == rotationAxis){
                continue;
            }

            qreal deltaX = 0;
            qreal deltaY = 0;

            if (square->x() == referenceX){
                deltaY = referenceY - square->y();
                deltaX = referenceY - square->y();
            }
            else if (square->y() == referenceY){
                deltaX = referenceX - square->x();
                deltaY = square->x() - referenceX;
            }
            else if (square->x() > referenceX){
                if (square->y() < referenceY){
                    deltaY = 2 * (referenceY - square->y());
                } else {
                    deltaX = 2 * (referenceX - square->x());
                }
            }
            else if (square->x() < referenceX){
                if (square->y() < referenceY){
                    deltaX = 2 * (referenceX - square->x());
                } else {
                    deltaY = 2 * (referenceY - square->y());
                }
            }

            //on second round the blocks are moved, on the first round the loop
            //is broken if a block cannot be moved
            if (loop == 2){
                square->moveBy(deltaX, deltaY);
            }
            else {
                qreal currentX = square->x();
                qreal currentY = square->y();

                currentX += deltaX;
                currentY += deltaY;

                QRectF rect = scene_->sceneRect();
                //if any block of tetromino would go out of scene, the tetromino
                //cannot be rotated
                if (not rect.contains(currentX, currentY)){
                    return;
                }

                //if there are tetrominos already placed on game area, check if
                // current tetromino would overlap them when rotated
                if (squares_.size() > 0){
                    for (std::vector<QGraphicsRectItem*> row: squares_){
                        for (QGraphicsRectItem* square: row){
                            if (square->x() == currentX and
                                    square->y() == currentY){
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::on_timeout()
{
    if (seconds_ % 60 == 59){
        ++minutes_;

    }
    ++seconds_;
    update_display();
}

void MainWindow::on_tetrominoTimer_timeout()
{
    if (not move_tetromino_down()){
        save_tetromino();
        if (squares_.size() > 0){
            remove_full_rows();
        }
        generate_tetromino();
    }
    update_display();
}

void MainWindow::update_display()
{
    ui->lcdNumberMin->display(minutes_);
    ui->lcdNumberSec->display(seconds_ % 60);
    ui->lcdNumberPoints->display(points_);
    ui->lcdNumberLines->display(removedLines_);
    ui->lcdNumberLevel->display(difficulty_ + 1);
}


bool MainWindow::move_tetromino_down()
{
    bool isDown = false;
    qreal deltaY = static_cast<qreal>(SQUARE_SIDE);
    for (QGraphicsRectItem* currentSquare: *currentTetromino_){
        //current position of tetromino part (square) in question
        qreal current_y = currentSquare->y();
        qreal current_x = currentSquare->x();

        //new y-coordinate of the square
        current_y += deltaY;

        //if the new position is out of scene_, the square is down
        QRectF rect = scene_->sceneRect();
        if (not rect.contains(current_x, current_y)){
            return false;
        }

        //if not already determined to be down, check if tetromino would overlap
        //already placed squares. If yes, tetromino is down
        if (not isDown){
            if (squares_.size() > 0){
                for (std::vector<QGraphicsRectItem*> row: squares_){
                    for (QGraphicsRectItem* square: row){
                        if (square->x() == current_x and square->y() == current_y){
                            return false;
                        }
                    }
                }
            }
        }
    }

    for (QGraphicsRectItem* square: *currentTetromino_){
        square->moveBy(static_cast<qreal>(0), deltaY);
    }
    return true;
}

void MainWindow::move_tetromino_vertically(std::string direction)
{
    qreal deltaX = 0;
    if (direction == "right"){
        deltaX = static_cast<qreal>(SQUARE_SIDE);
    } else {
        deltaX = static_cast<qreal>(-SQUARE_SIDE);
    }

    for (QGraphicsRectItem* currentSquare: *currentTetromino_){
        //current position of tetromino part (square) in question
        qreal currentY = currentSquare->y();
        qreal currentX = currentSquare->x();

        //new x-coordinate of the square
        currentX += deltaX;

        //exit method if square would go out of scene when down
        QRectF rect = scene_->sceneRect();
        if (not rect.contains(currentX, currentY)){
            return;
        }

        //if movement is not already prohibited, check if tetromino would
        //overlap previously placed squares if moved, and if yes, exit method
        if (squares_.size() > 0){
            for (std::vector<QGraphicsRectItem*> row: squares_){
                for (QGraphicsRectItem* square: row){
                    if (square->x() == currentX and square->y() == currentY){
                        return;
                    }
                }
            }
        }
    }

    for (QGraphicsRectItem* square: *currentTetromino_){
        square->moveBy(deltaX, static_cast<qreal>(0));
    }
}

void MainWindow::save_tetromino()
{
    //store every square of current tetromino to squares_, to a vector representing
    // a row in which other squares have the same y-coordinate
    //If no such row is found, a new vector is created
    for (QGraphicsRectItem* square: *currentTetromino_){
        bool rowFound = false;
        if (squares_.size() == 0){
            std::vector<QGraphicsRectItem*> newRow = {square};
            squares_.push_back(newRow);
            rowFound = true;
        } else {
            for (std::vector<QGraphicsRectItem*>& row: squares_){
                if (row.at(0)->y() == square->y()){
                    row.push_back(square);
                    rowFound = true;
                    break;
                }
            }
        }
        if (not rowFound){
            std::vector<QGraphicsRectItem*> newRow = {square};
            squares_.push_back(newRow);
        }
    }
}

void MainWindow::remove_full_rows()
{
    //count how many rows are removed at the same "turn" to track Tetris score
    int rowCount = 0;

    for (std::vector<QGraphicsRectItem*>& row: squares_){
        //row is full if it has as meny squares as there are columns on game
        //area, and those squares are removed and the row vector is erased when
        //empty
        if (row.size() == static_cast<unsigned int>(COLUMNS)){
            while (row.size() > 0){
                std::vector<QGraphicsRectItem*>::iterator iter = row.begin();
                std::vector<QGraphicsRectItem*>::size_type index = 0;
                QGraphicsRectItem* square = row.at(index);
                scene_->removeItem(square);
                delete square;
                row.erase(iter);
            }
            ++rowCount;
            ++removedLines_;
            //difficulty is increased every ten removed lines, and tetrominTimer
            //is restarted with new interval
            if (removedLines_ % 10 == 0){
                difficulty_ = removedLines_ / 10;
                if (difficulty_ > 8){
                    difficulty_ = 8;
                }
                tetrominoTimer_->setInterval(DEFAULT_DROPRATE -
                                       (difficulty_ * DIFFICULTY_DECREASE));
            }
        }
    }

    //add points and delete empty row vectors only if full rows were found and removed
    if (rowCount > 0){
        if (rowCount == 4){
            ++tetrisCount_;
        }
        points_ += 50 * rowCount;

        //remove empty row vectors from squares_
        std::vector<std::vector<QGraphicsRectItem*>>::iterator iter = squares_.begin();
        std::vector<std::vector<QGraphicsRectItem*>>::size_type index = 0;
        while (iter != squares_.end()){
            if (squares_.at(index).size() == 0){
                squares_.erase(iter);
                iter = squares_.begin();
                index = 0;
            } else {
                ++iter;
                ++index;
            }
        }

        //move rows down if necessary
        while (move_rows_down()){
            continue;
        }
    }
}

bool MainWindow::move_rows_down()
{
    bool movedRows = false;
    QRectF rect = scene_->sceneRect();
    for (std::vector<QGraphicsRectItem*>& rowToMove: squares_){
        qreal currentX = rowToMove.at(0)->x();
        qreal currentY = rowToMove.at(0)->y();

        //if row is at the bottom of game area, it cannot be moved down
        if (not rect.contains(currentX, currentY + SQUARE_SIDE)){
            continue;
        }

        bool canBeMoved = true;

        //if row is not at bottom of game area, iterate over other rows and
        // check if one of the is one step lower than row to be moved
        //if yes, row cannot be moved
        for (std::vector<QGraphicsRectItem*>& row: squares_){
            if (row.at(0)->y() == currentY + SQUARE_SIDE){
                canBeMoved = false;
                break;
            }
        }

        if (canBeMoved){
            for (QGraphicsRectItem* square: rowToMove){
                square->moveBy(0, static_cast<qreal>(SQUARE_SIDE));
            }
            movedRows = true;
        }
    }
    //return true when rows were moved down to re-enter the method to check if
    //this allows other rows to be moved
    return movedRows;
}

void MainWindow::generate_tetromino()
{
    QPen blackPen(Qt::black);
    std::shared_ptr<Tetromino> newTetromino(new Tetromino);
    int tetrominoNumber = distr(randomEng);

    //initialize isSquare as false, and change it to true if necessary
    isSquare_ = false;

    //each type of tetromino is generated so that a suitable rotation axis for
    // rotating the tetromino is found at position 1 in currentTetromino_
    //each block is first generated at point (0,0) and then moved to desired
    // location
    if (tetrominoNumber == HORIZONTAL){
        QBrush cyanBrush(Qt::cyan);

        qreal y = 0;
        for (qreal x = 4 * SQUARE_SIDE; x <= 7 * SQUARE_SIDE; x += SQUARE_SIDE){
            QGraphicsRectItem* square = scene_->addRect(0, 0, SQUARE_SIDE,
                                                        SQUARE_SIDE, blackPen,
                                                        cyanBrush);
            square->setX(x);
            square->setY(y);
            newTetromino->push_back(square);
        }
    }
    if (tetrominoNumber == LEFT_CORNER){
        QBrush blueBrush(Qt::darkBlue);
        qreal x = 4 * SQUARE_SIDE;
        qreal y = 0;

        QGraphicsRectItem* square = scene_->addRect(0, 0, SQUARE_SIDE,
                                                    SQUARE_SIDE, blackPen,
                                                    blueBrush);
        square->setX(x);
        square->setY(y);
        newTetromino->push_back(square);

        y += SQUARE_SIDE;
        for (x = 4 * SQUARE_SIDE; x <= 6 * SQUARE_SIDE; x += SQUARE_SIDE){
            QGraphicsRectItem* square = scene_->addRect(0, 0, SQUARE_SIDE,
                                                        SQUARE_SIDE, blackPen,
                                                        blueBrush);
            square->setX(x);
            square->setY(y);
            newTetromino->push_back(square);
        }
    }
    if (tetrominoNumber == RIGHT_CORNER){
        QBrush orangeBrush(QColor(255, 140, 0));
        qreal x = 7 * SQUARE_SIDE;
        qreal y = 0;

        QGraphicsRectItem* square = scene_->addRect(0, 0, SQUARE_SIDE,
                                                    SQUARE_SIDE, blackPen,
                                                    orangeBrush);
        square->setX(x);
        square->setY(y);
        newTetromino->push_back(square);

        y += SQUARE_SIDE;
        for (x = 7 * SQUARE_SIDE; x >= 5 * SQUARE_SIDE; x -= SQUARE_SIDE){
            QGraphicsRectItem* square = scene_->addRect(0, 0, SQUARE_SIDE,
                                                        SQUARE_SIDE, blackPen,
                                                        orangeBrush);
            square->setX(x);
            square->setY(y);
            newTetromino->push_back(square);
        }
    }
    if (tetrominoNumber == SQUARE){
        isSquare_ = true;

        QBrush yellowBrush(Qt::yellow);

        for (qreal y = 0; y <= SQUARE_SIDE; y += SQUARE_SIDE){
            for (qreal x = 5 * SQUARE_SIDE; x < 7 * SQUARE_SIDE; x += SQUARE_SIDE){
                QGraphicsRectItem* square = scene_->addRect(0, 0, SQUARE_SIDE,
                                                            SQUARE_SIDE, blackPen,
                                                            yellowBrush);
                square->setX(x);
                square->setY(y);
                newTetromino->push_back(square);
            }
        }
    }
    if (tetrominoNumber == STEP_UP_RIGHT){
        QBrush greenBrush(Qt::green);

        qreal x = 4 * SQUARE_SIDE;
        for (qreal y = SQUARE_SIDE; y >= 0; y -= SQUARE_SIDE){
            for (int block = 1; block <= 2; ++block){
                QGraphicsRectItem* square = scene_->addRect(0, 0, SQUARE_SIDE,
                                                            SQUARE_SIDE, blackPen,
                                                            greenBrush);
                square->setX(x);
                square->setY(y);
                newTetromino->push_back(square);

                if (block == 1){
                    x += SQUARE_SIDE;
                }
            }
        }
    }
    if (tetrominoNumber == STEP_UP_LEFT){
        QBrush redBrush(Qt::red);

        qreal x = 7 * SQUARE_SIDE;
        for (qreal y = SQUARE_SIDE; y >= 0; y -= SQUARE_SIDE){
            for (int block = 1; block <= 2; ++block){
                QGraphicsRectItem* square = scene_->addRect(0, 0, SQUARE_SIDE,
                                                            SQUARE_SIDE, blackPen,
                                                            redBrush);
                square->setX(x);
                square->setY(y);
                newTetromino->push_back(square);

                if (block == 1){
                    x -= SQUARE_SIDE;
                }
            }
        }
    }
    if (tetrominoNumber == PYRAMID){
        QBrush magentaBrush(Qt::magenta);

        qreal y = SQUARE_SIDE;
        for (qreal x = 5 * SQUARE_SIDE; x <= 7 * SQUARE_SIDE; x += SQUARE_SIDE){
            QGraphicsRectItem* square = scene_->addRect(0, 0, SQUARE_SIDE,
                                                        SQUARE_SIDE, blackPen,
                                                        magentaBrush);
            square->setX(x);
            square->setY(y);
            newTetromino->push_back(square);
        }

        qreal x = 6 * SQUARE_SIDE;
        y -= SQUARE_SIDE;
        QGraphicsRectItem* square = scene_->addRect(0, 0, SQUARE_SIDE,
                                                    SQUARE_SIDE, blackPen,
                                                    magentaBrush);
        square->setX(x);
        square->setY(y);
        newTetromino->push_back(square);
    }

    currentTetromino_ = newTetromino;

    if (game_over()){
        end_game();
    }
}

bool MainWindow::game_over()
{
    //if there are no previously placed squares, it is certain the game
    //is not over
    if (squares_.size() == 0){
        return false;
    } else {
        //iterate over all tetromino parts in current tetromino and check
        //if they overlap previously placed squares
        for (QGraphicsRectItem* currentSquare: *currentTetromino_){
            //current position of tetromino part (square) in question
            qreal current_y = currentSquare->y();
            qreal current_x = currentSquare->x();

            //check if previously placed squares have same coordinates
            //if yes, game is over -> return true
            for (std::vector<QGraphicsRectItem*> row: squares_){
                for (QGraphicsRectItem* square: row){
                    if (square->x() == current_x and square->y() == current_y){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void MainWindow::end_game()
{
    //pause game, tell player that they lost and set reset button as the button
    //from which the player can re-initialize the game
    on_startButton_clicked();
    ui->startButton->setText("Start Game");
    ui->startButton->setDisabled(true);
    ui->gameOverLabel->setText("GAME OVER");

    if (tetrisCount_ > 1){
        ui->tetrisCountLabel->setText("You got a Tetris " +
                                       QString::number(tetrisCount_)+ " times!");
    } else if (tetrisCount_ == 1){
        ui->tetrisCountLabel->setText("You got one Tetris!");
    } else {
        ui->tetrisCountLabel->setText("You did not get a Tetris :(");
    }

    ui->resetButton->setText("New Game");
}
