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
        const size_t transaction_id;
        const string sender;
        const string reciever;
        const double amount;

    public:
        transaction(const size_t &_transaction_id, const string &_sender, const string &_reciever, const double &_amount) : transaction_id(_transaction_id), sender(_sender), reciever(_reciever), amount(_amount) {}
    };

    class blockchain
    {
    private:
        class transaction
        {
        private:
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
            std::vector<blockchain::transaction> transactions;

        public:
            std::vector<blockchain::transaction> get_transactions()
            {
                return transactions;
            }
        };

        std::vector<block> block_chain;

    public:
        void add_block()
        {
        }
    };

};

#endif // ELI_BLOCK