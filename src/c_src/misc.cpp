#include "misc.hpp"

using namespace std;

vector<char> long_to_8_bytes(long x)
{
    vector<char> bytes(8);
    bytes[0] = (char)(x & 0xFF);
    bytes[1] = (char)((x >> 8) & 0xFF);
    bytes[2] = (char)((x >> 16) & 0xFF);
    bytes[3] = (char)((x >> 24) & 0xFF);
    bytes[4] = (char)((x >> 32) & 0xFF);
    bytes[5] = (char)((x >> 40) & 0xFF);
    bytes[6] = (char)((x >> 48) & 0xFF);
    bytes[7] = (char)((x >> 56) & 0xFF);

    return bytes;
}

vector<char> int_to_4_bytes(int x)
{
    vector<char> bytes(4);
    bytes[0] = (char)(x & 0xFF);
    bytes[1] = (char)((x >> 8) & 0xFF);
    bytes[2] = (char)((x >> 16) & 0xFF);
    bytes[3] = (char)((x >> 24) & 0xFF);

    return bytes;
}

// mem before
//      offset
//       || 
// .. XX XX XX XX
// mem after
//      offset
//       || 
// .. XX LL HX XX
void write_to_lower_12b(vector<char> mem, long offset, int value_12bit)
{
    mem[offset] = (char)(value_12bit & 0xFF);
    mem[offset + 1] &= 0xF0;
    mem[offset + 1] |= (char)((value_12bit >> 8) & 0x0F);
}

bool long_fits_in_12bits(long x)
{
    return (x >= -2048 && x <= 2047);
}