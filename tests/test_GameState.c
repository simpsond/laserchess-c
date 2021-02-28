#include <stdlib.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>
#include "../utils.h"
#include "../GameState.h"

void test_loadGameStateFileMP(void **state) {
    GameState* gs = createGameState();
    loadGameStateFileMP(gs, "data/startstate.mpack");

    assert_int_equal(gs->activePlayer, PLAYER1);
    freeGameState(gs);
}

const struct CMUnitTest gamestate_tests[] = {
        cmocka_unit_test(test_loadGameStateFileMP),
};

int main(void)
{
    return cmocka_run_group_tests(gamestate_tests, NULL, NULL);
}
