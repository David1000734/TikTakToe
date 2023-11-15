#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/// @brief Data structure for all tic tac toe boards
struct tik_tak{
    int size;               // size X size board (3 X 3)
    int freeSlots;          // # of available slots
    int numOfPlayers;       // How many players up to 4
    char compSym;           // Symbol for computer
    char usrSym;            // Symbol for player

    char* board[3][3];      // Board
};

char* toString(int);
void initStruct(struct tik_tak*, char, char);
void printBoard(struct tik_tak);
int checkWin(struct tik_tak);
void destructor(struct tik_tak);
int boardInput(struct tik_tak*, int, char);
void playGame(struct tik_tak*);
void usrTurn(struct tik_tak*, char**);
bool checkPlayInput();

/// @brief Function will turn the value from int to string
///
/// @param value number to be changed
///
/// @return give back the string of the number
///
/// @note to go from integer to lowercase char, add 49.
/// Function WILL allocate memory on the heap. Be sure to 
/// deallocate
char* toString(int value) {
    /*              BRUH
    int strCounter = 0;     // string index counter
    int iTemp = value;          // hold value temporarily
    char cTemp;     // char placeholder

    // Capable of holding at most 2 digits, + 1 for null
    char *str = malloc(sizeof(char) * 3);

    // if number is larger than 9, take the leftmost digit
    // and turn that one to a char first and repeat
    while (value > 9) {
        iTemp = value / 10;     // grab left digit
        value %= 10;            // update value
        cTemp = iTemp + 48;     // change to char
        str[strCounter] = cTemp;    // append
        strCounter++;           // increment
    }

    cTemp = value + 48;         // change final digit to char
    str[strCounter] = cTemp;        // char number
    str[strCounter + 1] = '\0';     // null terminated string
    */

    char* str = malloc(sizeof(char) * 3);   // allocate space on heap
    sprintf(str, "%d", value);      // convert to integers

    return str;
}

/// @brief Function will set up all variables in the struct
/// as well as initializing the board
///
/// @param t1 struct to initialize
///
/// @param comp Symbol used by computer
///
/// @param user Symbol used by player
///
/// @note Function will allocate some memory onto the heap.
/// use deconstructor() to deallocate memory
void initStruct(struct tik_tak* t1, char comp, char user) {
    int localTemp = 1;      // used to initialize board

    t1 -> size = 3;     // size of table is always 3 X 3
    t1 -> numOfPlayers = 2;     // Only 2 players
    t1 -> compSym = comp;
    t1 -> usrSym = user;

    // Initilize board
    for (int i = 0; i < t1 -> size; i++) {
        for (int j = 0; j < t1 -> size; j++) {
            t1 -> board[i][j] = toString(localTemp);
            localTemp++;
        }       // inner for, END
    }       // outer for, END

    t1 -> freeSlots = t1 -> size * t1 -> size;
}

/// @brief Function to print the board that is in the struct
///
/// @param temp the struct with the board to print
void printBoard(struct tik_tak temp) {
    // Set up some variables to make print look pretty
    char* header    = " __________ __________ __________ ";
    char* middle    = "|          |          |          |";
    char* inbetween = "|__________|__________|__________|";

    // Formatting done here, print header 
    printf("%s\n", header);
    for (int i = 0; i < temp.size; i++) {
        printf("%s\n%s\n", middle, middle);

        // Print body of the board
        for (int j = 0; j < temp.size; j++) {
            // Print the numbers
            if (j != temp.size - 1) {
                printf("%-5s%-6s", "|", temp.board[i][j]);
            } else {
                printf("%-5s%-6s|", "|", temp.board[i][j]);
            }
        }       // inner for, END

        // Print after the numbers,
        // ending for plot also handled here
        printf("\n%s", middle);
        printf("\n%s\n", inbetween);

    }       // outer for, END

}

/// @brief Function will check for L/R diaginal wins
/// and Vertical and Horizontal wins
///
/// @param temp Pass the struct in for function to use
///
/// @return return the player that won the gam
int checkWin(struct tik_tak temp) {
    bool win = false;       // Found a win
    int winingPlayer = 0;

    for (int i = 0; i < temp.size; i++) {
        for (int j = 0; j < temp.size; j++) {
            // Grab only the character from the board
            char charTemp = temp.board[i][j][0];

            // if it is a number, then it is ocupied
            if (!isdigit(charTemp)) {
                // Don't check outer rims of the board,
                // no diaginal wins for that particular spot
                if ((i > 0 && i < temp.size -1) &&
                    (j > 0 && j < temp.size - 1)) {
                    // Check for left diaginal, Only look at char
                    if ((temp.board[i - 1][j - 1][0] == charTemp) &&
                        (temp.board[i + 1][j + 1][0] == charTemp)) {
                        // Left diaginal win
                        win = true;

                    // Check right diaginal
                    } else if ((temp.board[i - 1][j + 1][0] == charTemp) &&
                        (temp.board[i + 1][j - 1][0] == charTemp)) {
                        // Right diaginal win
                        win = true;

                    } else {}       // Left/Right diaginal wins
                }       // Left/Right diagonal checks, END

                // Check for vertical & horizontal wins
                // Only check middle value for respective wins
                if (i > 0 && i < temp.size - 1) {
                    if ((temp.board[i - 1][j][0] == charTemp) &&
                        (temp.board[i + 1][j][0] == charTemp)) {
                        // vertical win
                        win = true;
                    }
                }

                if (j > 0 && j < temp.size - 1) {
                     if ((temp.board[i][j - 1][0] == charTemp) &&
                        (temp.board[i][j + 1][0] == charTemp)) {
                        // horizontal win
                        win = true;
                    } else {}       // Vertical/Horizontal wins
                }
            }       // outer if, END

            if (win) {
                // Winner found
                winingPlayer = ((temp.freeSlots + 1) % temp.numOfPlayers) + 1;

                // break loop
                i = temp.size;
                j = temp.size;
            }
        }       // inner for, END
    }       // outer for, END

    return winingPlayer;
}

