#pragma once

#include <random>
#include <map>
#include "gtest/gtest.h"
#include "Variables.hpp"
#include "globals.hpp"

#define COMPARE(x, y) EXPECT_EQ(x.size(), y.size());\
    for (size_t i = 0; i < x.size(); i++) \
        EXPECT_DOUBLE_EQ(x[i], y[i]);

#define DIFF(x, y) {EXPECT_EQ(x.size(), y.size());\
    bool diff = false;\
    for (size_t i = 0; i < x.size(); i++) \
        if (abs(x[i] - y[i]) > 1e-7)\
            diff = true;\
    EXPECT_TRUE(diff);}

namespace ePhotosynthesis {
namespace test {

const std::map<std::string, double> Emap = {{"V1", 120},
{"V2", 1241.24},
{"V3", 166.35},
{"V5", 50.2},
{"V6", 29.91},
{"V7", 128.58},
{"V8", 50.2},
{"V9", 13.35},
{"V10", 128.57},
{"V13", 446.19},
{"V23", 8.01},
{"V16", 150},
{"V112", 1572.6},
{"V113", 171.47},
{"V121", 43.68},
{"V122", 99.19},
{"V123", 300.29},
{"V124", 82.37},
{"V131", 74.84},
{"V51", 3.22},
{"V52", 1.92},
{"V55", 3.46},
{"V56", 1.67},
{"V57", 16.65},
{"V58", 0.5},
{"V59", 3.03},
{"Jmax", 180.0},
{"K1", 1000000.0},
{"K2", 500.0},
{"K3", 50000000.0},
{"K4", 50000000.0},
{"K5", 50000000.0},
{"K6", 50000000.0},
{"K7", 10000.0},
{"K8", 1000.0},
{"K9", 8300000.0},
{"K10", 800000000.0},
{"Vmax11", 6},
{"Kau", 10000000000.0},
{"Kua", 10000000000.0},
{"Kf", 6300000.0},
{"Kd",  200000000.0},
{"K15", 10000000000.0},
{"K16", 100000.0},
{"V2M", 27.8},
{"kA_d", 200000000.0},
{"kA_f", 1260000.0},
{"kA_U", 10000000000.0},
{"kU_A", 10000000000.0},
{"kU_d", 200000000.0},
{"kU_f", 1260000.0},
{"k1", 2.5E+11},
{"k_r1", 300000000.0},
{"kz", 5000000.0},
{"k12", 30000.0},
{"k23", 10000.0},
{"k30", 3000.0},
{"k01", 500.0},
{"k2", 2000000000.0},
{"kAB1", 2500.0},
{"kBA1", 200.0},
{"kAB2", 3300.0},
{"kBA2", 250.0},
{"k3", 800.0},
{"k_r3", 80.0},
{"k_pq_oxy", 500.0}};


class VariableFramework: public testing::Test {
protected:
    void SetUp() override {
        if (theVars == nullptr) {
#ifdef SUNDIALS_CONTEXT_REQUIRED
	    if (SUNContext_Create(0, &context) < 0) {
	      throw std::runtime_error("SUNContext_Create failed");
	    }
            theVars = new Variables(&context);
#else // SUNDIALS_CONTEXT_REQUIRED
            theVars = new Variables();
#endif // SUNDIALS_CONTEXT_REQUIRED
	}
        IniModelCom(theVars);
    }

    void TearDown() override {
        if (theVars != nullptr) {
            delete theVars;
            theVars = nullptr;
#ifdef SUNDIALS_CONTEXT_REQUIRED
	    SUNContext_Free(&context);
#endif // SUNDIALS_CONTEXT_REQUIRED
        }
    }

    Variables* theVars = nullptr;
#ifdef SUNDIALS_CONTEXT_REQUIRED
    SUNContext context = nullptr;
#endif // SUNDIALS_CONTEXT_REQUIRED
};

inline arr get_random(const size_t size) {
    arr values;
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(0, 10);

    for (size_t i = 0; i < size; i++)
        values.emplace_back(dis(e));
    return values;
}

template<class T, class U, class V, class W>
class CombinedTest : public virtual T, public virtual U {
protected:
    void SetUp() override {
        T::SetUp();
        U::SetUp();
    }

    V* MB_con(const double t, const V* conditions, Variables* theVars) {
        return W::_MB_con(t, conditions, theVars);
    }
    arr MB(const double t, const V* conditions, Variables* theVars) {
        return W::_MB(t, conditions, theVars);
    }
};
}  // namespace test
}  // namspace ePhotosynthesis

