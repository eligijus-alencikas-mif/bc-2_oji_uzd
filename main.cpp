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
    int number_of_blocks = -1;
    int number_of_transactions = -1;
    int difficulty_target = -1;
    int number_of_users = -1;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-b" && i + 1 < argc)
        {
            number_of_blocks = std::stoul(argv[++i]);
        }
        else if (arg == "-t" && i + 1 < argc)
        {
            number_of_transactions = std::stoul(argv[++i]);
        }
        else if (arg == "-d" && i + 1 < argc)
        {
            difficulty_target = std::stoul(argv[++i]);
        }
        else if (arg == "-u" && i + 1 < argc)
        {
            number_of_users = std::stoul(argv[++i]);
        }
        else
        {
            std::cerr << "Unknown argument: " << arg << "\n";
            return 1;
        }
    }

    // Validate inputs
    if (number_of_blocks == -1 || number_of_transactions == -1 || difficulty_target == -1 || number_of_users == -1)
    {
        std::cerr << "Usage: " << argv[0] << " -b <number_of_blocks> -t <number_of_transactions> -d <difficulty_target> -u <number_of_users> \n";
        return 1;
    }

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
    fill_transactions(transactions, my_chain.get_user_pool(), number_of_transactions, 10000);

    for (size_t i = 0; i < number_of_blocks; i++)
    {

        if (!my_chain.add_block(transactions, difficulty_target))
        {
            std::cout << "Block generation timed out" << std::endl;
            i--;
            continue;
        }
        std::cout << my_chain.get_block_header().str() << std::endl;
        // std::cout << my_chain.get_block_body().str() << std::endl;

        transactions.clear();
        fill_transactions(transactions, my_chain.get_user_pool(), number_of_transactions, 10000);
    }
    return 0;
}