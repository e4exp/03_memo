#pragma once
#include <iostream>

std::string cstr_to_str(CString cstr);
long get_unix_time();
CString utc_to_local(CString value);