#include "FIBF.hpp"
#include "CM.hpp"

class EPSCon {
public:
    EPSCon() {}
    EPSCon(const EPSCon &other) {
        CM_con = other.CM_con;
        FIBF_con = other.FIBF_con;
    }
    EPSCon(const FIBFCon &fother, const CMCon &cother) {
        CM_con = cother;
        FIBF_con = fother;
    }
    EPSCon(const std::vector<double> &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }
    void fromArray(const std::vector<double> &vec, const size_t offset = 0) {
        //for (int i = 0; i < size(); i++)
        //    std::cout << vec[offset + i] << ", ";
        //std::cout << std::endl;
        FIBF_con.fromArray(vec, offset);
        //for (int i = FIBF_con.size(); i < size(); i++)
        //    std::cout << vec[offset + i] << ", ";
        //std::cout << std::endl;
        CM_con.fromArray(vec, offset + FIBF_con.size());
        //std::cout << FIBF_con.size() << "  " << vec.size() << std::endl;
        //std::cout << std::endl << std::endl;
    }
    std::vector<double> toArray() {
        std::vector<double> fvec = FIBF_con.toArray();
        std::vector<double> cvec = CM_con.toArray();
        fvec.reserve(size());
        //std::cout << fvec.size() << std::endl;
        fvec.insert(fvec.end(), cvec.begin(), cvec.end());
        //std::cout << fvec.size() << std::endl;
        //std::cout << "INIT" << std::endl;
        //for (int i = 0; i < fvec.size(); i++)
        //    std::cout << fvec.at(i) << ", ";
        //std::cout << std::endl;
        //std::cout << FIBF_con.size() << std::endl;
        return fvec;
    }
    size_t size() {
        return CM_con.size() + FIBF_con.size();
    }
    CMCon CM_con;
    FIBFCon FIBF_con;
};

EPSCon EPS_Ini(Variables *myVars);

std::vector<double> EPS_mb(double t, EPSCon &EPS_Con, Variables *myVars);
