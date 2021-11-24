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

#include "my-xxd.hpp"

struct out_name get_out_name(const std::string src)
{
    std::string src2 = src;
    for(std::size_t i = 0; i < src2.length(); ++i)
    {
        src2[i] == '\\' ? src2[i] = '/' : src2[i];
    }
    std::size_t pos1 = src2.find_last_of("/");
    std::size_t pos2 = src2.find_last_of(".");

    std::string name = src2.substr(pos1+1,pos2-(pos1+1));
    std::string ext = src2.substr(pos2+1,src.length()-(pos2+1));
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char ch){return std::toupper(ch);});
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char ch){return std::toupper(ch);});
    return {name,ext};
}

std::vector<unsigned int> convert_in_file(const std::string src)
{
    std::ifstream ifs;
    ifs.open(src, std::ifstream::in | std::ifstream::binary);
    if(!ifs.is_open())
    {
        std::cout << "Failed to open src file " << src << std::endl;
        return std::vector<unsigned int>();
    }

    ifs.seekg(0, std::ifstream::end);
    unsigned int file_size = static_cast<unsigned int>(ifs.tellg()) + 1;
    ifs.seekg(0, std::ifstream::beg);

    std::vector<unsigned int> data(file_size);
    std::vector<unsigned int>::iterator it;
    for(it = data.begin(); ifs.good() && it != data.end(); ++it)
    {
        *it = static_cast<unsigned int>(ifs.get());
    }

    if(!ifs.eof())
    {
        std::cout << "Failed to read in whole file! Stopped at position " << static_cast<unsigned int>(ifs.tellg()) << " with rdstate " << ifs.rdstate() << std::endl;
        ifs.close();
        return std::vector<unsigned int>();
    }

    data.pop_back(); // Remove 0xff from end of data

    ifs.close();
    return data;
}

bool write_out_file(const std::vector<unsigned int> &data, const struct out_name labels, const std::string dst)
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
    for(std::size_t i = 0; i < data.size(); ++i)
    {
        if(i % 12 == 0)
        {
            ss << "\n\t";
        }
        ss << std::setfill('0') << std::setw(2) << "0x" << std::hex << (0xff & data[i]) << ", ";
        ofs.write(ss.str().c_str(), ss.str().length());
        ss.str(std::string());
        ss.clear();
    }
    ofs.seekp(static_cast<long>(ofs.tellp()) - 4); // Move from position (eof+1) to the position before ", " so we can remove those two chars
    ss << "\n};\n\nunsigned int " << labels.name << "_" << labels.ext << "_LEN = " << std::dec << data.size() << ";\n\n";
    ss << "#endif /* __" << labels.name << "_" << labels.ext << "_HPP */\n";
    ofs.write(ss.str().c_str(), ss.str().length());
    ofs.close();
    std::cout << "Generated out file: " << dst << std::endl;
    return 0;
}
