#include "Headers/Parsing.hpp"
#include <iostream>

/*
**  clang++ -Wall -Werror -Wextra -g -o test main.cpp Parsing.cpp && valgrind ./test


**	clang++ -Wall -Werror -Wextra -g -o test main.cpp Parsing.cpp Parsing_get.cpp && ./test confs/test.conf
*/

int             main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Error, need one argument!" << std::endl;
        return (1);
    }
    else
    {
        std::string name = argv[1];
        Parsing relou = Parsing(name);
    }
    return (0);
}