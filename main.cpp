#include "hash.hpp"
#include "block.hpp"
#include "globals.hpp"
#include "char_loop.hpp"
#include <chrono>
#include <ctime>

int main(int argc, char **argv)
{
    eli_blockchain::blockchain my_chain;
    std::vector<eli_blockchain::transaction> transactions;

    transactions.push_back(eli_blockchain::transaction("a", "b", 5));
    transactions.push_back(eli_blockchain::transaction("ac", "b", 2));

    my_chain.add_block(transactions, 3);

    transactions.push_back(eli_blockchain::transaction("a", "b", 5));
    transactions.push_back(eli_blockchain::transaction("ac", "b", 2));

    my_chain.add_block(transactions, 3);

    std::cout << my_chain.get_block_header(0).str() << std::endl;
    std::cout << my_chain.get_block_header().str() << std::endl;

    return 0;
}