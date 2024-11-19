//
// Created by lfc on 2021/3/1.
//
#include "csv_reader.hpp"

int main(int argc, char const* argv[]) {
    std::vector<std::vector<double>> datas;
    CsvReader::ReadCsvFile("/../scan_mode/mid360-real-centr.csv", datas);
    return 0;
}