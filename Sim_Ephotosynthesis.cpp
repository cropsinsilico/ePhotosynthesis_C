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

#include "globals.hpp"
#include "trDynaPS.hpp"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "Variables.hpp"

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

void Sim_Ephotosynthesis(bool record) {
    std::map<std::string, std::string> inputs;
    readFile("/home/friedel/crops_in_silico/ODE-FBA/ePhotosynthesis/C++/InputEvn.txt", inputs);
    readFile("/home/friedel/crops_in_silico/ODE-FBA/ePhotosynthesis/C++/InputATPCost.txt", inputs);
    Variables myVars;
    myVars.TestCa = static_cast<double>(stof(inputs.at("CO2"), nullptr));
    myVars.TestLi = static_cast<double>(stof(inputs.at("PAR"), nullptr));
    myVars.TestSucPath = stoi(inputs.at("SucPath"), nullptr);
    myVars.TestATPCost = stoi(inputs.at("ATPCost"), nullptr);
    myVars.record = record;
    trDynaPS *myDyna = new trDynaPS(&myVars);
    std::vector<double> ResultRate = myDyna->trDynaPS_Drive(1, 1);

    //std::ofstream outfile("output.data");
    //800,23.8514,8.04985,0.00395613,1.5763,2.58119,4.16627,8.04976
    std::cout << 800-myVars.TestLi << ",  " << 23.8514-ResultRate[0] << ",  ";
    std::cout << 8.04985-ResultRate[1] << ",  " << 0.00395613-ResultRate[2] << ",  " << 1.5763-ResultRate[3] << ",  ";
    std::cout << 2.58119-ResultRate[4] << ",  " << 4.16627-ResultRate[5] << ",  " << 8.04976-ResultRate[6] << std::endl;
    //outfile.close();
    delete myDyna;
}
