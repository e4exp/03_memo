#include "stdafx.h"
#include "Utils.h"


std::string cstr_to_str(CString cstr) {
	std::string str = static_cast<std::string>(cstr);
	return str;
}
