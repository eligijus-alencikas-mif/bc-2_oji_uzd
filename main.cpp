#include "hash.hpp"
#include "block.hpp"
#include "globals.hpp"
#include "char_loop.hpp"

int main(int argc, char **argv)
{
    char_loop sequence;

    for (size_t i = 0; i < 10000000; i++)
    {
        std::string current_hash = eli_hash::hash(sequence.process_string());
        if (current_hash.at(0) == '0' && current_hash.at(1) == '0' && current_hash.at(2) == '0')
        {
            std::cout << current_hash << std::endl;
        }
    }

    return 0;
}