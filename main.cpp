#include "hash.hpp"
#include "block.hpp"
#include "globals.hpp"
#include "char_loop.hpp"
#include "users.hpp"
#include <chrono>
#include <ctime>
#include <random>

void fill_transactions(std::vector<eli_blockchain::transaction> &transactions, eli_users::user_pool users, const size_t &num_transactions, const size_t &max_transaction)
{
    auto user_keys = users.get_all_user_public_keys();

    std::random_device rd;                                                // obtain a random number from hardware
    std::mt19937 gen(rd());                                               // seed the generator
    std::uniform_int_distribution<> user_dist(0, user_keys.size() - 1);   // define the range
    std::uniform_int_distribution<> transaction_dist(1, max_transaction); // define the range

    for (size_t i = 0; i < num_transactions; i++)
    {
        std::string sender_key = user_keys.at(user_dist(gen));
        std::string receiver_key = user_keys.at(user_dist(gen));
        double amount = static_cast<double>(transaction_dist(gen));

        // Ensure sender and receiver are not the same
        if (sender_key != receiver_key)
        {
            if (users.transfer_balance(sender_key, receiver_key, amount))
            {
                transactions.push_back(eli_blockchain::transaction(sender_key, receiver_key, amount));
                continue;
            }
        }
        i--; // retry this iteration
    }
}

int main(int argc, char **argv)
{
    eli_blockchain::blockchain my_chain;

    // generate 1000 users with balances between 100 and 1000000
    std::random_device rd;                                      // obtain a random number from hardware
    std::mt19937 gen(rd());                                     // seed the generator
    std::uniform_int_distribution<> balance_dist(100, 1000000); // define the range
    for (size_t i = 0; i < 1000; i++)
    {
        my_chain.add_user("User_" + std::to_string(i), balance_dist(gen));
    }

    std::vector<eli_blockchain::transaction> transactions;

    fill_transactions(transactions, my_chain.get_user_pool(), 1000, 10000);
    my_chain.add_block(transactions);
    transactions.clear();
    fill_transactions(transactions, my_chain.get_user_pool(), 1000, 10000);
    my_chain.add_block(transactions);

    std::cout << my_chain.get_block_header(0).str() << std::endl;
    std::cout << my_chain.get_block_header().str() << std::endl;
    // std::cout << my_chain.get_block_body().str() << std::endl;

    return 0;
}