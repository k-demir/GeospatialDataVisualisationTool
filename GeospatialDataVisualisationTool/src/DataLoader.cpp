#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>


void DataLoader::readCSV(const std::string& filepath, char sep, bool headerRow, unsigned int skipRows, FileFormat encoding, bool removeQuotationMarks) {
    std::ifstream file(filepath);
    headerRow_ = headerRow;
    
    reset();
    
    if (file.is_open()) {
        std::string line;
        unsigned int lineNum = 0;
        
        while (std::getline(file, line)) {
            if (lineNum++ <= skipRows)
                continue;
            
            if (removeQuotationMarks) {
                line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
                line.erase(std::remove(line.begin(), line.end(), '\''), line.end());
            }
                
            size_t pos = 0;
            size_t prevSep = 0;
            std::vector<std::string> row;
            while (pos != line.length() - 1) {
                if (line.at(pos) == sep) {
                    std::string d = line.substr(prevSep, pos - prevSep);
                    if (encoding == latin1) {
                        d = decodeLatin1_(d);
                    }
                    row.push_back(d);
                    prevSep = pos + 1;
                }
                pos++;
            }
            rows_.push_back(row);
        }
        
    }
    file.close();
}


void DataLoader::reset() {
    rows_.clear();
}


std::string DataLoader::decodeLatin1_(std::string& str) {
    std::string res;
    for (std::string::iterator it = str.begin(); it != str.end(); it++) {
        uint8_t currChar = *it;
        if (currChar < 0x80)
            res.push_back(currChar);
        else {
            res.push_back(0xc0 | currChar >> 6);
            res.push_back(0x80 | (currChar & 0x3f));
        }
    }
    return res;
}
