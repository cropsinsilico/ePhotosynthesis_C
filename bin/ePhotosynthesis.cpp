/**********************************************************************************************************************************************
 *   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
 *
 * CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * University of Illinois at Urbana Champaign
 * Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.
 *
 * Converted from Matlab to C++ by Douglas N. Friedel, National Center for Supercomputing Applications (2020)
 *
 *   This file is part of e-photosynthesis.
 *
 *    e-photosynthesis is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation;
 *
 *    e-photosynthesis is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License (GPL)
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************************************************************/

#include <string>
#include <map>
#include <vector>

#include "cxxopts.hpp"
#include "globals.hpp"
#include "modules/trDynaPS.hpp"
#include "modules/CM.hpp"
#include "modules/EPS.hpp"
#include "modules/PR.hpp"
#include "modules/BF.hpp"
#include "modules/FI.hpp"
#include "drivers/drivers.hpp"
#include "Variables.hpp"


using namespace ePhotosynthesis;

// macros to get options from either the command line (has precedence) or an options file
#define varSet(xcli, xfile) (result.count(#xcli) || inputs.count(#xfile))
#define fileProvided(xcli, xfile) varSet(xcli, xfile)
#define varSearchD(x)                                   \
    if (result.count(#x) == 0 && inputs.count(#x) > 0)  \
        x = stod(inputs.at(#x), nullptr);
#define varSearchI(x)                                   \
    if (result.count(#x) == 0 && inputs.count(#x) > 0)  \
        x = stoi(inputs.at(#x), nullptr);
#define varSearch(x)                                    \
    if (result.count(#x) == 0 && inputs.count(#x) > 0)  \
        x = inputs.at(#x);

#define displayInputVar(fmt, src, val)				\
    printf("Input variable \"%s\" = " #fmt "\n", #src, val)

#define assignInputVarD(src, dst)                                       \
    if (inputs.count(#src) > 0) {                                       \
        theVars-> dst = static_cast<double>(stof(inputs.at(#src), nullptr)); \
        displayInputVar(%lf, src, theVars-> dst);                       \
    } else								\
        printf("Input variable \"%s\" not set\n", #src)
#define assignInputVarI(src, dst)                                       \
    if (inputs.count(#src) > 0) {                                       \
        theVars-> dst = stoi(inputs.at(#src), nullptr);			\
        displayInputVar(%d, src, theVars-> dst);                        \
    } else								\
        printf("Input variable \"%s\" not set\n", #src)

#define setInputVarB(src, mod, dst)                                     \
    if (inputs.count(#src) > 0) {                                       \
        modules::mod::set ## dst ((bool)(stoi(inputs.at(#src), nullptr))); \
        displayInputVar(%d, src, stoi(inputs.at(#src), nullptr));       \
    } else								\
        printf("Input variable \"%s\" not set\n", #src)


enum DriverType {
    None,
    trDynaPS,
    DynaPS,
    CM,
    EPS
};


int main(int argc, const char* argv[]) {
    try {
        bool record = false;
        bool useC3 = false;
        cxxopts::Options options("ePhoto", "Command line interface to C++ implementation of the matlab original");
        options.show_positional_help();
        std::string evn, atpcost, optionsFile, enzymeFile, grnFile;
        double stoptime, begintime, stepsize;
        double abstol, reltol;
        double Tp;
        DriverType driverChoice;
        int driver, maxSubSteps, RUBISCOMETHOD = 2;
        ushort dbglvl;
        bool debugDelta, debugInternal;
        options.add_options()
                ("v,verbose", "Record output values for all steps (this can significantly slow the program).", cxxopts::value<bool>(record)->default_value("false"))
                ("e,evn", "The file (including path) containing environmental parameters", cxxopts::value<std::string>(evn)->default_value("InputEvn.txt"))
                ("a,atpcost", "The file (including path) containing the ATP cost", cxxopts::value<std::string>(atpcost)->default_value("InputATPCost.txt"))
                ("n,enzyme", "The file (including path) containing enzyme activities like InputEnzyme.txt", cxxopts::value<std::string>(enzymeFile)->default_value(""))
                ("g,grn", "The file (including path) containing protein ratios for relevant genes like InputGRNC.txt",
		 cxxopts::value<std::string>(grnFile)->default_value(""))
                ("b,begintime", "The starting time for the calculations.", cxxopts::value<double>(begintime)->default_value("0.0"))
                ("s,stoptime", "The time to stop calculations.", cxxopts::value<double>(stoptime)->default_value("5000.0"))
                ("z,stepsize", "The step size to use in the calculations.", cxxopts::value<double>(stepsize)->default_value("1.0"))
                ("m,maxSubSteps", "The maximum number of iterations at each time step.", cxxopts::value<int>(maxSubSteps)->default_value("750"))
                ("d,driver", "The driver to use. Choices are:                        1 - trDynaPS: PS, PR, FI, BF, SUCS, RuACT,                 XanCycle, RROEA                                2 - DynaPS: PS, PR, FI, BF, SUCS, XanCycle           3 - CM: PS, PR, SUCS                                 4 - EPS: PS, PR, FI, BF, SUCS                    ", cxxopts::value<int>(driver)->default_value("1"))
                ("c,c3", "Use the C3 model, automatically set to true for EPS driver", cxxopts::value<bool>(useC3)->default_value("false"))
	        ("rubiscomethod", "The method to use for rubisco calculations. Choices are: 1 - Use enzyme concentration for calculation          2 - Use the michaelis menton and enzyme concentration together for calculation",
                 cxxopts::value<int>(RUBISCOMETHOD)->default_value("2"))
                ("t,abstol", "Absolute tolerance for calculations", cxxopts::value<double>(abstol)->default_value("1e-5"))
                ("r,reltol", "Relative tolerance for calculations", cxxopts::value<double>(reltol)->default_value("1e-4"))
                ("T,Tp", "Input Temperature", cxxopts::value<double>(Tp)->default_value("0.0"))
                ("o,options", "Name of a text file which specifies any of the above options. Command line arguments have priority.", cxxopts::value<std::string>(optionsFile)->default_value(""))
                ("h,help", "Produce help message")
                ("debug","Debug level", cxxopts::value<ushort>(dbglvl)->default_value("0"))
                ("debugDelta", "Debug deltas", cxxopts::value<bool>(debugDelta)->default_value("false"))
                ("debugInternal", "Debug internals", cxxopts::value<bool>(debugInternal)->default_value("false"))
                ;

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return (EXIT_SUCCESS);
        }
        std::map<std::string, std::string> inputs;
        if (result.count("options")) {
            readFile(result["options"].as<std::string>(), inputs);

            varSearch(evn)
            varSearch(atpcost)
            varSearch(enzymeFile)
            varSearch(grnFile)
            varSearchD(begintime)
            varSearchD(stoptime)
            varSearchD(stepsize)
            varSearchD(Tp)
            varSearchI(maxSubSteps)
            varSearchI(driver)
            varSearchI(RUBISCOMETHOD)
            varSearchD(abstol)
            varSearchD(reltol)
        }
        driverChoice = static_cast<DriverType>(driver);
        if (driverChoice == EPS)
            useC3 = true;

        if (fileProvided(evn, evn))
          readFile(evn, inputs);
        if (fileProvided(atpcost, atpcost))
          readFile(atpcost, inputs);
#ifdef SUNDIALS_CONTEXT_REQUIRED
        SUNContext context;
        if (SUNContext_Create(0, &context) < 0) {
          std::cout << "SUNContext_Create failed" << std::endl;
          throw std::runtime_error("SUNContext_Create");
        }
        Variables *theVars = new Variables(&context);
#else // SUNDIALS_CONTEXT_REQUIRED
	Variables *theVars = new Variables();
#endif // SUNDIALS_CONTEXT_REQUIRED
        if (fileProvided(enzyme, enzymeFile)) {
            std::cerr << "ENZYME DATA PROVIDED" << std::endl;
            readFile(enzymeFile, theVars->EnzymeAct, true);
        } else {
            if (useC3)
                throw std::runtime_error("Enzyme data required if --c3 set (automatically true for EPS driver)");
        }

        assignInputVarD(CO2, Air_CO2);
        assignInputVarD(Air_CO2, Air_CO2);
        assignInputVarD(PAR, Radiation_PAR);
        assignInputVarD(Radiation_PAR, Radiation_PAR);
        assignInputVarD(ATPCost, TestATPCost);
        if (result.count("Tp") == 0) {
            assignInputVarD(WeatherTemperature, Tp);
            Tp = theVars->Tp;
        }
        assignInputVarI(GRNC, GRNC);
        setInputVarB(SucPath, CM, TestSucPath);

        // Read the GRN data and assign it into the correct positions
        // based on the expected order
        if (fileProvided(grn, grnFile)) {
            std::cerr << "GRN DATA PROVIDED" << std::endl;
            std::map<std::string, double> glymaID_map;
            readFile(grnFile, glymaID_map);
            double pcfactor = 1.0 / 0.973;
            if (inputs.count("ProteinTotalRatio") > 0)
                pcfactor = 1.0 / static_cast<double>(stof(inputs.at("ProteinTotalRatio"), nullptr));
            size_t i = 0;
            for (auto it = glymaID_order.begin(); it != glymaID_order.end(); it ++, i++) {
                double iVfactor = pcfactor;
                try {
                    if ((i >= 33) && (theVars->GRNC == 0))
                        iVfactor = 1.0;
                    else
                        iVfactor = pcfactor * glymaID_map.at(*it);
                } catch (const std::out_of_range&) {
                    // Do nothing
                    printf("GlymaID \"%s\" not present.\n", it->c_str());
                }
                if (i < 33) {
                    theVars->VfactorCp[i] = iVfactor;
                } else if (i == 33) {
                    modules::BF::setcATPsyn(iVfactor);
                } else if (i == 34) {
                    modules::BF::setCPSi(iVfactor);
                } else if (i == 35) {
                    modules::FI::setcpsii(iVfactor);
                } else if (i == 36) {
                    modules::BF::setcNADPHsyn(iVfactor);
                } else {
                    printf("More GlymaIDs than expected.\n");
                    exit(EXIT_FAILURE);
                }
            }
        }

        theVars->Tp = Tp;
        theVars->record = record;
        theVars->useC3 = useC3;
        theVars->RUBISCOMETHOD = RUBISCOMETHOD;
        theVars->CO2_in = theVars->Air_CO2;
        theVars->CO2_cond = theVars->CO2_in / (3. * pow(10., 4.));
        theVars->PPFD_in = theVars->Radiation_PAR * 1.0e6 / 2.35e5;
        theVars->TestLi = theVars->PPFD_in;
        modules::PR::setRUBISCOTOTAL(3);
        if (debugDelta)
            dbglvl += 8;
        if (debugInternal)
            dbglvl += 16;
#ifdef INCDEBUG
        std::cout << "Setting Debug level " << dbglvl << std::endl;
        theVars->debuglevel = static_cast<Debug::RequestedDebug>(dbglvl);
#else
        if (dbglvl != 0)
            std::cout << "This is not a debug build, no debug reporting will be done." << std::endl;
#endif
        drivers::Driver *maindriver;

        //DEBUG_MESSAGE("TESTING",0)
        switch (driverChoice) {
            case trDynaPS:
                maindriver = new drivers::trDynaPSDriver(theVars, begintime, stepsize, stoptime,
                                                         maxSubSteps, abstol, reltol, 1, 1);
                break;
            case DynaPS:
                maindriver = new drivers::DynaPSDriver(theVars, begintime, stepsize, stoptime,
                                                       maxSubSteps, abstol, reltol, 1, 1);
                break;
            case CM:
                maindriver = new drivers::CMDriver(theVars, begintime, stepsize, stoptime,
                                                   maxSubSteps, abstol, reltol);
                break;
            case EPS:
                maindriver = new drivers::EPSDriver(theVars, begintime, stepsize, stoptime,
                                                    maxSubSteps, abstol, reltol, 1, 1, Tp);
                break;
            default:
                printf("Invalid driver choice given.\n");
                exit(EXIT_FAILURE);
        }

        std::vector<double> ResultRate = maindriver->run();

        std::ofstream outfile("output.data");

        switch (driverChoice) {
            case trDynaPS:
                outfile << "Light intensity,Vc,Vo,VPGA,VT3P,Vstarch,Vt_glycerate,Vt_glycolate" << std::endl;
                outfile << theVars->TestLi << "," << ResultRate[0] << ",";
                outfile << ResultRate[1] << "," << ResultRate[2] << "," << ResultRate[3] << ",";
                outfile << ResultRate[4] << "," << ResultRate[5] << "," << ResultRate[6] << std::endl;
                break;
            case DynaPS:
                outfile << "Light intensity,PSIIabs,PSIabs,Vc,Vo,VPGA,Vsucrose,Vstarch" << std::endl;
                outfile << theVars->TestLi << "," << ResultRate[0] << ",";
                outfile << ResultRate[1] << "," << ResultRate[2] << "," << ResultRate[3] << ",";
                outfile << ResultRate[4] << "," << ResultRate[5] << "," << ResultRate[6] << std::endl;
                break;
            case CM:
                outfile << "Light intensity,CO2AR" << std::endl;
                outfile << theVars->TestLi << "," << ResultRate[0] << std::endl;
                break;
            case EPS:
                outfile << ResultRate[0] << std::endl;
                break;
            default:
                break;
        }
        outfile.close();

        if (theVars != nullptr) {
            maindriver->inputVars = nullptr;
            delete theVars;
        }
        delete maindriver;
#ifdef SUNDIALS_CONTEXT_REQUIRED
        SUNContext_Free(&context);
#endif // SUNDIALS_CONTEXT_REQUIRED
        return (EXIT_SUCCESS);
    } catch (std::exception& e) {
        std::cout << "An error occurred: " << e.what() << std:: endl;
        exit(EXIT_FAILURE);
    }
}
