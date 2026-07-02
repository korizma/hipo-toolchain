#pragma once

#include <string>
#include <vector>

using namespace std;

string find_first_option_value_equals(vector<string> args, string option);

vector<string> find_all_option_values_equals(vector<string> args, string option);

string find_first_option_value_seperate(vector<string> args, string option);

vector<string> find_all_option_values_seperate(vector<string> args, string option);

vector<string> find_all_non_options(vector<string> args);

bool find_option(vector<string> args, string option);