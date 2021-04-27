#pragma once
#include <vector>
#include <string>


enum FileFormat { ascii, latin1 };


class DataLoader {
    std::vector<std::vector<std::string>> rows_;
    bool headerRow_;
    
public:
    void readCSV(const std::string& filepath, char sep=';', bool headerRow=false, unsigned int skipRows=0, FileFormat encoding=latin1, bool removeQuotationMarks=true);
    void reset();
    
    std::vector<std::vector<std::string>> getRows() const { return rows_; }
    bool hasHeaderRow() const { return headerRow_; }
    bool hasValidData() const { return (rows_.size() > 1 && rows_.front().size() == rows_.back().size() && rows_.front().size() > 1); }
private:
    std::string decodeLatin1_(std::string& str);
};