/// @brief Function that should be ran to clean up
/// the struct and free memory from the heap
///
/// @param temp struct to clear memory from
void destructor(struct tik_tak temp) {
    // Unallocate memory
    for (int i = 0; i < temp.size; i++) {
        for (int j = 0; j < temp.size; j++) {
            free(temp.board[i][j]);
        }       // inner for, END
    }       // outer for, END

}

/// @brief Function would insert symbols into the array
///
/// @param t1 struct with the board
///
/// @param num the spot to place into the array
///
/// @param sym The symbol to place into the array
///
/// @return return true if it was able to do so, false otherwise.
///
/// @note Row can be figured out by num % with size
/// Ex. 8 % 3  --> 2
// Column: subtracting the product of the previous result minus 1
// Ex. 3 * 2 = 6
// 8 - 6 = 2
// 2 - 1 = 1
int boardInput(struct tik_tak* t1, int num, char sym) {
    int status = -1;     // check if attempt was valid

    int row = num % t1 -> size;
    int column = num - (row * t1 -> size) - 1;

    // row and column is found, check if that spot
    // is already taken
    if (num < 1 || num > t1 -> size * t1 -> size) {
        // if the value is outside the array
        status = -3;
    } else if (isdigit(t1 -> board[row][column][0])) {
        // Is a digit, so valid spot
        t1 -> board[row][column][0] = sym;
        t1 -> board[row][column][1] = '\0';
        t1 -> freeSlots--;
        status = 0;
    }

    return status;
}

/// @brief Play a round of the game
///
/// @param t1 Pass in struct for function to use
void playGame(struct tik_tak* t1) {
    char* tempStr = "\0";

    printf("%50s\n%53s\n", "Welcome to Tic Tac Toe!!!",
           "First game will be standard.");
    printf("Computer Symbol: %c\nPlayer Symbol: %c\n",
            t1 -> compSym, t1 -> usrSym);

    while (t1 -> freeSlots > 0) {

        // Used to simulate turns
        if (t1 -> freeSlots % t1 -> numOfPlayers == 0) {
            // Player1's turn
            usrTurn(t1, &tempStr);
        } else {
            // player2's Turn
            usrTurn(t1, &tempStr);
        }

        switch (checkWin(t1[0])) {
            case 1:
                tempStr = "Player 1 Won!";
                t1 -> freeSlots = -1;    // break loop
            break;

            case 2:
                tempStr = "Player 2 Won!";
                t1 -> freeSlots = -2;    // break loop
            break;
    
            case 3:
                tempStr = "Player 3 Won!";
                t1 -> freeSlots = -3;    // break loop
            break;

            case 4:
                tempStr = "Player 4 Won!";
                t1 -> freeSlots = -4;    // break loop
            break;

            default:
            break;
        }
    }
    printf("%s\n", tempStr);        // print winner, if present
}

/// @brief Function will play a turn from a player
///
/// @param t1 Pass in struct
///
/// @param str Output the error message if present
void usrTurn(struct tik_tak* t1, char** str) {
    char symbolTemp = t1 -> compSym;
    int input = 0;
    int temp = 0;

    printBoard(t1[0]);

    if (t1 -> freeSlots %  t1 -> numOfPlayers == 0) {
        symbolTemp = t1 -> usrSym;
        printf("Player 1's turn: %c\n", symbolTemp);
    } else {
        printf("Player 2's turn: %c\n", symbolTemp);
    }

    printf("%s\nPlease enter a number corresponding to the board "
          "to make a move.\nPlease enter a number: ", str[0]);
    // Check if input failed
    if (scanf("%d", &input) == 1) {
        printf("Number you entered is: %d\n", input);

        temp = boardInput(t1, input, symbolTemp);

    } else {
        int c;

        // getchar will clear the buffer
        while ((c = getchar() != '\n' && c != EOF)) {}
        temp = -2;
    }

    // Big old switch statement to check if input was valid
    switch (temp) {
        case 0:
            // Input is valid, reset string
            str[0] = "\0";
        break;

        case -1:
            str[0] = "Spot is already taken, choose another.";
        break;

        case -2:
            str[0] = "Letters/Symbols will not be accepted.";
        break;

        case -3:
            str[0] = "Value is outside the array.";
        break;

        default:
            str[0] = "Something happened...";
        break;
    }
    
}

/// @brief Function will check if the input was valid
///
/// @return return true if input is y, yes, n or no
bool checkPlayInput() {
    char input[20];
    bool valid = false;
    bool status = false;

    while (!valid) {
        printf("Would you like to play again? yes/no\n");
        scanf("%s", input);

        // lowercase all letters
        for (int i = 0; input[i]; i++) {
            input[i] = tolower(input[i]);
        }

        if (strcmp(input, "y") == 0 || strcmp(input, "yes") == 0) {
            valid = true;
            status = true;
        } else if (strcmp(input, "n") == 0 || strcmp(input, "no") == 0) {
            valid = true;
        } else {
            printf("Please enter yes or no.\n");
        }
    }

    return status;
}


