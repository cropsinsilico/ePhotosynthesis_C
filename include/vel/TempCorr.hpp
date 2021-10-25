#pragma once

namespace ePhotosynthesis {
class TempCorr {
public:
    void clear() {
        V1 = 0.;
        V2 = 0.;
        V5 = 0.;
        V6 = 0.;
        KmCO2_6 = 0.;
        KmO2_6 = 0.;
        Vm_OC = 0.;
        Jmax = 0.;
        Vm_Enz = 0.;
    }
    double V1 = 0.; // 0
    double V2 = 0.; // 1
    double V5 = 0.; // 2
    double V6 = 0.; // 3
    double KmCO2_6 = 0.; // 4
    double KmO2_6 = 0.; // 5
    double Vm_OC = 0.; // 6
    double Jmax = 0.; // 7
    double Vm_Enz = 0.; // 8
};
}
