#pragma once

#include <string>
#include <vector>

using namespace std;

// returns a vector of length 8 with the X turned into 8 bytes
vector<char> long_to_8_bytes(long x);

// returns a vector of length 4 with the X turned into 4 bytes
vector<char> int_to_4_bytes(int x);

// mem before
//      offset
//       || 
// .. XX XX XX XX
// mem after
//      offset
//       || 
// .. XX LL HX XX
void write_to_lower_12b(vector<char>& mem, long offset, int value_12bit);

// returns true if the long value can fit in 12 bits, false otherwise
bool long_fits_in_12bits(long x);

string char_to_string_hex(char x);

string long_to_string_hex(long x);
