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

#define convD(x) static_cast<double>(stof(x, nullptr))
#define convS(x) x
#define convI(x) stoi(x, nullptr)
#define convB(x) ((bool)(stoi(x, nullptr)))
#define displayInputVar(fmt, src, val)				\
  printf("Input variable \"%s\" = " #fmt "\n", #src, val)

#define handleInput(cli, input, local, dst, conv, fmt)	\
  if (result.count(#cli) != 0) {			\
    theVars-> dst = local;				\
    displayInputVar(fmt, cli, theVars-> dst);		\
  } else if (inputs.count(#input) > 0) {		\
    theVars-> dst = conv(inputs.at(#input));		\
    displayInputVar(fmt, src, theVars-> dst);		\
  } else {						\
    printf("Input variable \"%s\" not set\n", #input);	\
  }
#define handleInputD(cli, input, local, dst)	\
  handleInput(cli, input, local, dst, convD, %lf)
#define handleInputI(cli, input, local, dst)	\
  handleInput(cli, input, local, dst, convI, %d)

// macros to get options from either the command line (has precedence) or an options file
#define varSearch(x, conv)						\
  if (result.count(#x) == 0 && inputs.count(#x) > 0)			\
    x = conv(inputs.at(#x))
#define varSearchD(x) varSearch(x, convD)
#define varSearchI(x) varSearch(x, convI)
#define varSearchS(x) varSearch(x, convS)
#define varSet(xcli, xfile) (result.count(#xcli) || inputs.count(#xfile))
#define fileProvided(xcli, xfile) varSet(xcli, xfile)

#define assignInputVar(src, dst, conv, fmt)		\
  if (inputs.count(#src) > 0) {				\
    theVars-> dst = conv(inputs.at(#src));		\
    displayInputVar(fmt, src, theVars-> dst);		\
  } else						\
    printf("Input variable \"%s\" not set\n", #src)

#define assignInputVarD(src, dst) assignInputVar(src, dst, convD, %lf)
#define assignInputVarI(src, dst) assignInputVar(src, dst, convI, %d)
#define setInputVar(src, mod, dst, conv, fmt)		\
  if (inputs.count(#src) > 0) {				\
    modules::mod::set ## dst (conv(inputs.at(#src)));	\
    displayInputVar(fmt, src, conv(inputs.at(#src)));	\
  } else						\
    printf("Input variable \"%s\" not set\n", #src)
#define setInputVarB(src, mod, dst) setInputVar(src, mod, dst, convB, %d)


int main(int argc, const char* argv[]) {
    try {
        bool record = false;
        bool useC3 = false;
        cxxopts::Options options("ePhoto", "Command line interface to C++ implementation of the matlab original");
        options.show_positional_help();
        std::string evn, atpcost, optionsFile, enzymeFile, grnFile,
	  outputFile, outputVars, outputParamVars, outputParamBase;
        double stoptime, begintime, stepsize;
        double abstol, reltol;
        double Tp;
        DriverType driverChoice;
        int driver = 1, maxSubSteps, RUBISCOMETHOD = 2, outputParam = 0;
        ushort dbglvl;
        bool debugDelta, debugInternal;
	std::map<PARAM_TYPE, std::map<MODULE, std::string> > param_files;
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
	        ("outputVars", "Comma separated list of names of variables that should be output to the table created at the end of a run. Variables for modules, equilibirium constants, etc. can be specified by including the C++ scope of the variable. (e.g. MOD:BF:cATPsyn would output the concentration of ATP synthase). If not provided the output variables will be determined by the driver.",
                 cxxopts::value<std::string>(outputVars)->default_value(""))
	        ("outputParam",
                 "Output model parameters at different times throughout the run. Choices are:\n"
                 "0 - (default) Don't output parameters during the run.\n"
                 "1 - Output the initial parameter values.\n"
                 "2 - Output the initial and final parameter values.\n"
                 "3 - Output the parameter values for each step in addition to the initial and final values.",
                 cxxopts::value<int>(outputParam))
	        ("outputParamBase", "Base name for files parameter values should be output to at times through the run as specified by outputParam",
                 cxxopts::value<std::string>(outputParamBase)->default_value(""))
	        ("outputParamVars", "Comma separated list of names of variables that should be output at various times during the run based on the value of outputParam. If not provided, all variables will be output.",
                 cxxopts::value<std::string>(outputParamVars)->default_value(""))
                ("h,help", "Produce help message")
                ("debug","Debug level", cxxopts::value<ushort>(dbglvl)->default_value("0"))
                ("debugDelta", "Debug deltas", cxxopts::value<bool>(debugDelta)->default_value("false"))
                ("debugInternal", "Debug internals", cxxopts::value<bool>(debugInternal)->default_value("false"))
                ;

	for (std::vector<MODULE>::const_iterator m = ALL_MODULE.begin();
	     m != ALL_MODULE.end(); m++) {
	  std::string mS = utils::enum_key2string(*m);
	  for (std::vector<PARAM_TYPE>::const_iterator pt = ALL_PARAM_TYPE.begin();
	       pt != ALL_PARAM_TYPE.end(); pt++) {
	    std::string ptS = utils::enum_key2string(*pt);
	    std::string name = mS + "_" + ptS + "_constants";
	    std::string desc = "File containing ";
	    bool no_input = false;
	    switch (*pt) {
	    case PARAM_TYPE_COND: {
	      desc += "initial conditions tracked by";
	      break;
	    }
	    case PARAM_TYPE_RC: {
	      desc += "rate constants that control the";
	      break;
	    }
	    case PARAM_TYPE_POOL: {
	      desc += "pool constants that control the";
	      break;
	    }
	    case PARAM_TYPE_KE: {
	      desc += "equilibrium constants that control the";
	      break;
	    }
	    case PARAM_TYPE_MOD: {
	      desc += "top level constants that control the";
	      break;
	    }
	    case PARAM_TYPE_VEL: {
	      no_input = true;
	      break;
	    }
	    case PARAM_TYPE_VARS: {
	      desc += "top level variables";
	      break;
	    }
	    default : {
	      throw std::runtime_error("No description for PARAM_TYPE " + ptS);
	    }
	    }
	    if (mS == "ALL") {
	      desc += " shared by all modules";
	    } else {
	      desc += " the " + mS + " module";
	    }
	    param_files[*pt][*m] = "";
	    if (not no_input) {
	      options.add_options()
		(name, desc,
		 cxxopts::value<std::string>(param_files[*pt][*m])->default_value(""));
	    }
	  }
	}

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return (EXIT_SUCCESS);
        }
        std::map<std::string, std::string> inputs;
        if (result.count("options")) {
            readFile(result["options"].as<std::string>(), inputs);

            varSearchS(evn);
            varSearchS(atpcost);
            varSearchS(enzymeFile);
            varSearchS(grnFile);
            varSearchS(outputFile);
            varSearchD(begintime);
            varSearchD(stoptime);
            varSearchD(stepsize);
            varSearchD(Tp);
            varSearchI(maxSubSteps);
            varSearchI(driver);
            varSearchI(RUBISCOMETHOD);
            varSearchD(abstol);
            varSearchD(reltol);
            varSearchS(outputVars);
            varSearchI(outputParam);
            varSearchS(outputParamBase);
            varSearchS(outputParamVars);
        }
        driverChoice = static_cast<DriverType>(driver);
	// TODO: Do yggdrasil initial input before this point so driver
	//   choice can be an input
	if (driverChoice == EPS)
	  useC3 = true;
	// Select driver prior to updating values from user provided files
	//   so that warnings can be generated when a file is provided
	//   that will not be used by the selected driver.
	drivers::select_driver(driverChoice, useC3);

	for (std::vector<MODULE>::const_iterator m = ALL_MODULE.begin();
	     m != ALL_MODULE.end(); m++) {
	  std::string mS = utils::enum_key2string(*m);
	  for (std::vector<PARAM_TYPE>::const_iterator pt = ALL_PARAM_TYPE.begin();
	       pt != ALL_PARAM_TYPE.end(); pt++) {
	    std::string ptS = utils::enum_key2string(*pt);
	    std::string inName = mS + "_" + ptS + "_constants";
	    if (param_files[*pt][*m].empty() && inputs.count(inName) > 0) {
	      param_files[*pt][*m] = inputs.at(inName);
	      std::cerr << "initDefaults: " << *m << ", " << *pt << ", " <<
		useC3 << ", " << param_files[*pt][*m] << std::endl;
	      Variables::initDefaults(*m, *pt, useC3, param_files[*pt][*m]);
	      inputs.erase(inName);
	    }
	  }
	}
      
        Variables *theVars = new Variables();
        // Ensure that command line argument takes precedence
        if (result.count("Tp") != 0) {
            theVars->Tp = Tp;
            inputs["Tp_SETXXX"] = std::to_string(Tp);
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
        theVars->useC3 = useC3; // Redundant after select_driver
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
	
#ifdef WITH_YGGDRASIL
	int flag = 0;
	Variables *origVars = theVars->deepcopy();
	YggInput steps("param");
	YggOutput out("output");
	bool first = true;
	int iteration = 0;
	while (true) {
	  std::cout << "ITERATION: " << iteration << std::endl;
	  if (!first)
	    theVars = origVars->deepcopy();
	  rapidjson::Document new_state;
	  flag = steps.recvVar(new_state);
	  if (flag < 0) {
	    if (first) {
	      printf("Failed to receive first step.\n");
	      exit(EXIT_FAILURE);
	    } else {
	      printf("End of input\n");
	      delete theVars;
	      break;
	    }
	  }

	  std::cout << "INPUT STATE = " << new_state << std::endl;
          updateParamYgg(new_state, theVars);
	  // assignYggVarD(CO2, Air_CO2);
	  // assignYggVarD(Air_CO2, Air_CO2);
	  // assignYggVarD(PAR, Radiation_PAR);
	  // assignYggVarD(Radiation_PAR, Radiation_PAR);
	  // assignYggVarD(ATPCost, TestATPCost);
	  // assignYggVarI(GRNC, GRNC);
	  // setYggVarB(SucPath, CM, TestSucPath);
	  // if (new_state.HasMember("Temp")) {
	  //   theVars->Tp = new_state["Temp"].GetDouble();
	  //   displayYggInputVar(%lf, Temp, theVars->Tp);
	  // }
#endif

	  maindriver = drivers::create_driver(driverChoice, theVars,
					      begintime, stepsize,
					      stoptime, maxSubSteps,
					      abstol, reltol, 1, 1);
	  if ((outputParam == 0) && !(outputParamBase.empty() &&
                                      outputParamVars.empty()))
	    outputParam = 1;
	  if (outputParam) {
            std::vector<std::string> outputParamVarsV;
            if (!outputParamVars.empty())
              outputParamVarsV = utils::str_split(outputParamVars, ",");
	    maindriver->outputParam(static_cast<OutputFreq>(outputParam),
                                    outputParamBase,
                                    outputParamVarsV);
          }
        std::vector<double> ResultRate = maindriver->run();

#ifdef WITH_YGGDRASIL
        rapidjson::Document out_state;
        param2ygg(maindriver->output, out_state, theVars);
        std::cout << "OUTPUT STATE = " << out_state << std::endl;
        flag = out.sendVar(out_state);
	if (flag < 0) {
	  printf("Error sending output.\n");
	  exit(EXIT_FAILURE);
	}
#else
	// std::cerr << theVars << std::endl;
        std::ofstream outfile(outputFile);
        if (!outputVars.empty()) {
          std::vector<std::string> outputVarsV = utils::str_split(outputVars, ",");
          maindriver->setOutputVars(outputVarsV);
        }
	maindriver->writeOutputTable(outfile);

        outfile.close();
#endif
	
        if (theVars != nullptr) {
            maindriver->inputVars = nullptr;
            delete theVars;
        }
        delete maindriver;

#ifdef WITH_YGGDRASIL
	  first = false;
	  iteration++;
	}
	if (origVars != nullptr)
	  delete origVars;
#endif
        return (EXIT_SUCCESS);
    } catch (std::exception& e) {
        std::cout << "An error occurred: " << e.what() << std:: endl;
        exit(EXIT_FAILURE);
    }
}
