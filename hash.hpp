#ifndef ELI_HASH
#define ELI_HASH

#include <iostream>

// Number of characters in the output hash
#define HASH_OUTPUT_SIZE 64

namespace eli_hash
{
    // Global incrementor used in the hash functions to add variability
    // size_t incrementor = 1;

    // Concatenates two number (unsinged short) values
    size_t concatenate(const unsigned short a, const unsigned short b)
    {
        std::string result = std::to_string(a);
        result += std::to_string(b);
        return std::stoll(result);
    }

    // The ASCII character range the hash functions splitChar and combineChars return is from 33 to 126. Total unique characters: 94

    // Part of the hash function. Takes one character and determanistically returns two random characters based on the input character and the incrementor
    std::string splitChar(unsigned char target, size_t &incrementor)
    {
        std::string split;
        if (target < 33)
        {
            target += 33 + ++incrementor;
        }

        // ASCII Characters in use are from 33 to 126. Total unique characters: 94
        split += static_cast<char>((target + ++incrementor - 33) * 3 % 94 + 33);
        split += static_cast<char>((split.at(0) + ++incrementor - 33) * 3 % 94 + 33);
        return split;
    }

    // Part of the hash function. Takes two characters and determanistacally returns one random character based on both input characters and the incrementor
    char combineChars(unsigned const char a, unsigned const char b, size_t &incrementor)
    {
        auto position = concatenate(a, b);

        if (position < 33)
        {
            position += 33 + ++incrementor;
        }

        return static_cast<char>((position + ++incrementor - 33) * 3 % 94 + 33);
    }

    std::string hash(std::string input, size_t incrementor = 1)
    {
        // The input variable is set as the string to be hashed, used as the intermediary for the hash function and is the outputed hashed

        // Can't hash an empty string
        if (input.size() == 0)
        {
            std::cerr << "Input is empty" << std::endl;
            return "";
        }

        // Hash start

        // If the input is too small expand it to required size
        for (size_t i = 0; input.size() < HASH_OUTPUT_SIZE; i++)
        {
            auto split = splitChar(input.at(i), incrementor);
            input.at(0) = split.at(0);
            input += split.at(1);
        }

        // Hash process from left to right
        input.at(0) = combineChars(input.at(0), input.at(1), incrementor);
        for (size_t i = 1; i < input.size(); i++)
        {
            input.at(i) = combineChars(input.at(i - 1), input.at(i), incrementor);
        }

        // Hash process from right to left
        for (size_t i = input.size() - 2; i > 0; i--)
        {
            input.at(i) = combineChars(input.at(i + 1), input.at(i), incrementor);
        }
        input.at(0) = combineChars(input.at(0), input.at(1), incrementor);

        // If the input is too large shrink it to required size
        for (size_t i = 0; input.size() > HASH_OUTPUT_SIZE; i++)
        {
            if (i >= input.size())
            {
                i = 0;
            }
            input.at(i) = combineChars(input.back(), input.at(i), incrementor);
            input.pop_back();
        }

        // Result output
        return input;
    }
}

#endif