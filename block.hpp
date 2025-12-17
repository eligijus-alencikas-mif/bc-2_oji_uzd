#ifndef ELI_BLOCK
#define ELI_BLOCK

#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include "globals.hpp"
#include "hash.hpp"
#include "char_loop.hpp"

using std::string;

namespace eli_blockchain
{
    class transaction
    {
    private:
        const string transaction_id;
        const string sender;
        const string receiver;
        const double amount;

    public:
        transaction(const string &_sender, const string &_receiver, const double &_amount) : transaction_id(eli_hash::hash(sender + receiver + std::to_string(amount))), sender(_sender), receiver(_receiver), amount(_amount) {}

        string content_concat() const
        {
            string content_str = this->transaction_id;
            content_str.append(this->sender);
            content_str.append(this->receiver);
            content_str.append(std::to_string(this->amount));
            return content_str;
        }

        string get_checksum() const
        {
            return eli_hash::hash(this->content_concat());
        }

        std::string str() const
        {
            std::stringstream ss;
            ss << "Transaction ID: " << this->transaction_id << "\n";
            ss << "Sender: " << this->sender << "\n";
            ss << "Receiver: " << this->receiver << "\n";
            ss << "Amount: " << this->amount << "\n";
            ss << "Checksum: " << this->get_checksum() << "\n";
            return ss.str();
        }
    };

    class blockchain
    {
    private:
        class user_pool
        {
        private:
            class user
            {
            private:
                const std::string name;
                const std::string public_key;
                double balance;

            public:
                user(const std::string &_name, const std::string &_public_key, const double &_balance)
                    : name(_name), public_key(_public_key), balance(_balance) {}

                std::string get_name() const
                {
                    return this->name;
                }

                double get_balance() const
                {
                    return this->balance;
                }

                std::string get_public_key() const
                {
                    return this->public_key;
                }

                bool adjust_balance(const double &amount)
                {
                    if (this->balance + amount < 0)
                    {
                        return false;
                    }

                    this->balance += amount;
                    return true;
                }
            };

            char_loop public_key_generator;
            std::vector<user> users;

        public:
            void add_user(const std::string &_name, const double &_balance)
            {
                users.emplace_back(_name, eli_hash::hash(this->public_key_generator.get_and_increment() + _name), _balance);
            }

            double get_user_balance(const std::string &_public_key) const
            {
                for (const auto &u : users)
                {
                    if (u.get_public_key() == _public_key)
                    {
                        return u.get_balance();
                    }
                }
                throw std::invalid_argument("User with the given public key not found");
            }
        };

        // class transaction
        // {
        // private:
        //     const size_t transaction_id;
        //     const string sender;
        //     const string receiver;
        //     const double amount;

        // public:
        //     transaction(const size_t &_transaction_id, const string &_sender, const string &_receiver, const double &_amount)
        //         : transaction_id(_transaction_id), sender(_sender), receiver(_receiver), amount(_amount) {}
        //     transaction(const size_t &_transaction_id, const eli_blockchain::transaction &idless_transaction)
        //         : transaction_id(_transaction_id), sender(idless_transaction.sender), receiver(idless_transaction.receiver), amount(idless_transaction.amount) {}

        //     string content_concat() const
        //     {
        //         string content_str = std::to_string(this->transaction_id);
        //         content_str.append(this->sender);
        //         content_str.append(this->receiver);
        //         content_str.append(std::to_string(this->amount));
        //         return content_str;
        //     }

        //     string get_checksum() const
        //     {
        //         return eli_hash::hash(this->content_concat());
        //     }

        //     friend class blockchain;
        // };

        class block
        {
        private:
            const string prev_block_hash;
            const std::chrono::system_clock::time_point timestamp;
            const string version;
            const string root_hash;
            const string nonce;
            const size_t difficulty_target;
            const std::vector<eli_blockchain::transaction> transactions;

            static string calculate_root_hash(const std::vector<eli_blockchain::transaction> &ts)
            {
                return eli_hash::hash(eli_blockchain::blockchain::block_body_format(ts));
            }

