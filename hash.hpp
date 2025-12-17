#ifndef ELI_HASH
#define ELI_HASH

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

namespace eli_hash
{
    // 64-bit FNV-1a hash implemented for deterministic, fast hashing.
    // Returns a lowercase hexadecimal string of length 16 (64 bits / 4 = 16 hex digits).
    inline uint64_t fnv1a_64(const std::string &input)
    {
        const uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
        const uint64_t FNV_PRIME = 1099511628211ULL;

        uint64_t hash = FNV_OFFSET_BASIS;
        for (unsigned char c : input)
        {
            hash ^= static_cast<uint64_t>(c);
            hash *= FNV_PRIME;
        }

        return hash;
    }

    inline std::string hash(const std::string &input)
    {
        uint64_t value = fnv1a_64(input);
        std::ostringstream oss;
        oss << std::hex << std::setfill('0') << std::nouppercase << std::setw(16) << value;
        return oss.str();
    }

} // namespace eli_hash

#endif // ELI_HASH