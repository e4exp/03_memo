#include "stdafx.h"
#include "Utils.h"
#include <ctime>
#include "time.h"

#include <iostream>
#include <fstream>
#include <sstream>

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


std::string get_string_from_file(char* file_name) {

	//get connection string from file
	std::ifstream fin(file_name);
	if (!fin) {
		AfxMessageBox("file cant be read");
		return "";
	}
	std::stringstream strstream;
	strstream << fin.rdbuf();
	fin.close();
	//content -> string
	std::string str(strstream.str());

	return str;
}

