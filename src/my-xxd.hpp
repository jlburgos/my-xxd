#ifndef __MY_XXD_HPP
#define __MY_XXD_HPP

/**
 * @file my-xxd.hpp
 * @author Juan Burgos
 * @brief 
 * @version 0.1
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <string>
#include <vector>

struct out_name {
    std::string name;
    std::string ext;
};

struct out_name get_out_name(std::string src);
std::vector<unsigned int> convert_in_file(const std::string src);
bool write_out_file(const std::vector<unsigned int> &values, const struct out_name labels, const std::string dst);

#endif /* __MY_XXD_HPP */
