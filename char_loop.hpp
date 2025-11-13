#ifndef CHAR_LOOP
#define CHAR_LOOP

#include <iostream>
#define MIN_CHAR 0
#define MAX_CHAR 255

class char_loop
{
public:
    std::string current;

    char_loop(const std::string &_current = "") : current(_current)
    {
        if (_current.empty())
        {
            this->current = static_cast<char>(MIN_CHAR);
        }
    }

    void increment_character()
    {
        for (size_t i = 0; i < this->current.length(); i++)
        {
            if (this->current[i] != static_cast<char>(MAX_CHAR))
            {
                this->current[i]++;
                break;
            }

            this->current[i] = static_cast<char>(MIN_CHAR);

            if (i == this->current.length() - 1)
            {
                this->current += static_cast<char>(MIN_CHAR);
                break;
            }
        }
    }

    std::string get()
    {
        return current;
    }

    std::string get_and_increment()
    {
        this->increment_character();
        return current;
    }
};

#endif