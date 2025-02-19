#include "ePhotosynthesis.hpp"


using namespace ePhotosynthesis;

int main(int argc, const char* argv[]) {
    try {
        double stoptime = 5000.0, begintime = 0.0, stepsize = 1.0;
        double abstol = 1e-5, reltol = 1e-4;
        int maxSubSteps = 750, RUBISCOMETHOD = 2;

        std::map<std::string, std::string> inputs;
        Variables *theVars = new Variables();

        // Read files
        theVars->readParam("data/InputEvn.txt", inputs);
        theVars->readParam("data/InputATPCost.txt", inputs);
        theVars->readEnzymeAct("data/InputEnzyme.txt");
        theVars->readGRN("data/InputGRNC.txt");

        double Tp = theVars->Tp;
        theVars->record = false;
        theVars->useC3 = true;
        theVars->RUBISCOMETHOD = RUBISCOMETHOD;
        modules::PR::setRUBISCOTOTAL(3);
        
        drivers::Driver *maindriver = new drivers::EPSDriver(
            theVars, begintime, stepsize, stoptime,
            maxSubSteps, abstol, reltol, 1, 1, Tp);

        std::vector<double> ResultRate = maindriver->run();

        std::ofstream outfile("example_output.data");
        outfile << ResultRate[0] << std::endl;
        outfile.close();

        if (theVars != nullptr) {
            maindriver->inputVars = nullptr;
            delete theVars;
        }
        delete maindriver;
        return (EXIT_SUCCESS);
    } catch (std::exception& e) {
        std::cout << "An error occurred: " << e.what() << std:: endl;
        exit(EXIT_FAILURE);
    }
}
