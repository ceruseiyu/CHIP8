#include "bcd.h"
#include "minunit.h"
#include <stdint.h>
#include <stdlib.h>

BCD* bcdRep;
uint8_t binary;

void test_setup() {
    binary = 231;
    bcdRep = getBCD(binary);
}

void test_teardown() {
    free(bcdRep);
}

MU_TEST(bcd_hundreds) {
    mu_check(bcdRep->hundreds == 2);
}

MU_TEST(bcd_tens) {
    mu_check(bcdRep->tens == 3);
}

MU_TEST(bcd_ones) {
    mu_check(bcdRep->ones == 1);
}

MU_TEST_SUITE(bdc_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(bcd_hundreds);
    MU_RUN_TEST(bcd_tens);
    MU_RUN_TEST(bcd_ones);
}

int main() {
    MU_RUN_SUITE(bdc_suite);
    MU_REPORT();
    return 1;
}