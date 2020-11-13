class TimeSeries{
public:
    TimeSeries(){}

    void insert(int step, double time, std::vector<double> &input) {
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
    
    std::vector<double> &operator[](int i){
        return _data[i];
    }
    
    double timestamp(int i = -1) {
        if (i < 0)
            return _timestamp[current];
        return _timestamp[i];
    }
    
    std::vector<double> getLastData() {
        return _data.back();
    }
    
    double getLastTime(){
        return _timestamp.back();
    }
    int size() {
        return _step.size();
    }
    
    void write(std::ofstream &of) {
        for (size_t i = 0; i < _step.size(); i++)
    }
private:
    int current = 0;
    std::vector<std::vector<double> > _data;
    std::vector<int> _step;
    std::vector<double> _timestamp;
};
