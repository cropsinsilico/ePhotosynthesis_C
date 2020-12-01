#pragma once

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

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>

struct Variables;

typedef std::vector<double> arr;

inline arr ones(const size_t length) { return arr(length, 1.); }

inline arr zeros(const size_t length) { return arr(length, 0.); }

template <typename T>
class TimeSeries {
public:
    TimeSeries() {}

    void insert(size_t step, double time, T &input);

    T &operator[](size_t i) { return _data[i]; }

    double timestamp(int i = -1) {
        if (i < 0)
            return _timestamp[current];
        return _timestamp[i];
    }

    T getLastData() { return _data.back(); }

    double getLastTime() { return _timestamp.back(); }
    size_t size() { return _step.size(); }

    void write(std::ofstream &of);

private:
    size_t current = 0;
    std::vector<T> _data;
    std::vector<int> _step;
    std::vector<double> _timestamp;
};

template <typename T>
void TimeSeries<T>::insert(size_t step, double time, T &input) {
    std::vector<int>::iterator it = std::find(_step.begin(), _step.end(), step);
    T vec(input);
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

template <typename T>
void TimeSeries<T>::write(std::ofstream &of) {
    for (size_t i = 0; i < _step.size(); i++) {
        of << _timestamp[i];
        for (const auto &e : _data[i])
            of << "," << e;
        of << std::endl;
    }
}
