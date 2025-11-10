#ifndef ELI_BLOCK
#define ELI_BLOCK

#include <string>
#include <vector>
#include <ctime>
#include "globals.hpp"

using std::string;

namespace eli_blockchain
{
    class transaction
    {
    private:
        // Not to self: make the block_chain class handle the uniquiness and continuity of transaction_id
        const size_t transaction_id;
        const string sender;
        const string reciever;
        const double amount;

    public:
        transaction(const size_t &_transaction_id, const string &_sender, const string &_reciever, const double &_amount) : transaction_id(_transaction_id), sender(_sender), reciever(_reciever), amount(_amount) {}
    };

    class block
    {
    private:
        string prev_block_hash;
        time_t timestamp;
        string version;
        string root_hash;
        string nonce;
        size_t difficulty_target;
        std::vector<transaction> transactions;

    public:
        std::vector<transaction> get_transactions()
        {
            return transactions;
        }
    };

    class block_chain
    {
    private:
        std::vector<block> block_chain;

    public:
        size_t get_next_transaction_id() const
        {
            size_t transaction_id = 0;

            for (auto block : this->block_chain)
            {
                transaction_id += block.get_transactions().size();
            }

            return transaction_id;
        }

        void add_block(block _block)
        {
        }
    };

};

#endif // ELI_BLOCK