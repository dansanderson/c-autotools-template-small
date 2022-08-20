#include "executor.h"
#include "mock_cfgfile.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_DoMath_WithPositive_Works(void) {
    // (Intentionally mock the wrong answer to demonstrate that the mock is
    // being called:)
    cfgfile_add_one_ExpectAndReturn(7, 1000);
    int result = executor_do_math(7);
    TEST_ASSERT_EQUAL_MESSAGE(7000, result, "Does math");
}
