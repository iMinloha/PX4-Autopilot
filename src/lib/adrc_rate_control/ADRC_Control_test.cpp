#include "ADRC_Control.h"
#include "inc/library.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace matrix;

TEST(ADRCControlTest, AllZeroCase) {
    TD td(100.f, 0.1f);
    td.update(1.f, 0.01f);

    std::cout << "v1: " << td.get_state() << ", v2: " << td.get_derivative() << std::endl;
}
