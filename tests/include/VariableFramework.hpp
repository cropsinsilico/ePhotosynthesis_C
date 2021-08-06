#include <random>
#include "Variables.hpp"
#include "gtest/gtest.h"

#define COMPARE(x, y) EXPECT_EQ(x.size(), y.size());\
    for (size_t i = 0; i < x.size(); i++) \
        EXPECT_DOUBLE_EQ(x[i], y[i]);

class VariableFramework: public testing::Test {
protected:
    void SetUp() override {
        if (theVars == nullptr)
            theVars = new Variables();
    }

    void TearDown() override {
        if (theVars != nullptr) {
            delete theVars;
            theVars = nullptr;
        }
    }

    Variables* theVars = nullptr;
};

inline arr get_random(const size_t size) {
    arr values;
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(0, 10);

    for (size_t i = 0; i < size; i++)
        values.emplace_back(dis(e));
    return values;
}
