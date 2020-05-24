/* Game15 (or puzzle 15)
 *
 * Class: Board
 *
 * Program author
 * Name: Mauri Leino
 * Student number: 274411
 * UserID: leino23
 * E-Mail: mauri.leino@tuni.fi
 *
 * Implementation of Board object describing two dimensional grid for puzzle 15
 * with necessary functions
 *
 * */

#include "./board.hh"
#include <iostream>
#include <iomanip>
#include <random>

const unsigned int PRINT_WIDTH = 5;

Board::Board(std::vector<std::vector<unsigned int>> empty_grid): grid_(empty_grid){
}

void Board::print(){
    for (unsigned int y = 0; y < SIZE; ++y){
        // Print set of lines on top and bottom of gameboard, and between rows
        std::cout << std::string(PRINT_WIDTH * SIZE + 1, '-') << std::endl;
        // Print a vertical line in the beginning each row, and between pieces
        for (unsigned int x = 0; x < SIZE; ++x){
            std::cout << "|" << std::setw(PRINT_WIDTH - 1);

            if (grid_.at(y).at(x) != EMPTY){
                std::cout << grid_.at(y).at(x);
            } else {
                std::cout << ".";
            }
        }
        std::cout << "|" << std::endl;
    }
    std::cout << std::string(PRINT_WIDTH * SIZE + 1, '-') << std::endl;
}

void Board::init_board_random(int seed){
    
    //Save numbers 1-16 in a vector and shuffle them by calling my_shuffle
    std::vector<unsigned int> numbers;
    for (int i = 1; i <= 16; i++){
        numbers.push_back(i);
    }
    my_shuffle(numbers, seed);

    std::vector<std::vector<unsigned int>> gameboard;
    dimension_conversion(numbers, gameboard, "1d2d");

    //Initialize the attribute grid with the gameboard vector
    grid_ = gameboard;
}

void Board::init_board_user(std::vector<unsigned int> numbers){
    std::vector<std::vector<unsigned int>> gameboard;
    dimension_conversion(numbers, gameboard, "1d2d");
    grid_ = gameboard;
}


bool Board::can_be_moved(std::string direction, unsigned int piece){
    std::vector<std::vector<unsigned int>>::size_type x;
    std::vector<std::vector<unsigned int>>::size_type y;

    find_piece(y, x, piece);

    if (direction == "a"){
        if (x == 0 or grid_.at(y).at(x - 1) != EMPTY){
            return false;
        }
    }

    else if (direction == "w"){
        if (y == 0 or grid_.at(y - 1).at(x) != EMPTY){
            return false;
        }
    }

    else if (direction == "s"){
        if (y == (SIZE - 1) or grid_.at(y + 1).at(x) != EMPTY){
            return false;
        }
    }

    else if (direction == "d"){
        if (x == (SIZE - 1) or grid_.at(y).at(x + 1) != EMPTY){
            return false;
        }
    }

    return true;
}


void Board::move_piece(std::string direction, unsigned int piece){
    std::vector<std::vector<unsigned int>>::size_type x;
    std::vector<std::vector<unsigned int>>::size_type y;
    find_piece(y, x, piece);

    if (direction == "a"){
        grid_.at(y).at(x - 1) = piece;
        grid_.at(y).at(x) = EMPTY;
    }
    if (direction == "w"){
        grid_.at(y - 1).at(x) = piece;
        grid_.at(y).at(x) = EMPTY;
    }
    if (direction == "s"){
        grid_.at(y + 1).at(x) = piece;
        grid_.at(y).at(x) = EMPTY;
    }
    if (direction == "d"){
        grid_.at(y).at(x + 1) = piece;
        grid_.at(y).at(x) = EMPTY;
    }
}

bool Board::has_won(){
    std::vector<std::vector<unsigned int>> gameboard = grid_;
    std::vector<unsigned int> game_numbers;
    dimension_conversion(game_numbers, gameboard, "2d1d");

    int inversions_count = inversions(game_numbers);
    if (inversions_count == 0){
        return true;
    }
    return false;
}


