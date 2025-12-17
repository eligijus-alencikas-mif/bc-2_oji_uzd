#ifndef ELI_USERS
#define ELI_USERS

#include <string>
#include <vector>
#include "char_loop.hpp"
#include "hash.hpp"

namespace eli_users
{
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

    class user_pool
    {
    private:
        char_loop public_key_generator;
        std::vector<user> users;

    public:
        void add_user(const std::string &_name, const double &_balance)
        {
            users.emplace_back(_name, eli_hash::hash(this->public_key_generator.get_and_increment() + _name), _balance);
        }

        bool transfer_balance(const std::string &_from_public_key, const std::string &_to_public_key, const double &amount)
        {
            if (get_user_balance(_from_public_key) < amount && amount > 0)
            {
                return false;
            }

            for (auto &u : users)
            {
                if (u.get_public_key() == _from_public_key)
                {
                    u.adjust_balance(-amount);
                }
                else if (u.get_public_key() == _to_public_key)
                {
                    u.adjust_balance(amount);
                }
            }

            return true;
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

        std::vector<std::string> get_all_user_public_keys() const
        {
            std::vector<std::string> public_keys;
            for (const auto &u : users)
            {
                public_keys.push_back(u.get_public_key());
            }
            return public_keys;
        }

        std::vector<user> get_all_users() const
        {
            return this->users;
        }
    };
}

#endif // ELI_USERS