#include "board.h"
#include "display.h"
#include "state.h"

#include <assert.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
        init_ncurses();

        GlobalState glob;
        init_global_state_struct(&glob);

        int seed = time(NULL);
        srand(seed);

        state_machine(&glob);

        return 0;
}
