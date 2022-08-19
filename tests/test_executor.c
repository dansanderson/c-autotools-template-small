#include "executor.h"
#include "mock_cfgfile.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_DoMath_WithPositive_Works(void) {
    int result = executor_do_math(7);
    TEST_ASSERT_EQUAL_MESSAGE(56, result, "Does math");
}
