#ifndef ELI_BLOCK
#define ELI_BLOCK

#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include "globals.hpp"
#include "hash.hpp"

using std::string;

namespace eli_blockchain
{
    class transaction
    {
    public:
        string sender;
        string receiver;
        double amount;

        transaction(const string &_sender, const string &_receiver, const double &_amount) : sender(_sender), receiver(_receiver), amount(_amount) {}
    };

    class blockchain
    {
    private:
        class transaction
        {
        private:
            const size_t transaction_id;
            const string sender;
            const string receiver;
            const double amount;

        public:
            transaction(const size_t &_transaction_id, const string &_sender, const string &_receiver, const double &_amount)
                : transaction_id(_transaction_id), sender(_sender), receiver(_receiver), amount(_amount) {}
            transaction(const size_t &_transaction_id, const eli_blockchain::transaction &idless_transaction)
                : transaction_id(_transaction_id), sender(idless_transaction.sender), receiver(idless_transaction.receiver), amount(idless_transaction.amount) {}

            string content_concat() const
            {
                string content_str = std::to_string(this->transaction_id);
                content_str.append(this->sender);
                content_str.append(this->receiver);
                content_str.append(std::to_string(this->amount));
                return content_str;
            }

            string get_checksum() const
            {
                return eli_hash::hash(this->content_concat());
            }

            friend class blockchain;
        };

        class block
        {
        private:
            const string prev_block_hash;
            const std::chrono::system_clock::time_point timestamp;
            const string version;
            const string root_hash;
            const string nonce;
            const size_t difficulty_target;
            const std::vector<eli_blockchain::blockchain::transaction> transactions;

            inline string calculate_root_hash(const std::vector<eli_blockchain::blockchain::transaction> &ts)
            {
                return eli_hash::hash(eli_blockchain::blockchain::block_body_format(ts));
            }

        public:
            block(const string &_prev_block_hash,
                  const string &_nonce,
                  const size_t &_difficulty_target,
                  const std::vector<eli_blockchain::blockchain::transaction> &_transactions)
                : prev_block_hash(_prev_block_hash),
                  root_hash(eli_blockchain::blockchain::block::calculate_root_hash(_transactions)),
                  difficulty_target(_difficulty_target),
                  transactions(_transactions),
                  timestamp(std::chrono::system_clock::now()),
                  version(eli_globals::version),
                  nonce(_nonce)
            {
            }

            time_t get_timestamp() const
            {
                return std::chrono::system_clock::to_time_t(this->timestamp);
            }

            std::vector<blockchain::transaction> get_transactions() const
            {
                return transactions;
            }

            string get_checksum() const
            {
                return eli_hash::hash(eli_blockchain::blockchain::block_header_format(this));
            }

            friend class blockchain;
        };

        std::vector<block> block_chain;
        size_t transaction_count = 0;

        static string block_header_format(const string &prev_block_hash,
                                          const std::chrono::system_clock::time_point &timestamp,
                                          const string &version,
                                          const string &root_hash,
                                          const string &nonce,
                                          const size_t &difficulty_target)
        {
            string block_header_str = prev_block_hash;
            block_header_str.append(std::to_string(std::chrono::system_clock::to_time_t(timestamp)));
            block_header_str.append(version);
            block_header_str.append(root_hash);
            block_header_str.append(nonce);
            block_header_str.append(std::to_string(difficulty_target));
            return block_header_str;
        }

        static string block_header_format(const eli_blockchain::blockchain::block *b)
        {
            return block_header_format(b->prev_block_hash, b->timestamp, b->version, b->root_hash, b->nonce, b->difficulty_target);
        }

        static string block_body_format(const std::vector<eli_blockchain::blockchain::transaction> &transactions)
        {
            string block_body_str = "";
            for (auto transaction : transactions)
            {
                block_body_str.append(transaction.content_concat());
            }
        }

        size_t transaction_id_increment()
        {
            return transaction_count++;
        }

        string gen_nonce()
        {
        }

    public:
        void add_block(const std::vector<eli_blockchain::transaction> &idless_transactions, const size_t &difficulty_target = 3)
        {
            std::vector<eli_blockchain::blockchain::transaction> transactions;

            for (auto idl_t : idless_transactions)
            {
                eli_blockchain::blockchain::transaction t(transaction_id_increment(), idl_t);
                transactions.push_back(t);
            }
            string prev_block_hash = "";
            if (!block_chain.empty())
            {
                prev_block_hash = this->block_chain.back().get_checksum();
            }

            eli_blockchain::blockchain::block a(prev_block_hash, "", difficulty_target, transactions);
        }
    };

};

#endif // ELI_BLOCK