#include "stdafx.h"
#include "Picker.h"
#include "MongoSaver.h"

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
		holder[row][3] = value;
	}
	else if (field == STR(updated)) {
		holder[row][4] = value;
	}
	else if (field == STR(deleted)) {
		holder[row][5] = value;
	}


}