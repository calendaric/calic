#include <unity.h>

#include <calendar.h>

void setUp(void) {

}

void tearDown(void) {

}

void test_november() {
    TEST_ASSERT_EQUAL_HEX8(40, 40);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_november);
    
    return UNITY_END();
}
