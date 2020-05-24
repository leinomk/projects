/* Game15:
 *
 * Desc:
 *  This program generates a 15 puzzle. The board is 4 x 4, and every round the player chooses a direction and
 * a number tile, making the given tile move to the given direction.
 *  Game will end when the numbers 1-15 (1-16) are in ascending order,
 * starting from top left corner and proceeding row by row.
 *  Empty space is represented by the number 16 in the code (constant EMPTY)
 * and by dot when printing the game board.
 *  When printing the gameboard, for rows and columns that have number higher than ten, only the remainder of division
 * by ten is shown.
 *
 * Program author
 * Name: Mauri Leino
 * E-Mail: mauri.leino@tuni.fi
 *
 * */

#include "board.hh"
#include <random>
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <time.h>

const std::string DIRECTIONS = "awsd";

Board create_board(){
   /* Create a to dimensional vector representing empty game grid with number 0
    * in each place, and use it as a prameter to call gameboard constructor, to
    * create Board-object with an empty grid.
    * Return Board-object with empty grid
    */

    std::vector<std::vector<unsigned int>> empty_grid;

    //Create four vectors representing rows of puzzle
    //grid and add them to 2 dim vector empty_grid
    for (int y = 0; y < SIZE; ++y){
            std::vector<unsigned int> row;

            //Add four zeros in each row
            for (int x = 0; x < SIZE; ++x){
                row.push_back(0);
            }
            empty_grid.push_back(row);
    }

    Board board(empty_grid);
    return board;
}

void init_board_random(Board& board){
    /* Call random board initialization method init_board_random for board
     * object received as parameter, with seed given by user as parameter, or if
     * seed is not given, use value from computer clock as parameter
     */

    std::string seed_value;
    std::cout << "Enter a seed value (or an empty line): ";
    std::cin.ignore();
    std::getline(std::cin, seed_value);

    int seed;

    if (seed_value == ""){
        //If the user did not give a seed value, use value from computer clock
        //as the seed value
        seed = time(NULL);
    } else {
        //If the user gave a seed value, use it
        seed = stoi(seed_value);
    }

    board.init_board_random(seed);

}

bool valid_gameboard_input(std::vector<unsigned int> user_input){
    /* Check that the user input for gameboard contains every number 1-16
     * If a number or numbers are missing, call out the lowest not found and
     * return false to exit program
     * If all numbers are found, return true
     */

    std::vector<unsigned int>::size_type index;

    // Go through numbers 1-16 to look for in input vector
    for (unsigned int i = 1; i <= 16; i++){

        // Go through vector and see if number i can be found
        for (index = 0; index < 16; index++){

            // If the number is found, break current loop to move on
            if (user_input.at(index) == i){
                break;
            }

            //Print first number i that is not found and return false
            if (index == 15 and user_input.at(index) != i){
                std::cout << "Number " << i << " is missing" << std::endl;
                return false;
            }
        }
    }
    return true;
}


bool init_board_user(Board& board){
    /* Ask user input for gameboard initialization and store input in a vector
     * Call valid_gameboard_input to check that input is valid
     * If input is invalid, return false to exit program. Else, call board
     * initialization method init_board_user for board object
     * received as parameter. After this, return true
     */

    std::cout << "Enter the numbers 1-16 in a desired order (16 means empty):"
              << std::endl;
    std::vector<unsigned int> input_numbers;
    unsigned int new_integer = 0;

    //Wait until user has given 16 numbers, or if more are given
    //separated with a space, take in account the first 16
    for (int i = 0; i < 16; i++){
        std::cin >> new_integer;
        input_numbers.push_back(new_integer);
    }

    if (not valid_gameboard_input(input_numbers)){
        return false;
    }

    board.init_board_user(input_numbers);

    return true;
}



bool read_command_successfully(std::string& command, unsigned int& piece,
                               Board& gameboard){
    /* Function reads move command input from user and checks that it is valid
     * and receives command and number as reference parameters to change them
     * If input is valid, return true
     * If input is invalid, user is informed what went wrong,
     * and new command is asked for
     * If input is "q", return false to exit program
     */

    while (true){
        std::cout << "Dir (command, number): ";
        std::cin >> command;

        if (command == "q"){
            return false;
        }

        std::cin >> piece;

        bool impossible_direction = false;

        // If direction user has given is found in constant string containing
        // possible directions and the number given is a piece on board
        if ((DIRECTIONS.find(command) != std::string::npos) and
                (0 < piece and piece < EMPTY)){
            if (gameboard.can_be_moved(command, piece)){
                return true;
            }
            //Direction and number are otherwise valid, but given piece
            //cannot be moved in desired direction
            else {
                std::cout << "Impossible direction: " << command << std::endl;
                impossible_direction = true;
            }
        }

        // Direction given is invalid
        if (DIRECTIONS.find(command) == std::string::npos){
            std::cout << "Unknown command: " << command << std::endl;
        }
        // Direction is valid, but number given is invalid
        else {
            if (not impossible_direction){
                std::cout << "Invalid number: " << piece << std::endl;
            }
        }
        gameboard.print();
    }
}

bool has_won(Board& gameboard){
    /* Receive gameboard object as parameter, and caal its method has_won
     * If user has won, return true to exit program
     * Else, return false to continue program
     */

    if (gameboard.has_won()){
        std::cout << "You won!" << std::endl;
        return true;
    }
    return false;
}


void init_method(std::string& random_init){
    /* Ask user whether to initialize ranodm gameboard or given by user
    * If input is invalid, ask again until valid input is received
    * Return initialization method, either 'y' for random
    * or 'n' for user initialization
    */


    //Infinite for-loop to continue until valid initialization method is given
    for (;;){
        std::cout << "Random initialization (y/n): ";
        std::cin >> random_init;
        if (random_init == "y" or random_init == "n"){
            break;
        }
        else if (random_init == "Y"){
            random_init = "y";
            break;
        }
        else if (random_init == "N"){
            random_init = "n";
        }
        else {
            std::cout << "Unknown choice: " << random_init << std::endl;
        }
    }
}


int main(){
    Board gameboard = create_board();

    //Describes whether random initialization is used or not
    std::string random_init;
    init_method(random_init);

    if (random_init == "y"){
        init_board_random(gameboard);
    }

    else if (random_init == "n"){
        //User tried to initialize invalid gameboard, i.e. not containing
        //all numbers 1-16 once
        if (not init_board_user(gameboard)){
            return EXIT_FAILURE;
        }
    }

    if (not gameboard.is_solvable()){
        std::cout << "Game is not solvable. What a pity." << std::endl;
        return EXIT_SUCCESS;
    } else {
        std::cout << "Game is solvable: Go ahead!" << std::endl;
    }

    gameboard.print();

    std::string command = "";
    unsigned int piece = 0;

    while (not has_won(gameboard) and
           read_command_successfully(command, piece, gameboard)){
        gameboard.move_piece(command, piece);
        gameboard.print();
    }

    return EXIT_SUCCESS;
}
