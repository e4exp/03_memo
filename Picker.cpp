#include "stdafx.h"
#include "Picker.h"
#include "MongoSaver.h"
#include "Utils.h"

void Picker::store_field( int row, CString field, CString value) {


	if (field == STR(_id)) {
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