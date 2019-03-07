#pragma once

#include "stdafx.h"
#include<vector>


class Picker {

public:
	void store_field(int row, CString field, CString value);

public:
	std::vector<std::vector<CString>> holder;
	int selected_idx = 0;
	int whole_cnt = 0;
	/*
	CString old_title="" ;
	CString old_body = "";
	*/


};


