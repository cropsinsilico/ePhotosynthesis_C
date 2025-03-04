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

#include "ePhotosynthesis.hpp"
#include "modules/trDynaPS.hpp"
#include "modules/CM.hpp"
#include "modules/EPS.hpp"
#include "modules/PR.hpp"
#include "modules/BF.hpp"
#include "modules/FI.hpp"


using namespace ePhotosynthesis;

#ifdef WITH_YGGDRASIL
#include <YggInterface.hpp>
template<typename ValueType>
void updateParamYgg(const ValueType& v, Variables* theVars) {
  std::map<std::string, std::string> out;
  for (typename ValueType::ConstMemberIterator it = v.MemberBegin();
       it != v.MemberEnd(); it++) {
    std::string k(it->name.GetString());
    std::ostringstream oss;
    oss << it->value;
    out[k] = oss.str();
  }
  Variables::updateParam(out, theVars, "updateParamYgg");
}
template<typename T>
void param2ygg(const std::map<std::string, T>& v,
               rapidjson::Document& out,
               const Variables* theVars) {
  out.SetObject();
  for (typename std::map<std::string, T>::const_iterator it = v.begin();
       it != v.end(); it++) {
    rapidjson::Value key(it->first.c_str(), it->first.size(),
                         out.GetAllocator());
    rapidjson::Value val(it->second, out.GetAllocator());
    out.AddMember(key, val, out.GetAllocator());
  }
  out.AddMember("Light intensity", theVars->TestLi, out.GetAllocator());
}
#endif

std::map<std::string, double>
ePhotosynthesis::run_simulation(Variables*& theVars,
               const DriverType driverChoice,
               const double& begintime,
               const double& stoptime,
               const double& stepsize,
               const int& maxSubSteps,
               const double& abstol,
               const double& reltol,
               const std::string& outputFile,
               const std::vector<std::string>& outputVars,
               int outputParam,
               const std::string& outputParamBase,
               const std::vector<std::string>& outputParamVars) {
                     
    drivers::Driver *maindriver = nullptr;
    std::map<std::string, double> output;
	
#ifdef WITH_YGGDRASIL
    int flag = 0;
    Variables* origVars = nullptr;
    YggInput* steps = nullptr;
    YggOutput* out = nullptr;
    bool first = true;
    int iteration = 0;
    if (std::getenv("YGG_SUBPROCESS")) {
        try {
            steps = new YggInput("param");
            out = new YggOutput("output");
            origVars = theVars->deepcopy();
        } catch(...) {
            if (steps) {
                delete steps;
                steps = nullptr;
            }
        }
    }
          
    while (true) {
        if (steps) {
            std::cout << "ITERATION: " << iteration << std::endl;
            rapidjson::Document new_state;
            flag = steps->recvVar(new_state);
            if (flag < 0) {
                if (first) {
                    delete theVars;
                    theVars = nullptr;
                    throw std::runtime_error("Failed to receive first step.");
                } else {
                    std::cout << "End of input" << std::endl;
                    break;
                }
            }
            if (!first) {
                delete theVars;
                theVars = origVars->deepcopy();
            }

            std::cout << "INPUT STATE = " << new_state << std::endl;
            updateParamYgg(new_state, theVars);
        } else if (!first) {
            break;
        }
#endif

        maindriver = drivers::create_driver(driverChoice, theVars,
                                            begintime, stepsize,
                                            stoptime, maxSubSteps,
                                            abstol, reltol, 1, 1);
        if ((outputParam == 0) && !(outputParamBase.empty() &&
                                    outputParamVars.empty()))
	    outputParam = 1;
        if (outputParam) {
	    maindriver->outputParam(static_cast<OutputFreq>(outputParam),
                                    outputParamBase,
                                    outputParamVars);
        }
        if (!outputVars.empty()) {
            maindriver->setOutputVars(outputVars);
        }
        std::vector<double> ResultRate = maindriver->run();

#ifdef WITH_YGGDRASIL
        if (out) {
            rapidjson::Document out_state;
            param2ygg(maindriver->output, out_state, theVars);
            std::cout << "OUTPUT STATE = " << out_state << std::endl;
            flag = out->sendVar(out_state);
            if (flag < 0)
                throw std::runtime_error("Error sending output.");
        } else {
#endif // WITH_YGGDRASIL
            // std::cerr << theVars << std::endl;
            if (!outputFile.empty()) {
                std::ofstream outfile(outputFile);
                maindriver->writeOutputTable(outfile);
                outfile.close();
            }
#ifdef WITH_YGGDRASIL
        }
#endif // WITH_YGGDRASIL

        output = maindriver->getOutput();
        if (theVars != nullptr) {
            maindriver->inputVars = nullptr;
        }
        delete maindriver;

#ifdef WITH_YGGDRASIL
        first = false;
        iteration++;
    }
    if (origVars != nullptr)
        delete origVars;
#endif
    return output;
}

std::map<std::string, double>
ePhotosynthesis::run_simulation(DriverType driverChoice,
               const double& begintime,
               const double& stoptime,
               const double& stepsize,
               const int& maxSubSteps,
               const double& abstol,
               const double& reltol,
               std::map<std::string, std::string> inputs,
               bool useC3,
               const std::string& evnFile,
               const std::string& atpcostFile,
               const std::string& enzymeFile,
               const std::string& grnFile,
               const std::string& outputFile,
               const std::vector<std::string>& outputVars,
               const int& outputParam,
               const std::string& outputParamBase,
               const std::vector<std::string>& outputParamVars,
               ushort dbglvl,
               const bool debugDelta,
               const bool debugInternal,
               const bool& record,
               Variables *theVars) {
    // TODO: Do yggdrasil initial input before this point so driver
    //   choice can be an input?
    if (driverChoice == EPS)
        useC3 = true;
    // Select driver prior to updating values from user provided files
    //   so that warnings can be generated when a file is provided
    //   that will not be used by the selected driver.
    drivers::select_driver(driverChoice, useC3);

    bool created_theVars = false;
    if (!theVars) {
        theVars = new Variables();
        created_theVars = true;
    }

    // Add variables from inputs
    if (!inputs.empty()) {
        Variables::updateParam(inputs, theVars,
                               "ePhotosynthesis raw inputs");
    }
    
    // Ensure that command line argument takes precedence
    theVars->readParam(evnFile, inputs);
    theVars->readParam(atpcostFile, inputs);

    // Read transcription factors and set enzyme activity levels
    if (!enzymeFile.empty()) {
        theVars->readEnzymeAct(enzymeFile);
    } else {
        if (useC3)
            throw std::runtime_error("Enzyme data required if --c3 set (automatically true for EPS driver)");
    }

    // Read the GRN data and assign it into the correct positions
    // based on the expected order
    if (!grnFile.empty())
        theVars->readGRN(grnFile);

    theVars->record = record;
    theVars->useC3 = useC3; // Redundant after select_driver
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

    std::map<std::string, double> out = run_simulation(
        theVars, driverChoice,
        begintime, stoptime, stepsize, maxSubSteps, abstol, reltol,
        outputFile, outputVars,
        outputParam, outputParamBase, outputParamVars);

    if (theVars != nullptr)
        delete theVars;
    return out;
}
