/**
 * @file main.cpp
 * @author Juan Burgos
 * @brief 
 * @version 0.1
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include "my-xxd.hpp"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "./my-xxd [src] [target]" << std::endl;
        return 1;
    }

    std::string src = argv[1];
    std::string dst = argv[2];

    vdataptr<std::size_t> vdata;
    int rc = convert_in_file(vdata, src);
    if(rc != 0)
    {
        return rc;
    }
    OutputName name = get_out_name(src);
    return write_out_file(vdata, name, dst);
}

