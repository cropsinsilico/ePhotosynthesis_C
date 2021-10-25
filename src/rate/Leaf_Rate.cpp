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

#include <math.h>
#include "Variables.hpp"
#include "modules/Leaf.hpp"
#include "conditions/LeafCondition.hpp"
#include "conditions/RAC4leafMetaCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

const double PhotosynthesQ10 = 2.;
const double Rd25 = 0.6;//For steasy state model
const double Boltzman = 5.6697e-8;// Stefan-Boltzmann constant W m^{-2} K^{-4}
const double LatentHeatVaporization = 44000.0;//J mole^{-1}
const double Pressure = 101325.0; // Standard atmospheric pressure Pa
const double ConstantsCp = 29.3;

const double VmaxC4 = 160.; //For steady-state photosynthesis model
const double gm = 3.;
const double Sc = 3. * std::pow(10., 4.);
const double CForced = 4.322 / 1000.0;
const double CFree = 1.6361 / 1000.0;
const double LeafDimension = 0.028; //Leaf width/needle diameter m
const bool kdcon = true; //= true: constant kd; = false: kd change with light
const double Abs = 0.85;
//leaf energy balance
const double Epsilon = 0.96;// Leaf thermal emissivity
const double LEFactor = 1.0; ////////1.0//////////
const double LWFactor = 2.0;
const double HFactor = 2.0;//////////GGGGGGGG


