#pragma once
#include <iostream>

std::string cstr_to_str(CString cstr);
long get_unix_time();
CString utc_to_local(CString value);
std::string get_string_from_file(char* file_name);