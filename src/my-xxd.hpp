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

#include <memory>
using dataptr = std::unique_ptr<std::vector<std::size_t>>;

typedef struct
{
    std::string name;
    std::string ext;
} OutputName;

OutputName get_out_name(std::string src);
int convert_in_file(dataptr &data, const std::string src);
int write_out_file (dataptr &data, const OutputName labels, const std::string dst);

#endif /* __MY_XXD_HPP */
