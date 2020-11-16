#include "globals.hpp"

void TimeSeries::insert(int step, double time, std::vector<double> &input) {
        std::vector<int>::iterator it = std::find(_step.begin(), _step.end(), step);
        std::vector<double> vec(input);
        if (it == _step.end()) {
            _step.push_back(step);
            _timestamp.push_back(time);
            _data.push_back(vec);
        } else {
            int index = std::distance(_step.begin(), it);
            _timestamp[index] = time;
            _data[index] = vec;
        }
    }
    
void TimeSeries::write(std::ofstream &of) {
        for (size_t i = 0; i < _step.size(); i++) {
            of << _timestamp[i];
            for (const auto &e : _data[i])
                of << "," << e;
            of << std::endl;
    }
}
        
