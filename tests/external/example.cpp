#include "ePhotosynthesis.hpp"


using namespace ePhotosynthesis;

int main(int argc, const char* argv[]) {
    try {
        double stoptime = 5000.0, begintime = 0.0, stepsize = 1.0;
        double abstol = 1e-5, reltol = 1e-4;
        int maxSubSteps = 750;

        std::map<std::string, std::string> inputs;
        inputs["RUBISCOMETHOD"] = "2";
        Variables *theVars = new Variables();
        std::string evn("data/InputEvn.txt");
        std::string atpcost("data/InputATPCost.txt");
        std::string enzyme("data/InputEnzyme.txt");
        std::string grn("data/InputGRNC.txt");

        std::cerr << "evn: " << evn << std::endl;
        std::cerr << "atpcost: " << atpcost << std::endl;
        std::cerr << "enzyme: " << enzyme << std::endl;
        std::cerr << "grn: " << grn << std::endl;

        run_simulation(4, begintime, stoptime, stepsize, maxSubSteps,
                       abstol, reltol, inputs,
                       true, 0, false, false, false,
                       evn, atpcost, enzyme, grn,
                       "example_output.data");

        return (EXIT_SUCCESS);
    } catch (std::exception& e) {
        std::cout << "An error occurred: " << e.what() << std:: endl;
        exit(EXIT_FAILURE);
    }
}
