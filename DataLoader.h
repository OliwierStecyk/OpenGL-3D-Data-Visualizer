#ifndef DATALOADER_H
#define DATALOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct DataRecord {
    std::string label;
    std::vector<float> values;
};

class DataLoader {
public:
    std::vector<DataRecord> allData;
    bool load( const std::string& filename );
    float getMaxValue();
};

#endif