bool Board::is_solvable(){
    std::vector<std::vector<unsigned int>>::size_type x;
    std::vector<std::vector<unsigned int>>::size_type y;
    find_piece(y, x, EMPTY);

    //y-coordinate of the empty piece saved in a separate variable to help
    //moving it back to its place
    std::vector<std::vector<unsigned int>>::size_type empty_start_y = y;

    //Move empty piece to bottom of the grid
    while (y < (SIZE - 1)){
        move_piece("w", grid_.at(y + 1).at(x));
        find_piece(y, x, EMPTY);
    }

    std::vector<unsigned int> numbers;
    dimension_conversion(numbers, grid_, "2d1d");

    int inversion_count = inversions(numbers);

    //If empty spot is not in right border, substract inversions caused by the
    //empty spot having value of 16
    if (x != (SIZE - 1)){
        inversion_count -= (SIZE - 1) - x;
    }

    if ((inversion_count % 2) != 0){
        return false;
    }

    //Move empty piece back to its initial place
    while (y != empty_start_y){
        move_piece("s", grid_.at(y - 1).at(x));
        find_piece(y, x, EMPTY);
    }

    return true;
}


void Board::my_shuffle(std::vector<unsigned int>& numbers, const int seed){
    std::default_random_engine random_eng(seed);
    std::uniform_int_distribution<int> distr(0, numbers.size() - 1);

    //For every index on vector, generate random index and switch their values
    //with each other
    for (unsigned int i = 0; i < numbers.size(); ++i){
        unsigned int random_index = distr(random_eng);
        unsigned int temp = numbers.at(i);
        numbers.at(i) = numbers.at(random_index);
        numbers.at(random_index) = temp;
    }
}

void Board::dimension_conversion(std::vector<unsigned int>& numbers,
                              std::vector<std::vector<unsigned int>>& gameboard,
                              std::string conversion){
    if (conversion == "1d2d"){
        //Index to be used to retrieve correct number from vector
        std::vector<unsigned int>::size_type index = 0;

        //Save four vectors representing rows to vector
        //representing full gameboard
        for (int y = 0; y < SIZE; ++y){

                //Save 4 numbers from vector in a vector
                //representing a row at the gameboard
                std::vector<unsigned int> row;
                for (int x = 0; x < SIZE; ++x){
                    row.push_back(numbers.at(index));
                    index++;
                }
                gameboard.push_back(row);
        }
    }

    else if (conversion == "2d1d"){

        //Go through all "rows" of two dimensional vector representing puzzle grid
        for (std::vector<unsigned int>::size_type y = 0; y < SIZE; ++y){

            //Add each number on the row to the back of the 1 dimensional vector
            for (std::vector<std::vector<unsigned int>>::size_type x = 0;
                 x < SIZE; ++x){
                numbers.push_back(gameboard.at(y).at(x));
            }
        }
    }
}

void Board::find_piece(std::vector<std::vector<unsigned int>>::size_type& y,
                       std::vector<std::vector<unsigned int>>::size_type& x,
                       unsigned int piece){
    bool found = false;
    while (not found){

        //Go through all vectors representing rows of game grid
        for (y = 0; y < SIZE; y++){

            //Go through all values on row. If desired number
            //is found, break the loop
            for (x = 0; x < SIZE; x++){
                if (grid_.at(y).at(x) == piece){
                    found = true;
                    break;
                }
            }
            //If desired number is found, break the
            //loop going through row-vectors
            if (found == true){
                break;
            }
        }
    }
}

int Board::inversions(std::vector<unsigned int> numbers){
    std::vector<unsigned int>::size_type index;
    int inversions = 0;

    //Go through all numbers in vector and one save
    //them one at a time in variable "number"
    for (index = 0; index < 15; index++){
        unsigned int number = numbers.at(index);
        std::vector<unsigned int>::size_type secondary_index;

        //Go through numbers after initial number and
        //if smaller numbers are found, increase number of inversions found
        for (secondary_index = (index + 1); secondary_index < 16;
             secondary_index++){
            if (numbers.at(secondary_index) < number){
                inversions++;
            }
        }
    }
    return inversions;
}







