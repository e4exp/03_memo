#pragma once

#include "stdafx.h"
#include<vector>


class Picker {

public:
	void store_field(int row, CString field, CString value);

public:
	std::vector<std::vector<CString>> holder;
	//int selected_idx = 0;
	CString selected_id = "";
	int whole_cnt = 0;
	/*
	CString old_title="" ;
	CString old_body = "";
	*/

	const int UPDATE_TITLE_LIMIT = 10;
	const int UPDATE_BODY_LIMIT = 10;
	int update_title_count=0;
	int update_body_count=0;


};


