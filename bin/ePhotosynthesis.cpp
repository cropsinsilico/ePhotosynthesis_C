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
#include "ePhotosynthesis.hpp"


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
        std::string evn, atpcost, optionsFile, enzymeFile, grnFile,
          outputFile;
        double stoptime, begintime, stepsize;
        double abstol, reltol;
        double Tp;
        DriverType driverChoice;
        int driver = 1, maxSubSteps, RUBISCOMETHOD = 2;
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
                ("d,driver",
                 "The driver to use. Choices are:\n"
                 "1 - trDynaPS (default): PS, PR, FI, BF, SUCS,\n"
                 "       RuACT, XanCycle, RROEA\n"
                 "2 - DynaPS: PS, PR, FI, BF, SUCS, XanCycle\n"
                 "3 - CM: PS, PR, SUCS\n"
                 "4 - EPS: PS, PR, FI, BF, SUCS",
                 cxxopts::value<int>(driver))
                ("c,c3", "Use the C3 model, automatically set to true for EPS driver", cxxopts::value<bool>(useC3)->default_value("false"))
	        ("rubiscomethod",
                 "The method to use for rubisco calculations. Choices are:\n"
                 "1 - Use enzyme concentration for\n"
                 "    calculation\n"
                 "2 - (default) Use the michaelis menton and enzyme\n"
                 "    concentration together for calculation",
                 cxxopts::value<int>(RUBISCOMETHOD))
                ("t,abstol", "Absolute tolerance for calculations", cxxopts::value<double>(abstol)->default_value("1e-5"))
                ("r,reltol", "Relative tolerance for calculations", cxxopts::value<double>(reltol)->default_value("1e-4"))
                ("T,Tp", "Input Temperature", cxxopts::value<double>(Tp)->default_value("0.0"))
                ("o,options", "Name of a text file which specifies any of the above options. Command line arguments have priority.", cxxopts::value<std::string>(optionsFile)->default_value(""))
                ("x,output", "Name the the text file that outputs should be saved to.",
                 cxxopts::value<std::string>(outputFile)->default_value("output.data"))
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
            varSearch(outputFile)
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

        Variables *theVars = new Variables();
        // Ensure that command line argument takes precedence
        if (result.count("Tp") != 0) {
            theVars->Tp = Tp;
            inputs["Tp_SET"] = std::to_string(Tp);
        }
        if (fileProvided(evn, evn))
            theVars->readParam(evn, inputs);
        if (fileProvided(atpcost, atpcost))
            theVars->readParam(atpcost, inputs);
        if (fileProvided(enzyme, enzymeFile)) {
            theVars->readEnzymeAct(enzymeFile);
        } else {
            if (useC3)
                throw std::runtime_error("Enzyme data required if --c3 set (automatically true for EPS driver)");
        }

        // Read the GRN data and assign it into the correct positions
        // based on the expected order
        if (fileProvided(grn, grnFile))
            theVars->readGRN(grnFile);

        Tp = theVars->Tp;
        theVars->record = record;
        theVars->useC3 = useC3;
        theVars->RUBISCOMETHOD = RUBISCOMETHOD;
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

        std::ofstream outfile(outputFile);

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
        return (EXIT_SUCCESS);
    } catch (std::exception& e) {
        std::cout << "An error occurred: " << e.what() << std:: endl;
        exit(EXIT_FAILURE);
    }
}
