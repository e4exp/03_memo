#ifndef INCLUDE_GUARD_SAVER_H
#define INCLUDE_GUARD_SAVER_H


#include "stdafx.h"
#include "./sqlite3.h"
#include<vector>

// sqlite3.dll内関数ポインタ型定義
typedef int(__cdecl * pOpen)(char *, sqlite3 **);
typedef int(__cdecl * pExec)(sqlite3 *, const char *, sqlite3_callback, void *, char **);
typedef int(__cdecl * pGetTable)(sqlite3 *, const char *, char ***, int *, int *, char **);
typedef int(__cdecl * pFreeTable)(char **);
typedef int(__cdecl * pClose)(sqlite3 *);

class Saver {



public:
	Saver();
	~Saver();

	void store_note(CString str);
	int load_notes();
	

public:

	// 関数ポインタ変数
	pOpen sqlite3_open;
	pExec sqlite3_exec;
	pGetTable sqlite3_get_table;
	pFreeTable sqlite3_free_table;
	pClose sqlite3_close;


	HINSTANCE hDll;
	sqlite3 *db;
	char *err;
	char **result;
	int row, col;
	
	char* db_name;

	static const int COLUMN_NUM = 4; //all columns
	int notes_cnt;
	//CString **picker; // [][COLUMN_NUM];
	std::vector<std::vector<CString>> picker;



};


#endif // INCLUDE_GUARD_HOGE_HPP