void Leaf::_Rate(const double t, const LeafCondition* const Leaf_con, Variables *theVars) {

    double NetAssimilation;
    double Gs = Leaf_con->Gs;
    double Gsw = Leaf_con->Gs * 1.6;
    double Eb = Leaf_con->Eb;

    if (!theVars->Para_mata) {
        const double Q10Temperature = std::pow(PhotosynthesQ10, (Leaf_con->Tleaf - 25.0) / 10.0);
        const double Rd = Rd25 * Q10Temperature / (1.0 + std::exp(1.3 * (Leaf_con->Tleaf - 55.0)));
        const double ThetaC4 = 0.83;//curvature parameter
        const double BetaC4 = 0.93;//curvature parameter
        const double kC4 = 0.7;// initial slope of photosynthetic CO2 response
        const double kiC4 = kC4 * std::pow(2., (Leaf_con->Tleaf - 25.) / 10.);
        //myVars.VmaxC4=45;//umol m-2 s-1 maximum rubisco capacity
        const double VmaxiC4 = VmaxC4 * std::pow(2., ((Leaf_con->Tleaf - 25.) / 10.)) / ((1. + std::exp(0.3 * (13. - Leaf_con->Tleaf))) * (1. + std::exp(0.3 * (Leaf_con->Tleaf - 36.))));
        const double Ji = 0.05 * CONVERT * theVars->TestLi;//alpha*ar*f*Qp;
        const double Jc = Leaf_con->Ci * std::pow(10., -6.) * Pressure * kiC4 * std::pow(10., 6.) / Pressure;
        const double Je = VmaxiC4;
        //Ax=[Ji Jc Je];
        const double M = (Je + Ji - std::sqrt(std::pow((Je + Ji), 2.) - 4. * ThetaC4 * Je * Ji)) / (2. * ThetaC4);
        const double GrossAssimilation = (M + Jc - std::sqrt(std::pow((M + Jc), 2.) - 4. * BetaC4 * M * Jc)) / (2. * BetaC4);
        NetAssimilation = GrossAssimilation - Rd;
        //end
    } else {
        const double vinf = gm * Sc * std::pow(10., -3.) * (Leaf_con->Ci / (3. * std::pow(10., 4.)) - Leaf_con->parent->Enzyme_con->MC_CO2);
        NetAssimilation = vinf * 1000.;
    }
    const double LeafTemperatureKelvin = Leaf_con->Tleaf + 273.15;//Leaf temperature K
    const double TemperatureKelvin = theVars->Tp + 273.15;// Air temperature K
    const double ESatweather = 0.611 * std::exp(17.502 * theVars->Tp / (theVars->Tp + 240.97));//Vapor pressure kPa
    const double ESaturation = 0.611 * std::exp(17.502 * Leaf_con->Tleaf / (Leaf_con->Tleaf + 240.97));//Vapor pressure kPa

    const double Ea = theVars->WeatherRH * ESatweather;//Vapor pressure kPa

    const double TDifference = (LeafTemperatureKelvin / (1.0 - 0.378 * Eb / Pressure)) - (TemperatureKelvin / (1.0 - 0.378 * Ea * 1000. / Pressure));

    const double GbForced = CForced * std::pow(TemperatureKelvin, 0.56) * std::sqrt((TemperatureKelvin + 120.0) * (theVars->Wind / LeafDimension / Pressure));
    const double GbFree = CFree * std::pow(LeafTemperatureKelvin, 0.56) * std::sqrt((LeafTemperatureKelvin + 120.0) / Pressure) *  std::pow((std::abs(TDifference) / LeafDimension), 0.25);

    double Gbw, Cb;
    if (GbFree >= GbForced) {
        Gbw = GbFree;
    } else {
        Gbw = GbForced; // m/s
    }
    Gbw = Gbw * 41.4;//Conversion from m/s to moles/m2 leaf area/s
    const double Gb = Gbw / 1.37;//or 1.37
    Cb = theVars->CO2_in - 1.37 * NetAssimilation / Gb;// ppm ***********
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ////Update stomatal conductance moles/m2 leaf area/s
    double Gsw0 = theVars->BallBerryInterceptC4 + theVars->BallBerrySlopeC4 * NetAssimilation * Eb / ESaturation / Cb;
    const double a = - 0.1081;//Jarvist 1976
    const double b = 1.009;
    const double c = 1.104;
    const double PhiLeaf = 0.;//Mpa
    const double gsxsen = 1.;
    const double WaterStressFactor = a * std::exp(-b * PhiLeaf) + c;

    Gsw0 = gsxsen * Gsw0 * WaterStressFactor;//Apply water stress factor
    double Gs0 = Gsw0 / 1.6;
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //if gs response time is 0
    Gs0 = std::max(Gs0, 0.);

    //if gs response time is not O
    double vgs;
    double ki_Gs;
    double kd_Gs;

    if (theVars->GsResponse) {
        if (kdcon) {
            kd_Gs = 1. / (theVars->kd / 60.);
            ki_Gs = 1. / (theVars->ki / 60.);
        } else {
            //kd_Gs = 1 / ((theVars->kd + myVars.ainter * I) / 60);
            //ki_Gs = 1 / (theVars->ki / 60);
        }

        if (Gs < Gs0) {
            vgs = (Gs0 - Gs) / ki_Gs;
        } else {
            vgs = (Gs0 - Gs) / kd_Gs;
        }

    } else {
        Gs = Gs0;
        Gsw = Gsw0;
        vgs = 0.;
    }

    Cb = theVars->CO2_in - NetAssimilation / Gb;
    const double Gwtotal = Gbw * Gsw / (Gbw + Gsw);
    Eb = Gwtotal * (ESaturation - Ea) / Gbw + Ea;
    const double Gctotal = Gb * Gs / (Gb + Gs);// moles/m2 leaf area/s

    theVars->Leaf_Vel.vCO2b = Gb * (theVars->CO2_in - Cb);
    theVars->Leaf_Vel.vCO2s = Gs * (Cb - Leaf_con->Ci);
    theVars->Leaf_Vel.vH2Ob = Gbw * (Eb - Ea) / (Pressure / 1000.0) * std::pow(10., 6.0);
    theVars->Leaf_Vel.vH2Os = Gsw * (ESaturation - Eb) / (Pressure / 1000.0) * std::pow(10., 6.0);
    theVars->Leaf_Vel.vCO2total = Gctotal * (theVars->CO2_in - Leaf_con->Ci);

    const double vH2Ototal = Gwtotal * (ESaturation - Ea) / (Pressure / 1000.0) * std::pow(10., 6.0);

    const double LeafEnergyFluxMe = 0.506 * NetAssimilation;//Energy in biochemical reactions W/m2

    if (theVars->Radiation_LW == 0.)
        theVars->Radiation_LW = LWFactor * Epsilon * Boltzman *  std::pow((273.15 + theVars->Tp), 4.);

    const double SensibleHeat = HFactor * ConstantsCp * 0.924 * Gbw * (Leaf_con->Tleaf - theVars->Tp);

    const double Emission = LWFactor * Epsilon * Boltzman * std::pow((273.15 + Leaf_con->Tleaf), 4.0);
    const double LatentHeat = LEFactor * vH2Ototal / std::pow(10., 6.0) * LatentHeatVaporization;

    theVars->Leaf_Vel.EnergyBalanceResidual = theVars->TestLi * Abs + theVars->Radiation_NIR + theVars->Radiation_LW - Emission - SensibleHeat - LatentHeat - LeafEnergyFluxMe;

    theVars->Leaf_Vel.NetAssimilation = NetAssimilation;
    theVars->Leaf_Vel.vH2Ototal = vH2Ototal;
    theVars->Leaf_Vel.vgs = vgs;
    theVars->Leaf_Vel.vleak = theVars->Enzyme_Vel.vleak / theVars->Enzyme_Vel.v2;
    theVars->Leaf_Vel.Gs = Gs;
    theVars->Leaf_Vel.Cb = Cb;
    theVars->Leaf_Vel.Ci = Leaf_con->Ci;
    theVars->Leaf_Vel.Gbw = Gbw;
#ifdef INCDEBUG
    DEBUG_INTERNAL(theVars->Leaf_Vel)
#endif
    if (theVars->record) {
        if (t > Leaf::TIME) {
            Leaf::N++;
            Leaf::TIME = t;
        }
        theVars->Leaf_VEL.insert(Leaf::N - 1, t, theVars->Leaf_Vel);
    }
    /*
    myVars.Gs_VEL[myVars.TIME_N - 1][0] = t;
    myVars.Gs_VEL[myVars.TIME_N - 1][1] = NetAssimilation;
    myVars.Gs_VEL[myVars.TIME_N - 1][2] = vCO2b;
    myVars.Gs_VEL[myVars.TIME_N - 1][3] = vCO2s;
    myVars.Gs_VEL[myVars.TIME_N - 1][4] = vH2Ob;
    myVars.Gs_VEL[myVars.TIME_N - 1][5] = vH2Os;
    myVars.Gs_VEL[myVars.TIME_N - 1][6] = EnergyBalanceResidual;
    myVars.Gs_VEL[myVars.TIME_N - 1][7] = vCO2total;
    myVars.Gs_VEL[myVars.TIME_N - 1][8] = vH2Ototal;
    myVars.Gs_VEL[myVars.TIME_N - 1][9] = vgs;
    myVars.Gs_VEL[myVars.TIME_N - 1][10] = vleak / myVars.v2;
    myVars.Gs_VEL[myVars.TIME_N - 1][11] = Gs;
    myVars.Gs_VEL[myVars.TIME_N - 1][12] = Cb;
    myVars.Gs_VEL[myVars.TIME_N - 1][13] = Ci;
    myVars.Gs_VEL[myVars.TIME_N - 1][14] = Gbw;
    */

}
