#include <stdio.h>
#include <stdlib.h>
#include "tikTak.h"

int main() {
    struct tik_tak tk1;
    bool playAgain = true;

    initStruct(&tk1, 'O', 'X');  // initialize struct

    while (playAgain) {
        playGame(&tk1);
        // play again
        if (checkPlayInput()) {
            destructor(tk1);
            initStruct(&tk1, 'O', 'X');
        } else {
            printf("Thanks for playing! Goodbye.\n");
            playAgain = false;
        }
    }

    destructor(tk1);

    return 0;
}



