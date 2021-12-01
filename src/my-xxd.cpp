/**
 * @file my-xxd.cpp
 * @author Juan Burgos
 * @brief 
 * @version 0.1
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "my-xxd.hpp"

struct out_name get_out_name(std::string src)
{
    for(std::size_t i = 0; i < src.length(); ++i)
    {
        src[i] == '\\' ? src[i] = '/' : src[i];
    }

    std::size_t pos1 = src.find_last_of("/");
    std::size_t pos2 = src.find_last_of(".");

    std::string name = src.substr(pos1+1, pos2-(pos1+1));
    std::string ext  = src.substr(pos2+1, src.length()-(pos2+1));

    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char ch){return std::toupper(ch);});
    std::transform(ext.begin(),  ext.end(),  ext.begin(),  [](unsigned char ch){return std::toupper(ch);});

    return {name,ext};
}

int convert_in_file(dataptr &data, const std::string src)
{
    std::ifstream ifs;
    ifs.open(src, std::ifstream::in | std::ifstream::binary);
    if(!ifs.is_open())
    {
        std::cout << "Failed to open src file " << src << std::endl;
        return 1;
    }

    ifs.seekg(0, std::ifstream::end);
    unsigned int file_size = static_cast<unsigned int>(ifs.tellg()) + 1;
    ifs.seekg(0, std::ifstream::beg);

    dataptr contents(new std::vector<unsigned int>(file_size));
    std::vector<unsigned int>::iterator it;
    for(it = contents->begin(); ifs.good() && it != contents->end(); ++it)
    {
        *it = static_cast<unsigned int>(ifs.get());
    }

    if(!ifs.eof())
    {
        std::cout << "Failed to read in whole file! Stopped at position " << static_cast<unsigned int>(ifs.tellg()) << " with rdstate " << ifs.rdstate() << std::endl;
        ifs.close();
        return 1;
    }

    contents->pop_back(); // Remove 0xff from end of data
    ifs.close();
    data = std::move(contents);

    return 0;
}

int write_out_file(dataptr &data, const struct out_name labels, const std::string dst)
{
    std::ofstream ofs;
    ofs.open(dst, std::ofstream::out | std::ofstream::trunc);
    if(!ofs.is_open())
    {
        std::cout << "Failed to open dst file " << dst << std::endl;
        return 1;
    }

    std::stringstream ss;
    ss << "#ifndef __" << labels.name << "_" << labels.ext << "_HPP\n";
    ss << "#define __" << labels.name << "_" << labels.ext << "_HPP\n\n";
    ss << "unsigned char " << labels.name << "_" << labels.ext << "[] = {";
    for(std::size_t i = 0; i < data->size(); ++i)
    {
        if(i % 12 == 0)
        {
            ss << "\n\t";
        }
        ss << std::setfill('0') << std::setw(2) << "0x" << std::hex << (0xff & (*data)[i]) << ", ";
        ofs.write(ss.str().c_str(), ss.str().length());
        ss.str(std::string());
        ss.clear();
    }
    ofs.seekp(static_cast<std::size_t>(ofs.tellp()) - 4); // Move from position (eof+1) to the position before ", " so we can remove the extra comma in the end
    ss << "\n};\n\nunsigned int " << labels.name << "_" << labels.ext << "_LEN = " << std::dec << data->size() << ";\n\n";
    ss << "#endif /* __" << labels.name << "_" << labels.ext << "_HPP */\n";
    ofs.write(ss.str().c_str(), ss.str().length());
    ofs.close();
    std::cout << "Generated out file: " << dst << std::endl;
    return 0;
}