        public:
            block(const string &_prev_block_hash,
                  const std::chrono::system_clock::time_point &_timestamp,
                  const string &_root_hash,
                  const string &_nonce,
                  const size_t &_difficulty_target,
                  const std::vector<eli_blockchain::transaction> &_transactions)
                : prev_block_hash(_prev_block_hash),
                  root_hash(_root_hash),
                  difficulty_target(_difficulty_target),
                  transactions(_transactions),
                  timestamp(_timestamp),
                  version(eli_globals::version),
                  nonce(_nonce)
            {
            }

            time_t get_timestamp() const
            {
                return std::chrono::system_clock::to_time_t(this->timestamp);
            }

            std::vector<eli_blockchain::transaction> get_transactions() const
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

        static string block_body_format(const std::vector<eli_blockchain::transaction> &transactions)
        {
            string block_body_str = "";
            for (auto transaction : transactions)
            {
                block_body_str.append(transaction.content_concat());
            }
            return block_body_str;
        }

        size_t transaction_id_increment()
        {
            return transaction_count++;
        }

        string gen_nonce(const string &prev_block_hash,
                         const std::chrono::system_clock::time_point &timestamp,
                         const string &root_hash,
                         const size_t &difficulty_target)
        {
            char_loop nonce;

            while (true)
            {
                bool found = true;

                string hash = eli_hash::hash(eli_blockchain::blockchain::block_header_format(prev_block_hash, timestamp, eli_globals::version, root_hash, nonce.get(), difficulty_target));
                for (size_t i = 0; i < difficulty_target; i++)
                {
                    if (hash.at(i) != '0')
                    {
                        found = false;
                        break;
                    }

                    found = true;
                }
                if (found)
                {
                    break;
                }

                nonce.increment_character();
            }

            return nonce.get();
        }

    public:
        void add_block(const std::vector<eli_blockchain::transaction> &transactions, const size_t &difficulty_target = 3)
        {
            const std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now();

            const string root_hash = eli_blockchain::blockchain::block::calculate_root_hash(transactions);

            string prev_block_hash = "";
            if (!block_chain.empty())
            {
                prev_block_hash = this->block_chain.back().get_checksum();
            }

            const string nonce = gen_nonce(prev_block_hash, timestamp, root_hash, difficulty_target);

            eli_blockchain::blockchain::block b(prev_block_hash, timestamp, root_hash, nonce, difficulty_target, transactions);
            this->block_chain.push_back(b);
        }

        std::stringstream get_block_header(const size_t &index) const
        {
            if (index >= this->block_chain.size())
            {
                throw std::out_of_range("Block index out of range");
            }

            const eli_blockchain::blockchain::block &b = this->block_chain.at(index);
            std::stringstream ss;
            ss << "Block Index: " << index << "\n";
            ss << "Previous Block Hash: " << b.prev_block_hash << "\n";
            ss << "Timestamp: " << b.get_timestamp() << "\n";
            ss << "Version: " << b.version << "\n";
            ss << "Root Hash: " << b.root_hash << "\n";
            ss << "Nonce: " << b.nonce << "\n";
            ss << "Difficulty Target: " << b.difficulty_target << "\n";
            ss << "Block Checksum: " << b.get_checksum() << "\n";

            return ss;
        }

        std::stringstream get_block_header() const
        {
            return get_block_header(this->block_chain.size() - 1);
        }

        std::stringstream get_block_body(const size_t &index) const
        {
            if (index >= this->block_chain.size())
            {
                throw std::out_of_range("Block index out of range");
            }

            const eli_blockchain::blockchain::block &b = this->block_chain.at(index);
            std::stringstream ss;
            ss << "Transactions:\n";
            for (const auto &t : b.get_transactions())
            {
                ss << t.str() << "\n";
            }

            return ss;
        }

        std::stringstream get_block_body() const
        {
            return get_block_body(this->block_chain.size() - 1);
        }
    };

};

#endif // ELI_BLOCK