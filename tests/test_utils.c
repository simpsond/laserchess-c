//
// Created by Dustin Simpson on 2/3/21.
//
#include <stdlib.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>
#include "../utils.h"

void test_calcAngleVector2(void **state) {
    Vector2 laser_vectors[4] = {
            {0.0f, 1.0f},
            {-1.0f, 0.0f},
            {0.0f, -1.0f},
            {1.0f, 0.0f}
    };

    Vector2 ref_vector = {0.0f, 1.0f};

    for(int i = 0 ; i < 4; i++) {
        int angle = (int)calcAngleVector2(ref_vector, laser_vectors[i]);
        assert_int_equal(i*90,angle);
    }
}

void test_rotate(void **state) {
    Vector2 ref_vector = {0.0f, -1.0f};
    Vector2 origin = {0.0f, 0.0f};

    ref_vector = rotate(ref_vector, origin, 0);
    assert_int_equal((int)ref_vector.x, 0);
    assert_int_equal((int)ref_vector.y, -1);

    ref_vector = rotate(ref_vector, origin, 90);
    assert_int_equal((int)ref_vector.x, 1);
    assert_int_equal((int)ref_vector.y, 0);
}

const struct CMUnitTest util_tests[] = {
        cmocka_unit_test(test_calcAngleVector2),
        cmocka_unit_test(test_rotate),
};

int main(void)
{
    return cmocka_run_group_tests(util_tests, NULL, NULL);
}