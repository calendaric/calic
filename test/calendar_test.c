#include <unity.h>
#include <time.h>
#include <calendar.h>

void setUp(void) {

}

void tearDown(void) {

}

void test_August2019(void) {
    time_t     now = time(0);
    struct tm  timeinfo;
    timeinfo = *localtime(&now);
    timeinfo.tm_mon = 7;
    timeinfo.tm_wday = 4;
    timeinfo.tm_mday = 1;
    timeinfo.tm_year = 112;
    const int8_t expected[6][7] = {
        { 29, 30, 31,  1,  2,  3,  4 },
        {  5,  6,  7,  8,  9, 10, 11 },
        { 12, 13, 14, 15, 16, 17, 18 },
        { 19, 20, 21, 22, 23, 24, 25 },
        { 26, 27, 28, 29, 30, 31,  1 },
        {  2,  3,  4,  5,  6,  7,  8 }
    };
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, createCalendar(&timeinfo).week, sizeof(expected));
}

void test_Febrary2020(void) {
    time_t     now = time(0);
    struct tm  timeinfo;
    timeinfo = *localtime(&now);
    timeinfo.tm_mon = 1;
    timeinfo.tm_wday = 6;
    timeinfo.tm_mday = 1;
    timeinfo.tm_year = 120;
    const int8_t expected[6][7] = {
        { 27, 28, 29, 30, 31,  1,  2 },
        {  3,  4,  5,  6,  7,  8,  9 },
        { 10, 11, 12, 13, 14, 15, 16 },
        { 17, 18, 19, 20, 21, 22, 23 },
        { 24, 25, 26, 27, 28, 29,  1 },
        {  2,  3,  4,  5,  6,  7,  8 },
    };
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, createCalendar(&timeinfo).week, sizeof(expected));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_August2019);
    RUN_TEST(test_Febrary2020);

    return UNITY_END();
}
