#include "stdafx.h"
#include "Picker.h"
#include "MongoSaver.h"
#include <ctime>
#include "time.h"


CString utc_to_local(CString value) {

	const time_t utc = _ttol(value);
	struct tm now;

	localtime_s(&now, &utc);
	char result[32];
	asctime_s(result, sizeof(result), &now);
	std::string str(result);
	return str.c_str();
}

void Picker::store_field( int row, CString field, CString value) {


	if (field == STR(id)) {
		holder[row][0] = value;
	}
	else if (field == STR(title)) {
		holder[row][1] = value;
	}
	else if (field == STR(note)) {
		holder[row][2] = value;
	}
	else if (field == STR(created)) {
		//convert UTC -> local	
		holder[row][3] = utc_to_local(value);
	}

	else if (field == STR(updated)) {	
		holder[row][4] = utc_to_local(value);
	}
	else if (field == STR(deleted)) {
		holder[row][5] = value;
	}


}