#include "hash.hpp"
#include "block.hpp"
#include "globals.hpp"
#include "char_loop.hpp"
#include <chrono>
#include <ctime>

int main(int argc, char **argv)
{
    eli_blockchain::blockchain my_chain;
    std::vector<eli_blockchain::transaction> my_transactions;

    eli_blockchain::transaction a_transaction("a", "b", 5);

    my_transactions.push_back(a_transaction);

    a_transaction.amount = 2;
    a_transaction.sender = "c";
    a_transaction.receiver = "b";

    my_transactions.push_back(a_transaction);

    a_transaction.amount = 2.2;
    a_transaction.receiver = "a";

    my_transactions.push_back(a_transaction);

    my_chain.add_block(my_transactions, 2);

    return 0;
}