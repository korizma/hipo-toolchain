#include "misc.hpp"
#include <sstream>
#include <fstream>


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
void write_to_lower_12b(vector<char>& mem, long offset, int value_12bit)
{
    int value = value_12bit & 0xFFF;
    mem[offset] &= 0xF0;
    mem[offset] |= (char)(value & 0x0F);
    mem[offset + 1] = (char)((value >> 4) & 0xFF);
}

bool long_fits_in_12bits(long x)
{
    return (x >= -2048 && x <= 2047);
}

string char_to_string_hex(char x)
{
    string hex_chars = "0123456789ABCDEF";
    string result = "";
    result += hex_chars[(x >> 4) & 0x0F];
    result += hex_chars[x & 0x0F];
    return result;
}


string long_to_string_hex(long x)
{
    string hex_chars = "0123456789ABCDEF";
    string result = "";
    for (int i = 7; i >= 0; i--)
    {
        char byte = (x >> (i * 8)) & 0xFF;
        result += hex_chars[(byte >> 4) & 0x0F];
        result += hex_chars[byte & 0x0F];
    }
    return result;
}

vector<string> split_string(const string& text, char delimiter) {
    vector<string> parts;
    istringstream stream(text);

    string part;
    while (getline(stream, part, delimiter)) {
        parts.push_back(part);
    }

    return parts;
}

string trim_string(string text)
{
    size_t start = text.find_first_not_of(" \t\r\n");
    if (start == string::npos)
        return "";

    size_t end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}


char string_hex_to_char(string hex)
{
    string hex_chars = "0123456789ABCDEF";
    char result = 0;
    for (int i = 0; i < 2; i++)
    {
        char c = hex[i];
        int value = hex_chars.find(c);
        result = (result << 4) | value;
    }
    return result;
}


string read_all_text(const string& filename)
{
    ifstream file(filename);
    if (!file)
        return "";

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


string int_to_string_hex(int number)
{
    string hex_chars = "0123456789ABCDEF";
    string result = "0x";
    for (int i = 3; i >= 0; i--)
    {
        char byte = (number >> (i * 8)) & 0xFF;
        result += hex_chars[(byte >> 4) & 0x0F];
        result += hex_chars[byte & 0x0F];
    }
    return result;
}
