#include "cfgfile.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_AddOne_WithPositive_AddsOne(void) {
    int result = cfgfile_add_one(7);
    TEST_ASSERT_EQUAL_MESSAGE(8, result, "Adds one");
}
