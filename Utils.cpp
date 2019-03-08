#include "stdafx.h"
#include "Utils.h"
#include <ctime>
#include "time.h"


std::string cstr_to_str(CString cstr) {
	std::string str = static_cast<std::string>(cstr);
	return str;
}


long get_unix_time() {

	time_t now = std::time(nullptr);
	return now;

}

CString utc_to_local(CString value) {

	const time_t utc = _ttol(value);
	struct tm now;

	localtime_s(&now, &utc);
	char result[32];
	asctime_s(result, sizeof(result), &now);
	std::string str(result);
	return str.c_str();
}