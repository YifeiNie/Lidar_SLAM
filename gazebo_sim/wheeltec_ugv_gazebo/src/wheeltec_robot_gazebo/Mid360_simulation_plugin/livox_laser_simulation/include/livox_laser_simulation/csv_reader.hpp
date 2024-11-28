//
// Created by lfc on 2021/3/1.
//

#ifndef SRC_GAZEBO_CSV_READER_HPP
#define SRC_GAZEBO_CSV_READER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class CsvReader
{
public:
    // static bool ReadCsvFile(std::string file_name, std::vector<std::vector<double>>& datas) {
    //     std::fstream file_stream;
    //     file_stream.open(file_name, std::ios::in);
    //     if (file_stream.is_open()) {
    //         std::string header;
    //         std::getline(file_stream, header, '\n');
    //         while (!file_stream.eof()) {
    //             std::string line_str;
    //             std::getline(file_stream, line_str, '\n');
    //             std::stringstream line_stream;
    //             line_stream << line_str;
    //             std::vector<double> data;
    //             try {
    //                 while (!line_stream.eof()) {
    //                     std::string value;
    //                     std::getline(line_stream, value, ',');
    //                     data.push_back(std::stod(value));
    //                 }
    //             } catch (...) {
    //                 std::cerr << "cannot convert str:" << line_str << "\n";
    //                 continue;
    //             }
    //             datas.push_back(data);
    //         }
    //         std::cerr << "data size:" << datas.size() << "\n";
    //         return true;
    //     } else {
    //         std::cerr << "cannot read csv file!" << file_name << "\n";
    //     }
    //     return false;
    // }
    static bool ReadCsvFile(const std::string &file_name, std::vector<std::vector<double>> &datas)
    {
        std::ifstream file_stream(file_name);
        if (!file_stream.is_open())
        {
            std::cerr << "Cannot read csv file: " << file_name << "\n";
            return false;
        }
        std::string line_str;
        // 读取文件的每一行
        while (std::getline(file_stream, line_str))
        {
            if (line_str.empty())
            {
                continue; // 跳过空行
            }
            std::vector<double> data;
            std::stringstream line_stream(line_str);
            std::string value;
            try
            {
                // 读取每行中的每个值
                while (std::getline(line_stream, value, ','))
                {
                    if (!value.empty())
                    {
                        data.push_back(std::stod(value));
                    }
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "Cannot convert str: " << line_str << " - Error: " << e.what() << "\n";
                continue;
            }
            // 如果当前行数据有效，则添加到 datas
            if (!data.empty())
            {
                datas.push_back(data);
            }
        }
        file_stream.close(); // 手动关闭文件
        std::cerr << "Data size: " << datas.size() << "\n";
        return true;
    }
};

#endif // SRC_GAZEBO_CSV_READER_HPP
