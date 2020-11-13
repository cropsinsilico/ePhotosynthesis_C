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


double PSTitle(double m, double p, double n) {
    if (n == 1) {
        if (m == 1) {
            title('RUBP')
    } else if (if m == 2){
            title('PGA')
    } else if (if m == 3){
            title('DPGA')
    } else if (if m == 4){
            title('T3P')
    } else if (if m == 5){
            title('ADPG')
    } else if (if m == 6){
            title('FBP');
    } else if (if m == 7){
            title('E4P');
    } else if (if m == 8){
            title('S7P');
    } else if (if m == 9){
            title('SBP');
    } else if (if m == 10){
            title('ATP');
    } else if (if m == 11){
            title('NADPH');
    } else if (if m == 12){
            title('CO2');
    } else if (if m == 13){
            title('O2');
    } else if (if m == 14){
            title('HexP');
    } else if (if m == 15){
            title('PenP');
    } else {
            title('NONE');
        }
        suc = 1;
        xlabel('time(s)');
        ylabel('Concentration(mM)');
    } else if (if n == 2){
        if (m == 1) {
            title('Rubisco carboxylation')
    } else if (if m == 2){
            title('PGA kinase')
    } else if (if m == 3){
            title('GAPDH')
    } else if (if m == 4){
            title('T3P isomerase')
    } else if (if m == 5){
            title('Aldolase 1')
    } else if (if m == 6){
            title('FBPase');
    } else if (if m == 7){
            title('Transketolase 1');
    } else if (if m == 8){
            title('Aldolase 2');
    } else if (if m == 9){
            title('SBPase');
    } else if (if m == 10){
            title('Transketolase 2');
    } else if (if m == 11){
            title('Ru5P kinase');
    } else if (if m == 12){
            title('ATP synthase');
    } else if (if m == 13){
            title('ADPG Pyrophosphatase');
    } else if (if m == 14){
            title('DHAP export');
    } else if (if m == 15){
            title('PGA export');
    } else if (if m == 16){
            title('GAP export');
    } else {
            title('Starch synthase')
        }
        suc = 1;
        xlabel('time(s)');
        ylabel('Velocity(umol m - 2 s - 1)');
    }
    return suc;
}
