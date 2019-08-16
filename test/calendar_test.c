#include <unity.h>

#include <calendar.h>

void setUp(void) {

}

void tearDown(void) {

}

void test_August2019() {
    const int8_t expected[6][7] = {
        { 29, 30, 31,  1,  2,  3,  4 },
        {  5,  6,  7,  8,  9, 10, 11 },
        { 12, 13, 14, 15, 16, 17, 18 },
        { 19, 20, 21, 22, 23, 24, 25 },
        { 26, 27, 28, 29, 30, 31,  1 },
        {  2,  3,  4,  5,  6,  7,  8 }
    };
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, createCalendar().week, sizeof (expected));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_August2019);
    
    return UNITY_END();
}
