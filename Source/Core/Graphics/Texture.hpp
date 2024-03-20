#pragma once


#include <string>


struct Texture
{
public:
    unsigned int id;
    std::string type;

    static unsigned int FromPath(std::string const& path,
                                 std::string const& directory);
};

