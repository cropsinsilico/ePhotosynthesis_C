#include "globals.hpp"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>

void readFile(const std::string &filename, std::map<std::string, std::string> &mapper) {
    std::vector<std::string> tempVec;
    std::string input;
    std::ifstream inputfile(filename);
    while (std::getline(inputfile, input)) {
        if (input.empty())
            return;
        boost::split(tempVec, input, boost::is_any_of("\t "));
        mapper.insert(std::pair<std::string, std::string>(tempVec[0], tempVec[1]));
    }
}

void Sim_Ephotosynthesis() {
    std::map<std::string, std::string> inputs;
    readFile("/home/friedel/crops_in_silico/ODE-FBA/ePhotosynthesis/C++/InputEvn.txt", inputs);
    readFile("/home/friedel/crops_in_silico/ODE-FBA/ePhotosynthesis/C++/InputATPCost.txt", inputs);
    varptr myVars;
    myVars.TestCa = stof(inputs.at("CO2"), nullptr);
    myVars.TestLi = stof(inputs.at("PAR"), nullptr);
    myVars.TestSucPath = stoi(inputs.at("SucPath"), nullptr);
    myVars.TestATPCost = stoi(inputs.at("ATPCost"), nullptr);
    trDynaPS *myDyna = new trDynaPS(&myVars);
    std::vector<double> ResultRate = myDyna->trDynaPS_Drive(1, 1);
    
    std::ofstream outfile("output.data");
    outfile << myVars.TestLi << "," << ResultRate[0] << "," << ResultRate[1] << "," << ResultRate[2] << "," << ResultRate[3] << "," << ResultRate[4] << "," << ResultRate[5] << "," << ResultRate[6] << std::endl;
    outfile.close();
    delete myDyna;
}
