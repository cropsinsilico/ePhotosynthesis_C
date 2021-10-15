#include <math.h>
#include "Variables.hpp"
#include "globals.hpp"
#include "modules/Leaf.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

const double Cpwater = 4.184;//Jg-1c-1
const double Vandp = 198;//gm-2 leaf thickness 200um leaf density 0.7*10^3kg m-3
//leaf volume leaf thickness 200um~0.2L
//bundary layer
//leaf air space 20//
const double Vol_airspace = 0.04;//L

LeafCondition* Leaf::_MB_con(const double t, const LeafCondition* Leaf_con, Variables *theVars) {

    Rate(t, Leaf_con, theVars);

    const double Molar_Volume = 22.4 / 273 * (theVars->Tp + 273);

    LeafCondition* dydt = new LeafCondition();
    dydt->Ci = (theVars->Leaf_Vel.vCO2total - theVars->Leaf_Vel.NetAssimilation) / (Vol_airspace / Molar_Volume);
    dydt->Cb = 0.;
    dydt->Eb = 0.;
    dydt->Gs = theVars->Leaf_Vel.vgs;
    dydt->Tleaf = theVars->Leaf_Vel.EnergyBalanceResidual / Cpwater / Vandp;
    dydt->H2Oou = theVars->Leaf_Vel.vH2Ototal / pow(10, 6);
    dydt->CO2in = theVars->Leaf_Vel.vCO2total / pow(10, 6);

    return dydt;
}
