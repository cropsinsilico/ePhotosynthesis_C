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
#include "drivers/drivers.hpp"
#include "Variables.hpp"

using namespace ePhotosynthesis;

// macros to get options from either the command line (has precedence) or an options file
#define varSearchD(x) if (result.count(#x) == 0 && inputs.count(#x) > 0) \
    x = stod(inputs.at(#x), nullptr);
#define varSearchI(x) if (result.count(#x) == 0 && inputs.count(#x) > 0) \
    x = stoi(inputs.at(#x), nullptr);
#define varSearch(x) if (result.count(#x) == 0 && inputs.count(#x) > 0) \
    x = inputs.at(#x);

enum DriverType {
    None,
    trDynaPS,
    DynaPS,
    CM,
    EPS,
    C4
};


int main(int argc, const char* argv[]) {
    try {
        bool record = false;
        bool useC3 = false;
        cxxopts::Options options("ePhotosynthesis", "C++ implementation of the matlab original");
        options.show_positional_help();
        std::string evn, atpcost, optionsFile, enzymeFile, paramFile;
        double stoptime, begintime, stepsize;
        double abstol, reltol;
        double Tp;
        DriverType driverChoice;
        int driver, maxSubSteps, species;
        ushort dbglvl;
        bool debugDelta, debugInternal;
        options.add_options()
                ("v,verbose", "Record output values for all steps (this can significantly slow the program).", cxxopts::value<bool>(record)->default_value("false"))
                ("e,evn", "The InputEvn.txt file.", cxxopts::value<std::string>(evn)->default_value("InputEvn.txt"))
                ("a,atpcost", "The InputATPCost.txt file.", cxxopts::value<std::string>(atpcost)->default_value("InputATPCost.txt"))
                ("n,enzyme", "The input enzyme file.", cxxopts::value<std::string>(enzymeFile)->default_value(""))
                ("p, paramFile", "The input parameter file for the C4 model", cxxopts::value<std::string>(paramFile)->default_value(""))
                ("x, species", "The species for the C4 model", cxxopts::value(species)->default_value("3"))
                ("b,begintime", "The starting time for the calculations.", cxxopts::value<double>(begintime)->default_value("0.0"))
                ("s,stoptime", "The time to stop calculations.", cxxopts::value<double>(stoptime)->default_value("5000.0"))
                ("z,stepsize", "The step size to use in the calculations.", cxxopts::value<double>(stepsize)->default_value("1.0"))
                ("m,maxSubSteps", "The maximum number of iterations at each time step.", cxxopts::value<int>(maxSubSteps)->default_value("750"))
                ("d,driver", "The driver to use. Choices are:                            1 - trDynaPS                                         2 - DynaPS                                           3 - CM                                               4 - EPS         ", cxxopts::value<int>(driver)->default_value("1"))
                ("c,c3", "Use the C3 model, automatically set to true for EPS driver", cxxopts::value<bool>(useC3)->default_value("false"))
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
            varSearchD(begintime)
            varSearchD(stoptime)
            varSearchD(stepsize)
            varSearchI(maxSubSteps)
            varSearchI(driver)
            varSearchD(abstol)
            varSearchD(reltol)
        }
        driverChoice = static_cast<DriverType>(driver);

        readFile(evn, inputs);
        if (driverChoice != C4)
            readFile(atpcost, inputs);
        Variables *theVars = new Variables();
        if (result.count("enzyme")) {
            readFile(enzymeFile, theVars->EnzymeAct);
        }

        for (auto const& mp : inputs) {
            //std::cout << mp.first << " = " << mp.second << std::endl;
            if (mp.first == "CO2" || mp.first == "CO2_air") {
                theVars->CO2_in = static_cast<double>(stof(mp.second, nullptr));
            } else if(mp.first == "Temperature") {
                theVars->Tp = static_cast<double>(stof(mp.second, nullptr));
            } else if(mp.first == "O2" || mp.first == "O2_air") {
                theVars->O2_cond = static_cast<double>(stof(mp.second, nullptr));
            } else if (mp.first == "RH") {
                theVars->WeatherRH = static_cast<double>(stof(mp.second, nullptr));
            } else if (mp.first == "Radiation_NIR") {
                theVars->Radiation_NIR = static_cast<double>(stof(mp.second, nullptr));
            } else if (mp.first == "Radiation_PAR" || mp.first == "PAR") {
                theVars->TestLi = static_cast<double>(stof(mp.second, nullptr));
            } else if (mp.first == "Radiation_LW") {
                theVars->Radiation_LW = static_cast<double>(stof(mp.second, nullptr));
            } else if (mp.first == "SucPath" && static_cast<double>(stof(mp.second, nullptr)) > 0.) {
                modules::CM::setTestSucPath(true);
            } else if (mp.first == "ATPCost") {
                theVars->TestATPCost = stoi(mp.second, nullptr);
            }
        }
        if (driverChoice == C4) {
            std::map<std::string, double> parameters;
            readFile(paramFile, parameters, species);
            //1Maize	2sorghum	3sugarcane
            for (auto const& mp : parameters) {
                if (mp.first == "slope") {
                    theVars->BallBerrySlopeC4 = mp.second;
                } else if (mp.first == "intercept") {
                    theVars->BallBerryInterceptC4 = mp.second * 1.6;
                } else if (mp.first == "ki") {
                    theVars->ki = mp.second;
                } else if (mp.first == "kd") {
                    theVars->kd = mp.second;
                } else if (mp.first == "Vpmax") {
                    theVars->Vpmax = mp.second;
                } else if (mp.first == "Vcmax") {
                    theVars->Vcmax = mp.second;
                } else if (mp.first == "taoRubisco") {
                    theVars->taoRub = mp.second;
                } else if (mp.first == "FactorVP") {
                    theVars->FactorVP = mp.second;
                } else if (mp.first == "FactorVC") {
                    theVars ->FactorVC = mp.second;
                } else if (mp.first == "RP") {
                    theVars->PPDKRP = mp.second;
                } else if (mp.first == "Rd") {
                    theVars->MRd = abs(mp.second);
                }
            }
        }

        theVars->record = record;
        theVars->useC3 = useC3;
        theVars->RUBISCOMETHOD = 1;
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
                theVars->useC3 = true;
                maindriver = new drivers::EPSDriver(theVars, begintime, stepsize, stoptime,
                                                    maxSubSteps, abstol, reltol, 1, 1, Tp);
                break;
            case C4:
                maindriver = new drivers::RAC4leafMetaDriver(theVars, begintime, stepsize,
                                                             stoptime, maxSubSteps, abstol,
                                                             reltol);
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
                outfile << theVars->TestLi << ResultRate[0] << std::endl;
                break;
            case EPS:
                outfile << ResultRate[0] << std::endl;
                break;
            case C4:
                outfile << ResultRate[0] << std::endl;
                //std::cout << ResultRate[0] << std::endl;
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
