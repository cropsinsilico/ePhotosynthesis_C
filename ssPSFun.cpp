#include "globals.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
//CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//University of Illinois at Urbana Champaign
//Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.

//   This file is part of e-photosynthesis.

//    e-photosynthesis is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation;

//    e-photosynthesis is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License (GPL)
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


arr ssPSFun(double VcmaxT, double JmaxT, double temp, double CO2, double Light, varptr &myVars) {
    
    //global kmCO2;
    //global kmO2;
    //global O2;
    //global GammaStar;
    //global Rd;
    
    const double done = ssPSIni(temp, myVars);
    
    const double Ci = CO2;
    const double wc = VcmaxT * (Ci - myVars.GammaStar) / (Ci + myVars.kmCO2 * (1 + myVars.O2 / myVars.kmO2));
    const double wj = JmaxT * (Ci - myVars.GammaStar) / (4.5 * Ci + 10.5 * myVars.GammaStar);
    const double w = std::min(wc, wj);
    
    const double Vm = 88.6 * pow(10, -3);
    const double at = 12.6 * Vm;
    const double p = 2.5 * Vm;
    const double Vr = Vm * 2.27;
    
    const double tC = at - p * wc / wj;
    const double tJ = (at - p) * (Vr / Vm - 1) / (wc * Vr / (wj * Vm) - 1);
    
    const double To = 1;
    double ATP;
    if (wc < wj) {
        ATP = To + tC;
    } else {
        ATP = tJ;
    }
    
    ATP = ATP / 5 * 1.5;
    arr rval = zeros(2);
    rval[0] = ATP;
    rval[1] = w;
    return rval;
}
