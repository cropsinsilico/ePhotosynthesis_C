#include "globals.hpp"
#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype */
#include "RedoxReg.hpp"
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

int RedoxReg_FPercent(N_Vector u, N_Vector f_val, void *user_data) {
    realtype *udata = N_VGetArrayPointer(u);
    realtype *fdata = N_VGetArrayPointer(f_val);
    UserData *data = static_cast<UserData*>(user_data);
    
    double Etr = data->coeffs[0];
    double Em = data->coeffs[1];
    realtype x = udata[0];
    fdata[0] = Etr - (Em - 0.03 * log10(x / (1 - x)));
    return 0;
}
//double RedoxReg_FPercent(double x, double Etr, double Em) {
    
//    const double f = Etr - (Em - 0.03 * log10(x / (1 - x)));
    
//    return f;
//}
