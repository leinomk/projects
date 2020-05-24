/* Game15 (or puzzle 15)
 *
 * Class: Board
 *  Describes a two-dimensional grid for puzzle 15 with necessary functions.
 *
 * Program author
 * Name: Mauri Leino
 * Student number: 274411
 * UserID: leino23
 * E-Mail: mauri.leino@tuni.fi
 *
 * Notes:
 *  Methods can_be_moved, has_won and is_solvable could be better in private
 *  interface and called by other public methods
 *
 * */

#ifndef BOARD_HH
#define BOARD_HH
#include <vector>
#include <string>

const int SIZE = 4;
const int EMPTY = 16;

class Board
{
public:

    /* One constructor for both initializing methods. Constructor recieves two
     * dimensional vector representing
     * empty game grid and initializes it in attribute grid_
     */
    Board(std::vector<std::vector<unsigned int>> gamegrid);


    // Prints the game grid
    void print();


    /* Initialize random gameboard by storing numbers 1-16 in a vector,
     * shuffling them calling my_shuffle-method and then calling
     * dimension_conversion to receive two dimensional vector representing
     * puzzle grid and initializing attribute grid_
     */
    void init_board_random(int seed);


    /* Initialize gameboard using user input received as parameter, by calling
     * dimension_convert-method to receive two dimensional vector representing
     * puzzle grid and initializing attribute grid_
     */
    void init_board_user(std::vector<unsigned int> numbers);


    /* Check that the piece that user wants to move is movable, i.e. user tries
     * not to move a piece off the board and empty place is in the direction
     * where user wants to move the piece
     * Receive desired direction and piece
     * Return true if piece can be moved, else, return false
     */
    bool can_be_moved(std::string direction, unsigned int piece);


    /* Move piece to desired direction, both received as parameters, by
     * switching places of desired piece and empty spot
     */
    void move_piece(std::string direction, unsigned int piece);


    /* Check if user has won by finding out number of inversions in grid
     * If number of inversions is zero, user has won and return true
     * Else, return false.
     */
    bool has_won();


    /* Check that puzzle is solvable by moving empty piece to bottom of the grid
     * and then counting if number of inversions is divisible by two. If it is
     * not, return false to exit program. Else, move empty spot back to initial
     * place and return true.
     */
    bool is_solvable();


private:

    /* Shuffles the numbers 1-16 stored in vector using seed as a seed value for
     * random engine and switching places of two numbers at a time for the
     * length of the vector
     */
    void my_shuffle(std::vector<unsigned int>& numbers, const int seed);


    /* Converts one dimensional vector with puzzle numbers to two dimensional
     * vector representing puzzle grid, or two dimensional vector to one
     * dimensional vector with puzzle numbers
     * Recieves each vector as parameters, and the desired convertion, "1d2d"
     * for one dim to two dim, or "2d1d" for two dim to one dim
     */
    void dimension_conversion(std::vector<unsigned int>& numbers,
                              std::vector<std::vector<unsigned int>>& gameboard,
                              std::string conversion);


    /* Receive y and x coordinates and piece as parameters and find where the
     * piece is on the board, then save coordinates in y and x
     */
    void find_piece(std::vector<std::vector<unsigned int>>::size_type& y,
                    std::vector<std::vector<unsigned int>>::size_type& x,
                    unsigned int piece);


    /* Count number of inversions in vector received as parameters by finding
     * for each number whether there are smaller numbers after it in the vector
     * Return number of inversions
     */
    int inversions(std::vector<unsigned int>);


    // Game grid for the 15 puzzle
    std::vector<std::vector<unsigned int>> grid_;

};

#endif // BOARD_HH
